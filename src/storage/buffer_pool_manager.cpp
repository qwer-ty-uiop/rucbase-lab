/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "buffer_pool_manager.h"

/**
 * 锁策略总览：
 *
 * 本缓冲池使用 std::shared_mutex 读写锁，遵循两条核心原则：
 *
 * 1. 读写分离：页面命中（高频路径）使用 shared_lock 允许并发读，
 *    页面未命中/修改数据结构（低频路径）使用 unique_lock 独占写。
 *
 * 2. I/O 期间释放锁：磁盘 I/O 是慢操作（毫秒级），持锁期间做 I/O
 *    会严重阻塞其他线程。优化方式为：持锁复制数据 → 释放锁 → 磁盘写入 →
 *    重新获取锁更新元数据（需重新验证映射关系，防止页面在释放锁期间被淘汰替换）。
 */

/**
 * @description: 找一个可用帧，优先从空闲列表获取，如果没有空闲帧则淘汰一个页面
 * @return {bool} true: 找到可用帧, false: 缓冲池已满且无可淘汰页面
 * @param {frame_id_t*} frame_id 输出参数，返回找到的帧编号
 * @note 调用者必须已持有 latch_ 写锁
 */
bool BufferPoolManager::find_victim_page(frame_id_t* frame_id) {
    // 优先从空闲列表获取帧（无需淘汰，直接使用）
    if (free_list_.empty())
        // 空闲列表为空，缓冲池已满，使用替换策略选择一个可淘汰页面
        return replacer_->victim(frame_id);
    else {
        // 空闲列表非空，取出第一个空闲帧
        *frame_id = free_list_.front();
        free_list_.pop_front();
        return true;
    }
}

/**
 * @description: 将帧中的旧页面替换为新页面。如果旧页面是脏页则先写回磁盘，
 *              然后更新页表映射，重置页面数据
 * @param {Page*} page 指向帧中当前页面的指针
 * @param {PageId} new_page_id 新分配的页面ID
 * @param {frame_id_t} new_frame_id 帧编号（用于更新页表）
 * @note 调用者必须已持有 latch_ 写锁
 */
void BufferPoolManager::update_page(Page* page,
                                    PageId new_page_id,
                                    frame_id_t new_frame_id) {
    // 步骤1：如果旧页面是脏页且不是无效页面，先写回磁盘
    if (page->is_dirty() && page->get_page_id().page_no != INVALID_PAGE_ID) {
        disk_manager_->write_page(page->get_page_id().fd,
                                  page->get_page_id().page_no, page->get_data(),
                                  PAGE_SIZE);
        page->is_dirty_ = false;
    }
    // 步骤2：更新页表映射——移除旧页面ID的映射，建立新页面ID到帧编号的映射
    page_table_.erase(page->get_page_id());
    page_table_[new_page_id] = new_frame_id;
    // 步骤3：重置页面数据，设置新的页面ID
    page->reset_memory();
    page->id_ = new_page_id;
}

/**
 * @description: 从缓冲池获取指定页面。如果页面已在缓冲池中则直接返回（页面命中），
 *              否则从磁盘加载到缓冲池（页面未命中）。
 *              使用读写锁 + 双重检查优化并发性能：
 *              - 页面命中（高频路径）：shared_lock，允许多线程并发读
 *              - 页面未命中（低频路径）：unique_lock，独占修改数据结构
 *              - 双重检查：获取写锁后再次检查页面是否已被其他线程加载
 * @return {Page*} 成功返回页面指针，缓冲池满且无可淘汰页面时返回 nullptr
 * @param {PageId} page_id 目标页面的 PageId（fd + page_no）
 */
Page* BufferPoolManager::fetch_page(PageId page_id) {
    // 第一步：读锁检查页面是否已在缓冲池中（高频路径，允许多线程并发读）
    {
        std::shared_lock lock{latch_};
        auto it = page_table_.find(page_id);
        if (it != page_table_.end()) {
            // 页面命中：增加引用计数，标记为正在使用（不可淘汰）
            frame_id_t frame_id = it->second;
            replacer_->pin(frame_id);
            Page* page = &pages_[frame_id];
            page->pin_count_++;
            return page;
        }
    }

    // 第二步：页面未命中，需要写锁修改数据结构（低频路径）
    {
        std::unique_lock lock{latch_};

        // 双重检查：获取写锁后再次查找，防止其他线程在等待写锁期间已加载该页面
        auto it = page_table_.find(page_id);
        if (it != page_table_.end()) {
            // 其他线程已加载该页面，直接返回
            frame_id_t frame_id = it->second;
            replacer_->pin(frame_id);
            Page* page = &pages_[frame_id];
            page->pin_count_++;
            return page;
        }

        // 确认页面不在缓冲池中，执行淘汰和加载
        // 2.1 找一个可用帧（优先空闲列表，其次淘汰）
        frame_id_t frame_id;
        if (!find_victim_page(&frame_id)) {
            return nullptr;
        }

        // 2.2 将帧中的旧页面替换为新页面（脏页写回 + 页表更新 + 数据重置）
        Page* page = &pages_[frame_id];
        update_page(page, page_id, frame_id);

        // 2.3 从磁盘读取页面数据到帧中
        disk_manager_->read_page(page_id.fd, page_id.page_no, page->data_,
                                 PAGE_SIZE);

        // 2.4 标记页面为正在使用，设置引用计数为1
        replacer_->pin(frame_id);
        page->pin_count_ = 1;

        return page;
    }
}

/**
 * @description: 取消固定一个页面（减少 pin_count）。当 pin_count 降为 0 时，
 *              页面变为可淘汰状态，加入替换策略的候选列表。
 *              使用读锁快速路径：页面不存在时快速返回，避免无谓的写锁竞争。
 * @return {bool} true: 成功取消固定; false: 页面不存在或 pin_count 已为 0
 * @param {PageId} page_id 目标页面的 PageId
 * @param {bool} is_dirty 是否将页面标记为脏页
 */
bool BufferPoolManager::unpin_page(PageId page_id, bool is_dirty) {
    // 快速路径：读锁检查页面是否存在，不存在则直接返回
    // 避免每次都获取写锁，减少并发冲突
    {
        std::shared_lock lock{latch_};
        if (page_table_.find(page_id) == page_table_.end()) {
            return false;
        }
    }

    // 慢路径：写锁修改 pin_count 和 replacer 状态
    std::unique_lock lock{latch_};
    auto it = page_table_.find(page_id);
    // 再次检查：页面可能在读锁释放到写锁获取期间被其他线程删除
    if (it == page_table_.end()) {
        return false;
    }
    frame_id_t frame_id = it->second;
    Page* page = &pages_[frame_id];
    // pin_count 已为 0，说明页面已处于可淘汰状态，不能再次取消固定
    if (page->pin_count_ == 0)
        return false;
    // 减少 pin_count
    page->pin_count_--;
    // pin_count 降为 0：页面变为可淘汰状态，加入替换策略的候选列表
    if (page->pin_count_ == 0)
        replacer_->unpin(frame_id);
    // 根据参数标记脏页
    if (is_dirty) {
        page->is_dirty_ = true;
    }
    return true;
}

/**
 * @description: 将指定页面写回磁盘（无论是否为脏页都写入）。
 *              使用三阶段锁策略，避免持锁期间进行磁盘 I/O：
 *              阶段1：读锁复制页面数据到栈上缓冲区
 *              阶段2：无锁执行磁盘写入
 *              阶段3：写锁验证映射关系并清除脏标记
 * @return {bool} true: 成功写回; false: 页面不在缓冲池中或 page_id 无效
 * @param {PageId} page_id 目标页面的 PageId，page_no 不能为 INVALID_PAGE_ID
 */
bool BufferPoolManager::flush_page(PageId page_id) {
    // 栈上缓冲区，用于在释放锁后安全地持有页面数据副本
    char data_copy[PAGE_SIZE];
    frame_id_t flush_frame_id;

    // 阶段1：读锁——查找页面并复制数据
    {
        std::shared_lock lock{latch_};
        auto it = page_table_.find(page_id);
        if (it == page_table_.end() || page_id.page_no == INVALID_PAGE_ID) {
            return false;
        }
        flush_frame_id = it->second;
        // 复制页面数据，释放锁后仍可安全使用
        std::memcpy(data_copy, pages_[flush_frame_id].data_, PAGE_SIZE);
    }

    // 阶段2：无锁——执行磁盘写入（慢操作，不阻塞其他线程）
    disk_manager_->write_page(page_id.fd, page_id.page_no, data_copy,
                              PAGE_SIZE);

    // 阶段3：写锁——验证映射关系未变后清除脏标记
    // 必须验证：释放锁期间其他线程可能淘汰了该页面，导致映射关系改变
    {
        std::unique_lock lock{latch_};
        auto it = page_table_.find(page_id);
        if (it != page_table_.end() && it->second == flush_frame_id) {
            // 映射关系未变，安全清除脏标记
            pages_[flush_frame_id].is_dirty_ = false;
        }
        // 映射关系已变：页面已被淘汰替换，不做任何修改
    }
    return true;
}

/**
 * @description: 创建一个新页面。从缓冲池中分配一个可用帧，
 *              在磁盘上分配新的 page_no，将帧与新页面关联。
 *              需要同时修改 free_list_、page_table_、replacer_ 等多个数据结构，
 *              因此全程持有写锁，无法分段释放。
 * @return {Page*} 成功返回新页面指针，缓冲池满且无可淘汰页面时返回 nullptr
 * @param {PageId*} page_id 输入输出参数，fd 由调用者设置，page_no 由本函数分配
 */
Page* BufferPoolManager::new_page(PageId* page_id) {
    std::unique_lock lock{latch_};

    // 步骤1：找一个可用帧
    frame_id_t victim_fid;
    if (!find_victim_page(&victim_fid)) {
        return nullptr;
    }

    // 步骤2：在磁盘上分配新的 page_no
    page_id->page_no = disk_manager_->allocate_page(page_id->fd);

    // 步骤3：将帧中的旧页面替换为新页面（脏页写回 + 页表更新 + 数据重置）
    Page* victim_page = &pages_[victim_fid];
    update_page(victim_page, *page_id, victim_fid);

    // 步骤4：标记帧为正在使用
    replacer_->pin(victim_fid);
    victim_page->pin_count_ = 1;

    return victim_page;
}

/**
 * @description: 从缓冲池删除指定页面。如果页面正在被使用（pin_count > 0）则无法删除。
 *              删除后帧归还到空闲列表。如果是脏页，先复制数据再释放锁后写回磁盘，
 *              避免持锁期间进行磁盘 I/O。
 * @return {bool} true: 页面不存在或成功删除; false: 页面正在被使用（pin_count > 0）
 * @param {PageId} page_id 目标页面的 PageId
 */
bool BufferPoolManager::delete_page(PageId page_id) {
    bool was_dirty = false;
    char data_copy[PAGE_SIZE];

    {
        std::unique_lock lock{latch_};

        // 步骤1：查找页面，不存在则直接返回 true
        auto it = page_table_.find(page_id);
        if (it == page_table_.end()) {
            return true;
        }

        // 步骤2：检查页面是否正在被使用，正在使用则无法删除
        frame_id_t frame_id = it->second;
        Page* page = &pages_[frame_id];
        if (page->pin_count_ != 0) {
            return false;
        }

        // 步骤3：如果是脏页，复制数据到栈上缓冲区（释放锁后写回磁盘）
        if (page->is_dirty()) {
            was_dirty = true;
            std::memcpy(data_copy, page->get_data(), PAGE_SIZE);
        }

        // 步骤4：从页表中移除该页面的映射
        page_table_.erase(it);

        // 步骤5：重置页面元数据，使帧回到初始状态
        page->reset_memory();
        page->id_.fd = -1;
        page->id_.page_no = INVALID_PAGE_ID;
        page->is_dirty_ = false;
        page->pin_count_ = 0;

        // 步骤6：从替换策略中移除（如果存在）
        replacer_->pin(frame_id);

        // 步骤7：将帧归还到空闲列表
        free_list_.push_back(frame_id);
    }

    // 步骤8：释放锁后，将脏页数据写回磁盘（不阻塞其他线程）
    if (was_dirty) {
        disk_manager_->write_page(page_id.fd, page_id.page_no, data_copy,
                                  PAGE_SIZE);
    }

    return true;
}

/**
 * @description: 将指定文件的所有页面写回磁盘。
 *              使用三阶段锁策略逐页刷新，避免在遍历大量页框和磁盘 I/O 期间持锁：
 *              阶段1：读锁收集目标文件的页面列表
 *              阶段2：逐页执行三阶段刷新（读锁复制 → 无锁写磁盘 → 写锁清脏标记）
 * @param {int} fd 目标文件的文件描述符
 */
void BufferPoolManager::flush_all_pages(int fd) {
    // 阶段1：读锁收集目标文件的所有页面信息
    std::vector<std::pair<PageId, frame_id_t>> pages_to_flush;
    {
        std::shared_lock lock{latch_};
        for (size_t i = 0; i < pool_size_; i++) {
            Page* page = &pages_[i];
            if (page->get_page_id().fd == fd &&
                page->get_page_id().page_no != INVALID_PAGE_ID) {
                pages_to_flush.emplace_back(page->get_page_id(),
                                            static_cast<frame_id_t>(i));
            }
        }
    }

    // 阶段2：逐页执行三阶段刷新
    for (auto& [pid, fid] : pages_to_flush) {
        char data_copy[PAGE_SIZE];
        bool need_flush = false;

        // 2.1 读锁：验证页面仍在原帧中，复制数据
        {
            std::shared_lock lock{latch_};
            auto it = page_table_.find(pid);
            // 验证映射关系：页面可能已被淘汰替换到其他帧
            if (it != page_table_.end() && it->second == fid) {
                std::memcpy(data_copy, pages_[fid].data_, PAGE_SIZE);
                need_flush = true;
            }
        }

        // 2.2 无锁：执行磁盘写入
        if (need_flush) {
            disk_manager_->write_page(pid.fd, pid.page_no, data_copy,
                                      PAGE_SIZE);

            // 2.3 写锁：验证映射关系未变后清除脏标记
            std::unique_lock lock{latch_};
            auto it = page_table_.find(pid);
            if (it != page_table_.end() && it->second == fid) {
                pages_[fid].is_dirty_ = false;
            }
        }
    }
}

/**
 * @description: 工厂方法，根据配置创建页面替换策略实例
 * @param {size_t} pool_size 缓冲池大小，传递给替换策略构造函数
 * @return {std::unique_ptr<Replacer>} 替换策略实例
 */
std::unique_ptr<Replacer> BufferPoolManager::create_replacer(size_t pool_size) {
        if (REPLACER_TYPE == "CLOCK")
            return std::make_unique<ClockReplacer>(pool_size);
        return std::make_unique<LRUReplacer>(pool_size);
}

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
 * 本缓冲池使用 std::shared_mutex 读写锁，遵循三条核心原则：
 *
 * 1. 读写分离：页面命中（高频路径）使用 shared_lock 允许并发读，
 *    页面未命中/修改数据结构（低频路径）使用 unique_lock 独占写。
 *
 * 2. I/O 期间释放锁（reserve-and-load 模式）：磁盘 I/O 是慢操作（毫秒级），
 *    持锁期间做 I/O 会严重阻塞其他线程。优化方式为：
 *    - 写锁内：修改元数据、设置 io_pending_ 标志、pin 防淘汰 → 释放锁
 *    - 无锁：执行磁盘读写
 *    - 写锁内：复制数据、清除 io_pending_、通知等待线程
 *    其他线程遇到 io_pending_=true 的页面时，通过条件变量等待 I/O 完成。
 *
 * 3. io_cv_.wait() 后必须重新验证页面映射：
 *    wait 释放锁期间，页面可能被淘汰替换到其他帧，唤醒后 frame_id 对应的
 *    可能已经是完全不同的页面。必须重新检查 page_table_[page_id] == frame_id。
 */

/**
 * @description: 找一个可用帧，优先从空闲列表获取，如果没有空闲帧则淘汰一个页面
 * @return {bool} true: 找到可用帧, false: 缓冲池已满且无可淘汰页面
 * @param {frame_id_t*} frame_id 输出参数，返回找到的帧编号
 * @note 调用者必须已持有 latch_ 写锁
 */
bool BufferPoolManager::find_victim_page(frame_id_t* frame_id) {
    if (free_list_.empty())
        return replacer_->victim(frame_id);
    else {
        *frame_id = free_list_.front();
        free_list_.pop_front();
        return true;
    }
}

/**
 * @description: 更新页表映射和页面元数据（不执行磁盘 I/O）。
 *              旧脏页数据由调用者在释放锁后写回磁盘。
 *              调用流程：
 *              1. 调用前：先保存旧脏页数据到栈缓冲区（用于释放锁后写回）
 *              2. 调用本函数：更新页表映射 + 重置页面数据 + 设置新 PageId
 *              3. 调用后：设置 io_pending_、pin、pin_count_ 等
 * @param {Page*} page 指向帧中当前页面的指针
 * @param {PageId} new_page_id 新分配的页面ID
 * @param {frame_id_t} new_frame_id 帧编号（用于更新页表）
 * @note 调用者必须已持有 latch_ 写锁
 */
void BufferPoolManager::update_page(Page* page,
                                    PageId new_page_id,
                                    frame_id_t new_frame_id) {
    page_table_.erase(page->get_page_id());
    page_table_[new_page_id] = new_frame_id;
    page->reset_memory();
    page->id_ = new_page_id;
}

/**
 * @description: 等待页面 I/O 完成并验证映射关系是否仍然有效。
 *              io_cv_.wait() 会临时释放锁，期间页面可能被淘汰替换，
 *              唤醒后必须重新检查 page_table_[page_id] == frame_id。
 * @tparam Lock shared_lock 或 unique_lock
 * @param {Lock&} lock 当前持有的锁（wait 会临时释放）
 * @param {Page*} page 目标页面
 * @param {PageId} page_id 目标页面ID（用于重新查找映射）
 * @param {frame_id_t} frame_id 期望的帧编号（用于验证映射是否改变）
 * @return {bool} true: 映射关系有效（页面仍在原帧）; false: 映射已变（页面被替换）
 */
template<typename Lock>
bool BufferPoolManager::wait_io_and_verify(Lock& lock, Page* page,
                                           PageId page_id,
                                           frame_id_t frame_id) {
    if (!page->io_pending_.load()) {
        return true;
    }
    page->io_cv_.wait(lock, [&] { return !page->io_pending_.load(); });
    auto it = page_table_.find(page_id);
    return it != page_table_.end() && it->second == frame_id;
}

/**
 * @description: 从缓冲池获取指定页面。如果页面已在缓冲池中则直接返回（页面命中），
 *              否则从磁盘加载到缓冲池（页面未命中）。
 *              使用 reserve-and-load 模式，将磁盘 I/O 移到锁外。
 *
 *              执行流程：
 *              ┌─ 第一步：shared_lock 查找 page_table_（高频路径）
 *              │  ├─ 命中且映射有效：pin + pin_count++ → 返回
 *              │  └─ io_pending 或映射已变：放弃读锁，进入写锁路径
 *              └─ 未命中：放弃读锁
 *
 *              ┌─ 阶段1（unique_lock）：预约帧、修改元数据
 *              │  ├─ 双重检查：再次查找 page_table_
 *              │  ├─ 找到可用帧（free_list 或 replacer）
 *              │  ├─ 保存旧脏页数据到栈缓冲区
 *              │  ├─ update_page：更新页表映射 + 重置页面
 *              │  ├─ 设 io_pending_=true：通知其他线程等待
 *              │  └─ pin + pin_count_=1：防止帧被淘汰
 *              │
 *              ├─ 阶段2（无锁）：执行磁盘 I/O
 *              │  ├─ 写旧脏页到磁盘
 *              │  └─ 读新页面到栈缓冲区
 *              │
 *              └─ 阶段3（unique_lock）：完成加载
 *                 ├─ 将栈缓冲区数据复制到 page->data_
 *                 ├─ 清 io_pending_=false
 *                 └─ notify_all：唤醒等待的线程
 *
 * @return {Page*} 成功返回页面指针，缓冲池满且无可淘汰页面时返回 nullptr
 * @param {PageId} page_id 目标页面的 PageId（fd + page_no）
 */
Page* BufferPoolManager::fetch_page(PageId page_id) {
    // 局部函数：pin 页面并增加引用计数后返回
    auto pin_and_return = [&](frame_id_t fid) -> Page* {
        replacer_->pin(fid);
        Page* p = &pages_[fid];
        p->pin_count_++;
        return p;
    };

    // 第一步：读锁检查页面是否已在缓冲池中（高频路径，允许多线程并发读）
    {
        std::shared_lock lock{latch_};
        auto it = page_table_.find(page_id);
        if (it != page_table_.end()) {
            frame_id_t frame_id = it->second;
            Page* page = &pages_[frame_id];
            // 等待 I/O 完成并验证映射，有效则直接返回
            if (wait_io_and_verify(lock, page, page_id, frame_id)) {
                return pin_and_return(frame_id);
            }
            // 映射已变，放弃读锁，走写锁路径重新加载
        }
    }

    // 以下变量在阶段2（无锁）中使用，需要在写锁内提前保存
    char old_data_copy[PAGE_SIZE];
    PageId old_page_id;
    bool old_page_dirty = false;
    frame_id_t frame_id;

    // 阶段1：写锁——预约帧、修改元数据、标记 I/O 进行中
    {
        std::unique_lock lock{latch_};

        // 双重检查：获取写锁后再次查找
        auto it = page_table_.find(page_id);
        if (it != page_table_.end()) {
            frame_id_t fid = it->second;
            Page* page = &pages_[fid];
            if (wait_io_and_verify(lock, page, page_id, fid)) {
                return pin_and_return(fid);
            }
            // 映射已变，继续走未命中路径
        }

        // 确认页面不在缓冲池中，找一个可用帧
        if (!find_victim_page(&frame_id)) {
            return nullptr;
        }

        Page* page = &pages_[frame_id];

        // 保存旧页面信息，必须在 update_page 之前（update_page 会重置数据）
        old_page_id = page->get_page_id();
        old_page_dirty = page->is_dirty_ && old_page_id.page_no != INVALID_PAGE_ID;
        if (old_page_dirty) {
            std::memcpy(old_data_copy, page->data_, PAGE_SIZE);
        }

        update_page(page, page_id, frame_id);

        // 标记 I/O 进行中，立即 pin 防淘汰
        page->io_pending_.store(true);
        replacer_->pin(frame_id);
        page->pin_count_ = 1;
    }

    // 阶段2：无锁——执行磁盘 I/O
    if (old_page_dirty) {
        disk_manager_->write_page(old_page_id.fd, old_page_id.page_no,
                                  old_data_copy, PAGE_SIZE);
    }
    // 读到栈缓冲区而非 page->data_，避免无锁写入被其他线程读到半完成数据
    char new_data[PAGE_SIZE];
    disk_manager_->read_page(page_id.fd, page_id.page_no, new_data, PAGE_SIZE);

    // 阶段3：写锁——复制数据、清除 io_pending_、通知等待线程
    {
        std::unique_lock lock{latch_};
        Page* page = &pages_[frame_id];
        std::memcpy(page->data_, new_data, PAGE_SIZE);
        page->io_pending_.store(false);
        page->io_cv_.notify_all();
    }

    return &pages_[frame_id];
}

/**
 * @description: 取消固定一个页面（减少 pin_count）。当 pin_count 降为 0 时，
 *              页面变为可淘汰状态，加入替换策略的候选列表。
 *              直接使用 unique_lock，不做读锁快速路径（负优化）。
 * @return {bool} true: 成功取消固定; false: 页面不存在或 pin_count 已为 0
 * @param {PageId} page_id 目标页面的 PageId
 * @param {bool} is_dirty 是否将页面标记为脏页
 */
bool BufferPoolManager::unpin_page(PageId page_id, bool is_dirty) {
    std::unique_lock lock{latch_};
    auto it = page_table_.find(page_id);
    if (it == page_table_.end()) {
        return false;
    }
    frame_id_t frame_id = it->second;
    Page* page = &pages_[frame_id];
    if (page->pin_count_ == 0)
        return false;
    page->pin_count_--;
    if (page->pin_count_ == 0)
        replacer_->unpin(frame_id);
    if (is_dirty) {
        page->is_dirty_ = true;
    }
    return true;
}

/**
 * @description: 将指定页面写回磁盘（无论是否为脏页都写入）。
 *              三阶段锁策略：读锁复制 → 无锁写磁盘 → 写锁清脏标记。
 *              每个阶段都验证映射关系，防止释放锁期间页面被替换。
 * @return {bool} true: 成功写回; false: 页面不在缓冲池中或 page_id 无效
 * @param {PageId} page_id 目标页面的 PageId
 */
bool BufferPoolManager::flush_page(PageId page_id) {
    char data_copy[PAGE_SIZE];
    frame_id_t flush_frame_id;

    // 阶段1：读锁——查找页面、等待 I/O、复制数据
    {
        std::shared_lock lock{latch_};
        auto it = page_table_.find(page_id);
        if (it == page_table_.end() || page_id.page_no == INVALID_PAGE_ID) {
            return false;
        }
        flush_frame_id = it->second;
        Page* page = &pages_[flush_frame_id];
        if (!wait_io_and_verify(lock, page, page_id, flush_frame_id)) {
            return false;
        }
        std::memcpy(data_copy, page->data_, PAGE_SIZE);
    }

    // 阶段2：无锁——执行磁盘写入
    disk_manager_->write_page(page_id.fd, page_id.page_no, data_copy,
                              PAGE_SIZE);

    // 阶段3：写锁——验证映射后清除脏标记
    {
        std::unique_lock lock{latch_};
        auto it = page_table_.find(page_id);
        if (it != page_table_.end() && it->second == flush_frame_id) {
            pages_[flush_frame_id].is_dirty_ = false;
        }
    }
    return true;
}

/**
 * @description: 创建一个新页面。从缓冲池中分配一个可用帧，
 *              在磁盘上分配新的 page_no，将帧与新页面关联。
 *              释放锁后写回旧脏页，避免持锁期间做 I/O。
 * @return {Page*} 成功返回新页面指针，缓冲池满且无可淘汰页面时返回 nullptr
 * @param {PageId*} page_id 输入输出参数，fd 由调用者设置，page_no 由本函数分配
 */
Page* BufferPoolManager::new_page(PageId* page_id) {
    char old_data_copy[PAGE_SIZE];
    PageId old_page_id;
    bool old_page_dirty = false;
    frame_id_t victim_fid;
    Page* victim_page;

    {
        std::unique_lock lock{latch_};

        if (!find_victim_page(&victim_fid)) {
            return nullptr;
        }

        victim_page = &pages_[victim_fid];

        // 保存旧页面信息，必须在 update_page 之前
        old_page_id = victim_page->get_page_id();
        old_page_dirty = victim_page->is_dirty_ && old_page_id.page_no != INVALID_PAGE_ID;
        if (old_page_dirty) {
            std::memcpy(old_data_copy, victim_page->data_, PAGE_SIZE);
        }

        page_id->page_no = disk_manager_->allocate_page(page_id->fd);

        update_page(victim_page, *page_id, victim_fid);

        replacer_->pin(victim_fid);
        victim_page->pin_count_ = 1;
    }

    // 释放锁后写旧脏页
    if (old_page_dirty) {
        disk_manager_->write_page(old_page_id.fd, old_page_id.page_no,
                                  old_data_copy, PAGE_SIZE);
    }

    return victim_page;
}

/**
 * @description: 从缓冲池删除指定页面。如果页面正在被使用（pin_count > 0）则无法删除。
 *              删除后帧归还到空闲列表。释放锁后写回脏页。
 * @return {bool} true: 页面不存在或成功删除; false: 页面正在被使用
 * @param {PageId} page_id 目标页面的 PageId
 */
bool BufferPoolManager::delete_page(PageId page_id) {
    bool was_dirty = false;
    char data_copy[PAGE_SIZE];

    {
        std::unique_lock lock{latch_};

        auto it = page_table_.find(page_id);
        if (it == page_table_.end()) {
            return true;
        }

        frame_id_t frame_id = it->second;
        Page* page = &pages_[frame_id];
        if (page->pin_count_ != 0) {
            return false;
        }

        if (page->is_dirty_) {
            was_dirty = true;
            std::memcpy(data_copy, page->data_, PAGE_SIZE);
        }

        page_table_.erase(it);

        page->reset_memory();
        page->id_.fd = -1;
        page->id_.page_no = INVALID_PAGE_ID;
        page->is_dirty_ = false;
        page->pin_count_ = 0;

        replacer_->pin(frame_id);

        free_list_.push_back(frame_id);
    }

    if (was_dirty) {
        disk_manager_->write_page(page_id.fd, page_id.page_no, data_copy,
                                  PAGE_SIZE);
    }

    return true;
}

/**
 * @description: 将指定文件的所有页面写回磁盘。
 *              三阶段锁策略逐页刷新，避免在遍历 65536 个页框和磁盘 I/O 期间持锁。
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

        // 2.1 读锁：验证映射、等待 I/O、复制数据
        {
            std::shared_lock lock{latch_};
            auto it = page_table_.find(pid);
            if (it != page_table_.end() && it->second == fid) {
                Page* page = &pages_[fid];
                if (!wait_io_and_verify(lock, page, pid, fid)) {
                    continue;
                }
                std::memcpy(data_copy, page->data_, PAGE_SIZE);
                need_flush = true;
            }
        }

        // 2.2 无锁：写磁盘
        if (need_flush) {
            disk_manager_->write_page(pid.fd, pid.page_no, data_copy,
                                      PAGE_SIZE);

            // 2.3 写锁：验证映射后清脏标记
            std::unique_lock lock{latch_};
            auto it = page_table_.find(pid);
            if (it != page_table_.end() && it->second == fid) {
                pages_[fid].is_dirty_ = false;
            }
        }
    }
}

std::unique_ptr<Replacer> BufferPoolManager::create_replacer(size_t pool_size) {
        if (REPLACER_TYPE == "CLOCK")
            return std::make_unique<ClockReplacer>(pool_size);
        return std::make_unique<LRUReplacer>(pool_size);
}

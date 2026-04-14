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
    // 移除旧页面ID的映射
    page_table_.erase(page->get_page_id());
    // 建立新页面ID到帧编号的映射
    page_table_[new_page_id] = new_frame_id;
    // 重置页面数据（清零），设置新的页面ID
    page->reset_memory();
    page->id_ = new_page_id;
}

/**
 * @description: 从缓冲池获取指定页面。如果页面已在缓冲池中则直接返回（页面命中），
 *              否则从磁盘加载到缓冲池（页面未命中）。
 *              使用 reserve-and-load 模式，将磁盘 I/O 移到锁外。
 *              关键安全约束：io_cv_.wait() 释放锁期间页面可能被淘汰替换，
 *              唤醒后必须重新验证 page_table_ 映射关系，否则会操作错误的页面。
 *
 *              执行流程：
 *              ┌─ 第一步：shared_lock 查找 page_table_（高频路径）
 *              │  ├─ 命中且无 io_pending_：pin + pin_count++ → 返回
 *              │  ├─ 命中但有 io_pending_：wait → 验证映射 → pin + pin_count++ → 返回
 *              │  └─ wait 后映射已变：放弃读锁，进入写锁路径
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
    // 第一步：读锁检查页面是否已在缓冲池中（高频路径，允许多线程并发读）
    {
        std::shared_lock lock{latch_};
        auto it = page_table_.find(page_id);
        if (it != page_table_.end()) {
            frame_id_t frame_id = it->second;
            Page* page = &pages_[frame_id];

            // 页面正在从磁盘加载中，需要等待 I/O 完成
            if (page->io_pending_.load()) {
                // wait 会临时释放 shared_lock，I/O 完成后重新获取
                page->io_cv_.wait(lock, [&] { return !page->io_pending_.load(); });

                // 【关键】wait 释放锁期间页面可能被淘汰替换，必须重新验证映射
                // 场景：Thread A wait → Thread B 完成 I/O → Thread C 淘汰该帧
                //       → Thread D 加载新页面到该帧 → Thread A 唤醒
                //       此时 frame_id 对应的已经是完全不同的页面
                it = page_table_.find(page_id);
                if (it == page_table_.end() || it->second != frame_id) {
                    // 页面已被替换，放弃读锁，走写锁路径重新加载
                } else {
                    // 映射关系未变，安全地 pin 并返回
                    replacer_->pin(frame_id);
                    page->pin_count_++;
                    return page;
                }
            } else {
                // 页面已加载完成且映射有效，直接 pin 并返回
                replacer_->pin(frame_id);
                page->pin_count_++;
                return page;
            }
        }
        // 页面未命中，读锁自动释放
    }

    // 以下变量在阶段2（无锁）中使用，需要在写锁内提前保存
    char old_data_copy[PAGE_SIZE];   // 旧脏页数据副本
    PageId old_page_id;              // 旧页面的 PageId
    bool old_page_dirty = false;     // 旧页面是否为脏页
    frame_id_t frame_id;             // 预约的帧编号

    // 阶段1：写锁——预约帧、修改元数据、标记 I/O 进行中
    {
        std::unique_lock lock{latch_};

        // 双重检查：获取写锁后再次查找
        // 防止其他线程在等待写锁期间已加载该页面
        auto it = page_table_.find(page_id);
        if (it != page_table_.end()) {
            frame_id_t fid = it->second;
            Page* page = &pages_[fid];
            // 同样需要处理 io_pending_ 和映射验证
            if (page->io_pending_.load()) {
                page->io_cv_.wait(lock, [&] { return !page->io_pending_.load(); });
                // wait 释放锁期间页面可能被淘汰替换，重新验证
                it = page_table_.find(page_id);
                if (it == page_table_.end() || it->second != fid) {
                    // 页面已被替换，继续走未命中路径（找帧、加载）
                } else {
                    // 映射关系未变，安全地 pin 并返回
                    replacer_->pin(fid);
                    page->pin_count_++;
                    return page;
                }
            } else {
                // 其他线程已加载该页面，直接返回
                replacer_->pin(fid);
                page->pin_count_++;
                return page;
            }
        }

        // 确认页面不在缓冲池中，找一个可用帧
        if (!find_victim_page(&frame_id)) {
            return nullptr;
        }

        Page* page = &pages_[frame_id];

        // 保存旧页面信息，用于释放锁后写回脏页
        // 必须在 update_page 之前保存，因为 update_page 会重置页面数据
        old_page_id = page->get_page_id();
        old_page_dirty = page->is_dirty_ && old_page_id.page_no != INVALID_PAGE_ID;
        if (old_page_dirty) {
            std::memcpy(old_data_copy, page->data_, PAGE_SIZE);
        }

        // 更新页表映射和页面元数据（不执行磁盘 I/O）
        update_page(page, page_id, frame_id);

        // 标记页面为 I/O 进行中，其他线程遇到此页面会通过条件变量等待
        page->io_pending_.store(true);

        // 立即 pin 并设置 pin_count_=1，防止其他线程淘汰此帧
        // 即使页面数据还没从磁盘加载，帧已被"预约"给这个页面
        replacer_->pin(frame_id);
        page->pin_count_ = 1;

        // 写锁释放，其他线程可以继续操作缓冲池
    }

    // 阶段2：无锁——执行磁盘 I/O（慢操作，不阻塞其他线程）
    // 2.1 如果被淘汰的旧页面是脏页，写回磁盘
    if (old_page_dirty) {
        disk_manager_->write_page(old_page_id.fd, old_page_id.page_no,
                                  old_data_copy, PAGE_SIZE);
    }
    // 2.2 从磁盘读取新页面数据到栈缓冲区（而非直接读到 page->data_）
    // 原因：page->data_ 可能被其他线程访问（通过 shared_lock），
    // 直接写入会导致数据不一致；使用栈缓冲区 + 阶段3 的写锁复制更安全
    char new_data[PAGE_SIZE];
    disk_manager_->read_page(page_id.fd, page_id.page_no, new_data, PAGE_SIZE);

    // 阶段3：写锁——复制数据、清除 io_pending_、通知等待线程
    {
        std::unique_lock lock{latch_};
        Page* page = &pages_[frame_id];
        // 将栈缓冲区的数据复制到页面的 data_ 中
        std::memcpy(page->data_, new_data, PAGE_SIZE);
        // 清除 I/O 进行中标志
        page->io_pending_.store(false);
        // 通知所有等待此页面的线程：I/O 已完成，可以安全访问
        page->io_cv_.notify_all();
    }

    return &pages_[frame_id];
}

/**
 * @description: 取消固定一个页面（减少 pin_count）。当 pin_count 降为 0 时，
 *              页面变为可淘汰状态，加入替换策略的候选列表。
 *              直接使用 unique_lock，不做读锁快速路径（负优化）：
 *              读锁路径只省了一个 unique_lock 的获取（页面不存在时），
 *              但页面存在时反而多了一次 find + 两次锁获取。
 * @return {bool} true: 成功取消固定; false: 页面不存在或 pin_count 已为 0
 * @param {PageId} page_id 目标页面的 PageId
 * @param {bool} is_dirty 是否将页面标记为脏页
 */
bool BufferPoolManager::unpin_page(PageId page_id, bool is_dirty) {
    std::unique_lock lock{latch_};

    // 查找页面，不存在则返回 false
    auto it = page_table_.find(page_id);
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
 *
 *              阶段1（shared_lock）：查找页面 → 等待 io_pending_ → 复制数据到栈缓冲区
 *              阶段2（无锁）：执行磁盘写入
 *              阶段3（unique_lock）：验证映射关系未变 → 清除脏标记
 *
 *              阶段3 必须验证映射关系：释放锁期间其他线程可能淘汰了该页面，
 *              导致 page_table_[page_id] 不再指向原来的帧。
 *
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
        // 页面不在缓冲池中或 page_id 无效，直接返回
        if (it == page_table_.end() || page_id.page_no == INVALID_PAGE_ID) {
            return false;
        }
        flush_frame_id = it->second;
        Page* page = &pages_[flush_frame_id];

        // 等待 I/O 完成后再复制数据，确保数据一致性
        if (page->io_pending_.load()) {
            page->io_cv_.wait(lock, [&] { return !page->io_pending_.load(); });
            // wait 释放锁期间页面可能被替换，重新验证映射
            it = page_table_.find(page_id);
            if (it == page_table_.end() || it->second != flush_frame_id) {
                // 页面已被替换，无法安全刷新，返回 false
                return false;
            }
        }

        // 复制页面数据到栈缓冲区，释放锁后仍可安全使用
        std::memcpy(data_copy, page->data_, PAGE_SIZE);
    }

    // 阶段2：无锁——执行磁盘写入（慢操作，不阻塞其他线程）
    disk_manager_->write_page(page_id.fd, page_id.page_no, data_copy,
                              PAGE_SIZE);

    // 阶段3：写锁——验证映射关系未变后清除脏标记
    {
        std::unique_lock lock{latch_};
        auto it = page_table_.find(page_id);
        // 验证映射关系：页面可能已被淘汰替换到其他帧
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
 *              释放锁后写回旧脏页，避免持锁期间做 I/O。
 *
 *              执行流程：
 *              1. 写锁内：找帧 → 保存旧脏页数据 → allocate_page → update_page → pin
 *              2. 无锁：写旧脏页到磁盘
 *
 *              注意：allocate_page 在写锁内调用，因为它很快（只操作 set/atomic），
 *              且需要 page_no 才能更新页表。如果移到锁外需要更复杂的状态管理。
 *
 * @return {Page*} 成功返回新页面指针，缓冲池满且无可淘汰页面时返回 nullptr
 * @param {PageId*} page_id 输入输出参数，fd 由调用者设置，page_no 由本函数分配
 */
Page* BufferPoolManager::new_page(PageId* page_id) {
    // 以下变量在释放锁后使用，需要在写锁内提前保存
    char old_data_copy[PAGE_SIZE];   // 旧脏页数据副本
    PageId old_page_id;              // 旧页面的 PageId
    bool old_page_dirty = false;     // 旧页面是否为脏页
    frame_id_t victim_fid;           // 被淘汰的帧编号
    Page* victim_page;               // 被淘汰帧中的页面指针

    {
        std::unique_lock lock{latch_};

        // 步骤1：找一个可用帧（优先空闲列表，其次淘汰）
        if (!find_victim_page(&victim_fid)) {
            return nullptr;
        }

        victim_page = &pages_[victim_fid];

        // 步骤2：保存旧页面信息，用于释放锁后写回脏页
        // 必须在 update_page 之前保存，因为 update_page 会重置页面数据
        old_page_id = victim_page->get_page_id();
        old_page_dirty = victim_page->is_dirty_ && old_page_id.page_no != INVALID_PAGE_ID;
        if (old_page_dirty) {
            std::memcpy(old_data_copy, victim_page->data_, PAGE_SIZE);
        }

        // 步骤3：在磁盘上分配新的 page_no
        page_id->page_no = disk_manager_->allocate_page(page_id->fd);

        // 步骤4：更新页表映射和页面元数据（不执行磁盘 I/O）
        update_page(victim_page, *page_id, victim_fid);

        // 步骤5：标记帧为正在使用，防止被淘汰
        replacer_->pin(victim_fid);
        victim_page->pin_count_ = 1;
    }

    // 步骤6：释放锁后，将旧脏页写回磁盘（不阻塞其他线程）
    if (old_page_dirty) {
        disk_manager_->write_page(old_page_id.fd, old_page_id.page_no,
                                  old_data_copy, PAGE_SIZE);
    }

    return victim_page;
}

/**
 * @description: 从缓冲池删除指定页面。如果页面正在被使用（pin_count > 0）则无法删除。
 *              删除后帧归还到空闲列表。如果是脏页，先复制数据再释放锁后写回磁盘，
 *              避免持锁期间进行磁盘 I/O。
 *
 *              执行流程：
 *              1. 写锁内：查找页面 → 检查 pin_count → 复制脏页数据
 *                        → 从页表移除 → 重置页面元数据 → 归还帧到 free_list
 *              2. 无锁：写脏页到磁盘
 *
 *              注意：脏页数据在写锁内复制到栈缓冲区，释放锁后才写磁盘。
 *              此时页面已从缓冲池移除，其他线程不会访问到该帧，
 *              所以不存在数据竞争问题。
 *
 * @return {bool} true: 页面不存在或成功删除; false: 页面正在被使用（pin_count > 0）
 * @param {PageId} page_id 目标页面的 PageId
 */
bool BufferPoolManager::delete_page(PageId page_id) {
    bool was_dirty = false;
    char data_copy[PAGE_SIZE];

    {
        std::unique_lock lock{latch_};

        // 步骤1：查找页面，不存在则直接返回 true（幂等）
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

        // 步骤3：如果是脏页，复制数据到栈缓冲区（释放锁后写回磁盘）
        if (page->is_dirty_) {
            was_dirty = true;
            std::memcpy(data_copy, page->data_, PAGE_SIZE);
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
 *
 *              阶段1（shared_lock）：遍历所有页框，收集目标文件的页面列表
 *              阶段2：逐页执行三阶段刷新：
 *                2.1（shared_lock）：验证页面仍在原帧中 → 等待 io_pending_ → 复制数据
 *                2.2（无锁）：执行磁盘写入
 *                2.3（unique_lock）：验证映射关系未变 → 清除脏标记
 *
 *              为什么不一次性持锁遍历+刷新？
 *              缓冲池有 65536 个页框，逐帧检查 + 磁盘 I/O 可能需要数百毫秒，
 *              期间所有其他线程都被阻塞。逐页刷新允许其他线程在页面间插入操作。
 *
 * @param {int} fd 目标文件的文件描述符
 */
void BufferPoolManager::flush_all_pages(int fd) {
    // 阶段1：读锁收集目标文件的所有页面信息
    std::vector<std::pair<PageId, frame_id_t>> pages_to_flush;
    {
        std::shared_lock lock{latch_};
        for (size_t i = 0; i < pool_size_; i++) {
            Page* page = &pages_[i];
            // 只收集属于目标文件且有效的页面
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

        // 2.1 读锁：验证页面仍在原帧中，等待 I/O 完成后复制数据
        {
            std::shared_lock lock{latch_};
            auto it = page_table_.find(pid);
            // 验证映射关系：页面可能已被淘汰替换到其他帧
            if (it != page_table_.end() && it->second == fid) {
                Page* page = &pages_[fid];
                // 等待 I/O 完成，确保数据一致性
                if (page->io_pending_.load()) {
                    page->io_cv_.wait(lock, [&] { return !page->io_pending_.load(); });
                    // wait 释放锁期间页面可能被替换，重新验证
                    it = page_table_.find(pid);
                    if (it == page_table_.end() || it->second != fid) {
                        // 页面已被替换，跳过此页
                        continue;
                    }
                }
                // 复制页面数据到栈缓冲区
                std::memcpy(data_copy, page->data_, PAGE_SIZE);
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
 * @note 使用参数而非成员变量 pool_size_，因为构造函数中 pool_size_ 可能尚未初始化
 */
std::unique_ptr<Replacer> BufferPoolManager::create_replacer(size_t pool_size) {
        if (REPLACER_TYPE == "CLOCK")
            return std::make_unique<ClockReplacer>(pool_size);
        return std::make_unique<LRUReplacer>(pool_size);
}

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

bool BufferPoolManager::find_victim_page(frame_id_t* frame_id) {
    if (free_list_.empty())
        return replacer_->victim(frame_id);
    else {
        *frame_id = free_list_.front();
        free_list_.pop_front();
        return true;
    }
}

void BufferPoolManager::update_page(Page* page,
                                    PageId new_page_id,
                                    frame_id_t new_frame_id) {
    if (page->is_dirty() && page->get_page_id().page_no != INVALID_PAGE_ID) {
        disk_manager_->write_page(page->get_page_id().fd,
                                  page->get_page_id().page_no, page->get_data(),
                                  PAGE_SIZE);
        page->is_dirty_ = false;
    }
    page_table_.erase(page->get_page_id());
    page_table_[new_page_id] = new_frame_id;
    page->reset_memory();
    page->id_ = new_page_id;
}

/**
 * @description: 从buffer pool获取需要的页。
 *              使用读写锁 + 双重检查优化并发性能：
 *              - 页面命中（高频）：读锁，允许多线程并发读
 *              - 页面未命中（低频）：写锁，独占修改数据结构
 *              - 双重检查：获取写锁后再次检查页面是否已被其他线程加载
 */
Page* BufferPoolManager::fetch_page(PageId page_id) {
    {
        std::shared_lock lock{latch_};
        auto it = page_table_.find(page_id);
        if (it != page_table_.end()) {
            frame_id_t frame_id = it->second;
            replacer_->pin(frame_id);
            Page* page = &pages_[frame_id];
            page->pin_count_++;
            return page;
        }
    }

    {
        std::unique_lock lock{latch_};

        auto it = page_table_.find(page_id);
        if (it != page_table_.end()) {
            frame_id_t frame_id = it->second;
            replacer_->pin(frame_id);
            Page* page = &pages_[frame_id];
            page->pin_count_++;
            return page;
        }

        frame_id_t frame_id;
        if (!find_victim_page(&frame_id)) {
            return nullptr;
        }

        Page* page = &pages_[frame_id];
        update_page(page, page_id, frame_id);
        disk_manager_->read_page(page_id.fd, page_id.page_no, page->data_,
                                 PAGE_SIZE);
        replacer_->pin(frame_id);
        page->pin_count_ = 1;

        return page;
    }
}

/**
 * @description: 取消固定pin_count>0的在缓冲池中的page
 *              读锁快速路径：页面不存在时快速返回，允许并发检查
 */
bool BufferPoolManager::unpin_page(PageId page_id, bool is_dirty) {
    {
        std::shared_lock lock{latch_};
        if (page_table_.find(page_id) == page_table_.end()) {
            return false;
        }
    }

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
 * @description: 将目标页写回磁盘
 *              三阶段锁策略：读锁复制数据 → 释放锁写磁盘 → 写锁更新元数据
 *              避免持锁期间进行磁盘I/O，显著减少并发冲突
 */
bool BufferPoolManager::flush_page(PageId page_id) {
    char data_copy[PAGE_SIZE];
    frame_id_t flush_frame_id;
    {
        std::shared_lock lock{latch_};
        auto it = page_table_.find(page_id);
        if (it == page_table_.end() || page_id.page_no == INVALID_PAGE_ID) {
            return false;
        }
        flush_frame_id = it->second;
        std::memcpy(data_copy, pages_[flush_frame_id].data_, PAGE_SIZE);
    }

    disk_manager_->write_page(page_id.fd, page_id.page_no, data_copy,
                              PAGE_SIZE);

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
 * @description: 创建一个新的page
 *              需要修改多个数据结构，全程使用写锁
 */
Page* BufferPoolManager::new_page(PageId* page_id) {
    std::unique_lock lock{latch_};
    frame_id_t victim_fid;
    if (!find_victim_page(&victim_fid)) {
        return nullptr;
    }
    page_id->page_no = disk_manager_->allocate_page(page_id->fd);
    Page* victim_page = &pages_[victim_fid];
    update_page(victim_page, *page_id, victim_fid);
    replacer_->pin(victim_fid);
    victim_page->pin_count_ = 1;
    return victim_page;
}

/**
 * @description: 从buffer_pool删除目标页
 *              修改数据结构后释放锁，再进行磁盘I/O
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

        if (page->is_dirty()) {
            was_dirty = true;
            std::memcpy(data_copy, page->get_data(), PAGE_SIZE);
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
 * @description: 将buffer_pool中的所有页写回到磁盘
 *              三阶段锁策略：读锁收集页面 → 逐页复制+写磁盘 → 写锁清除脏标记
 *              避免在遍历65536个页框和磁盘I/O期间持锁
 */
void BufferPoolManager::flush_all_pages(int fd) {
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

    for (auto& [pid, fid] : pages_to_flush) {
        char data_copy[PAGE_SIZE];
        bool need_flush = false;
        {
            std::shared_lock lock{latch_};
            auto it = page_table_.find(pid);
            if (it != page_table_.end() && it->second == fid) {
                std::memcpy(data_copy, pages_[fid].data_, PAGE_SIZE);
                need_flush = true;
            }
        }

        if (need_flush) {
            disk_manager_->write_page(pid.fd, pid.page_no, data_copy,
                                      PAGE_SIZE);
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

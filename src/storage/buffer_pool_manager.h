/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#pragma once
#include <fcntl.h>
#include <unistd.h>

#include <cassert>
#include <cstring>
#include <list>
#include <shared_mutex>
#include <unordered_map>
#include <vector>

#include "disk_manager.h"
#include "errors.h"
#include "page.h"
#include "replacer/clock_replacer.h"
#include "replacer/lru_replacer.h"
#include "replacer/replacer.h"

class BufferPoolManager {
   private:
   std::unordered_map<PageId, frame_id_t, PageIdHash> page_table_;  // 帧号和页面号的映射哈希表，用于根据页面的PageId定位该页面的帧编号
   std::list<frame_id_t> free_list_;  // 空闲帧编号的链表
   DiskManager* disk_manager_;
   std::unique_ptr<Replacer> replacer_;  // buffer_pool的置换策略，当前赛题中为LRU置换策略
   std::unique_ptr<Page[]> pages_;  // buffer_pool中的Page对象数组，在构造空间中申请内存空间，在析构函数中释放，大小为BUFFER_POOL_SIZE
   std::shared_mutex latch_;  // 读写锁，读多写少场景下减少并发冲突
   size_t pool_size_;  // buffer_pool中可容纳页面的个数，即帧的个数

   public:
    BufferPoolManager(size_t pool_size, DiskManager* disk_manager)
        : disk_manager_(disk_manager)
        , replacer_(create_replacer(pool_size))
        , pages_(new Page[pool_size])
        , pool_size_(pool_size) {
        
        // 初始化时，所有的page都在free_list_中
        for (size_t i = 0; i < pool_size_; ++i) {
            free_list_.emplace_back(static_cast<frame_id_t>(i));  // static_cast转换数据类型
        }
    }

    ~BufferPoolManager() = default;

    /**
     * @description: 将目标页面标记为脏页
     * @param {Page*} page 脏页
     */
    static void mark_dirty(Page* page) { page->is_dirty_ = true; }

   public:
    Page* fetch_page(PageId page_id);

    bool unpin_page(PageId page_id, bool is_dirty);

    bool flush_page(PageId page_id);

    Page* new_page(PageId* page_id);

    bool delete_page(PageId page_id);

    void flush_all_pages(int fd);

   private:
    bool find_victim_page(frame_id_t* frame_id);

    void update_page(Page* page, PageId new_page_id, frame_id_t new_frame_id);

    template<typename Lock>
    bool wait_io_and_verify(Lock& lock, Page* page, PageId page_id, frame_id_t frame_id);

    std::unique_ptr<Replacer> create_replacer(size_t pool_size);
};

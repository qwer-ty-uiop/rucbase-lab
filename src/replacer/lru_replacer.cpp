/* Copyright (c) 2023 Renmin University of China
RMDB is licensed under Mulan PSL v2.
You can use this software according to the terms and conditions of the Mulan PSL
v2. You may obtain a copy of Mulan PSL v2 at:
        http://license.coscl.org.cn/MulanPSL2
THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
See the Mulan PSL v2 for more details. */

#include "lru_replacer.h"

LRUReplacer::LRUReplacer(size_t num_pages) : max_size_(num_pages) {}

LRUReplacer::~LRUReplacer() = default;

/**
 * @description: 淘汰一个页面，并返回该frame的id
 * @param {frame_id_t*} frame_id 被移除的frame的id，如果没有frame被移除返回nullptr
 * @return {bool} 如果成功淘汰了一个页面则返回true，否则返回false
 * 
 * 【数据库LRU vs 传统LRU的区别】
 * 
 * 传统LRU实现：
 *   - 维护所有内存页面
 *   - 每次访问都移动到链表头部
 *   - 淘汰时选择链表尾部
 * 
 * 数据库LRU实现（本实现）：
 *   - 只维护"可以被淘汰"的页面（pin_count=0的页面）
 *   - 页面被访问时从列表移除，而不是移动到头部
 *   - 页面使用完毕才加入列表
 *   - 淘汰时选择链表尾部（最久unpin的页面）
 * 
 * 为什么这样设计？
 *   1. 减少链表操作：传统LRU每次访问都要移动，数据库LRU只在pin/unpin时操作
 *   2. 减少锁竞争：操作次数少，锁竞争少
 *   3. 语义清晰：正在使用的页面不应该被淘汰
 */
bool LRUReplacer::victim(frame_id_t* frame_id) {
    std::scoped_lock lock{latch_};
    
    if (LRUlist_.empty()) {
        return false;
    }
    
    // 选择最近最少使用的页面（list尾部）作为淘汰页面
    // 尾部 = 最久没有被unpin的页面 = 最久没有被使用的页面
    *frame_id = LRUlist_.back();
    LRUlist_.pop_back();
    LRUhash_.erase(*frame_id);
    
    return true;
}

/**
 * @description: 固定指定的frame，即该页面无法被淘汰
 * @param {frame_id_t} frame_id 需要固定的frame的id
 * 
 * 【数据库LRU vs 传统LRU的区别】
 * 
 * 传统LRU：
 *   - 没有pin操作的概念
 *   - 所有页面都可以被淘汰
 * 
 * 数据库LRU（本实现）：
 *   - pin表示页面正在被使用
 *   - 从LRU列表中移除该页面
 *   - 该页面不能被淘汰，直到被unpin
 * 
 * 调用时机：fetch_page() 或 new_page() 时
 */
void LRUReplacer::pin(frame_id_t frame_id) {
    std::scoped_lock lock{latch_};
    
    // 在hash中查找该frame
    auto it = LRUhash_.find(frame_id);
    
    // 如果找到，从LRU列表中移除（正在使用，不能被淘汰）
    // 【关键区别】传统LRU会移动到头部，数据库LRU直接移除
    if (it != LRUhash_.end()) {
        LRUlist_.erase(it->second);
        LRUhash_.erase(frame_id);
    }
}

/**
 * @description: 取消固定一个frame，代表该页面可以被淘汰
 * @param {frame_id_t} frame_id 取消固定的frame的id
 * 
 * 【数据库LRU vs 传统LRU的区别】
 * 
 * 传统LRU：
 *   - 没有unpin操作的概念
 *   - 页面一直在列表中
 * 
 * 数据库LRU（本实现）：
 *   - unpin表示页面使用完毕
 *   - 将页面加入LRU列表头部
 *   - 头部 = 最近使用完毕 = 最不可能被淘汰
 * 
 * 调用时机：unpin_page() 且 pin_count 变为 0 时
 */
void LRUReplacer::unpin(frame_id_t frame_id) {
    std::scoped_lock lock{latch_};
    
    // 如果该frame不在LRU列表中，才加入
    // 【关键区别】传统LRU是移动到头部，数据库LRU是加入头部
    if (LRUhash_.find(frame_id) == LRUhash_.end()) {
        LRUlist_.push_front(frame_id);  // 加入头部（最近使用，最不可能被淘汰）
        LRUhash_[frame_id] = LRUlist_.begin();
    }
}

/**
 * @description: 获取当前replacer中可以被淘汰的页面数量
 * 
 * 【注意】这里返回的是"可以被淘汰"的页面数量
 * 而不是缓冲池中的总页面数
 */
size_t LRUReplacer::Size() {
    return LRUlist_.size();
}

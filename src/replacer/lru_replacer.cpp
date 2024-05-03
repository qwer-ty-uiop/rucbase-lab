#include "lru_replacer.h"

LRUReplacer::LRUReplacer(size_t num_pages) {
    max_size_ = num_pages;
}

LRUReplacer::~LRUReplacer() = default;

/**
 * @brief 使用LRU策略删除一个victim frame，这个函数能得到frame_id
 * @param[out] frame_id id of frame that was removed, nullptr if no victim was
 * found
 * @return true if a victim frame was found, false otherwise
 */
bool LRUReplacer::Victim(frame_id_t* frame_id) {
    // C++17 std::scoped_lock
    // 它能够避免死锁发生，其构造函数能够自动进行上锁操作，析构函数会对互斥量进行解锁操作，保证线程安全。
    std::scoped_lock lock{latch_};

    // Todo:
    //  利用lru_replacer中的LRUlist_,LRUHash_实现LRU策略
    //  选择合适的frame指定为淘汰页面,赋值给*frame_id
    // 映射为空
    if (LRUhash_.empty())
        return false;
    // 尾部的是最早访问的
    *frame_id = LRUlist_.back();
    // 删除映射和页(unpined)
    LRUlist_.pop_back();
    LRUhash_.erase(*frame_id);
    return true;
}

/**
 * @brief 固定一个frame, 表明它不应该成为victim（即在replacer中移除该frame_id）
 * @param frame_id the id of the frame to pin
 */
void LRUReplacer::Pin(frame_id_t frame_id) {
    std::scoped_lock lock{latch_};
    // Todo:
    // 固定指定id的frame
    // 在数据结构中移除该frame
    // 在unlock中能找到frame_id则将他移除，不让他被victim分配
    if (LRUhash_.find(frame_id) != LRUhash_.end()) {
        LRUlist_.erase(LRUhash_[frame_id]);
        LRUhash_.erase(frame_id);
    }
}

/**
 * 取消固定一个frame, 表明它可以成为victim（即将该frame_id添加到replacer）
 * @param frame_id the id of the frame to unpin
 */
void LRUReplacer::Unpin(frame_id_t frame_id) {
    // Todo:
    //  支持并发锁
    //  选择一个frame取消固定
    std::scoped_lock lock{latch_};
    auto it = LRUhash_.find(frame_id);
    if (it == LRUhash_.end()) {
        while (LRUhash_.size() >= max_size_) {  // 缓存不够用了
            frame_id_t delete_id = LRUlist_.back();
            LRUlist_.pop_back();  // 丢弃最早弃用的unpined_id
            LRUhash_.erase(delete_id);
        }
        LRUlist_.push_front(frame_id);  // 新加入的unpined_id放在前面
        LRUhash_[frame_id] = LRUlist_.begin();  // 更新hash
    }
}

/** @return replacer中能够victim的数量 */
size_t LRUReplacer::Size() {
    // Todo:
    // 改写return size
    return LRUhash_.size();
}

#include "replacer/clock_replacer.h"

#include <algorithm>

ClockReplacer::ClockReplacer(size_t num_pages)
    : circular_{num_pages, ClockReplacer::Status::EMPTY_OR_PINNED}, hand_{0}, capacity_{num_pages} {
    // 成员初始化列表语法
}

ClockReplacer::~ClockReplacer() = default;

bool ClockReplacer::Victim(frame_id_t *frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);
    
    // 遍历所有帧，寻找可淘汰的页面
    size_t scanned = 0;
    while (scanned < capacity_) {
        // 检查当前hand_位置的帧
        if (circular_[hand_] == Status::UNTOUCHED) {
            // 找到可淘汰的帧
            *frame_id = hand_;
            // 将该位置标记为EMPTY
            circular_[hand_] = Status::EMPTY_OR_PINNED;
            // 移动hand_到下一个位置
            hand_ = (hand_ + 1) % capacity_;
            return true;
        } else if (circular_[hand_] == Status::ACCESSED) {
            // 给第二次机会，标记为UNTOUCHED，下次可能被淘汰
            circular_[hand_] = Status::UNTOUCHED;
        }
        // 如果是EMPTY_OR_PINNED或已处理过ACCESSED，继续下一个
        hand_ = (hand_ + 1) % capacity_;
        scanned++;
    }
    
    // 没有找到可淘汰的帧
    return false;
}

void ClockReplacer::Pin(frame_id_t frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);
    // 检查frame_id是否在有效范围内
    if (frame_id < 0 || static_cast<size_t>(frame_id) >= capacity_) {
        return;
    }
    // 固定该帧，标记为EMPTY_OR_PINNED（表示不可淘汰）
    circular_[frame_id] = Status::EMPTY_OR_PINNED;
}

void ClockReplacer::Unpin(frame_id_t frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);
    // 检查frame_id是否在有效范围内
    if (frame_id < 0 || static_cast<size_t>(frame_id) >= capacity_) {
        return;
    }
    // 取消固定，标记为ACCESSED（表示最近被访问）
    if (circular_[frame_id] == Status::EMPTY_OR_PINNED) {
        circular_[frame_id] = Status::ACCESSED;
    }
}

size_t ClockReplacer::Size() {
    const std::lock_guard<mutex_t> guard(mutex_);
    // 统计状态不为EMPTY_OR_PINNED的帧的数量
    size_t count = 0;
    for (size_t i = 0; i < capacity_; i++) {
        if (circular_[i] != Status::EMPTY_OR_PINNED) {
            count++;
        }
    }
    return count;
}

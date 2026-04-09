#include "replacer/clock_replacer.h"

#include <algorithm>

ClockReplacer::ClockReplacer(size_t num_pages)
    : circular_{num_pages, ClockReplacer::Status::EMPTY_OR_PINNED}, hand_{0}, capacity_{num_pages} {
}

ClockReplacer::~ClockReplacer() = default;

bool ClockReplacer::victim(frame_id_t *frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);
    
    size_t scanned = 0;
    while (scanned < capacity_) {
        if (circular_[hand_] == Status::UNTOUCHED) {
            *frame_id = hand_;
            circular_[hand_] = Status::EMPTY_OR_PINNED;
            hand_ = (hand_ + 1) % capacity_;
            return true;
        } else if (circular_[hand_] == Status::ACCESSED) {
            circular_[hand_] = Status::UNTOUCHED;
        }
        hand_ = (hand_ + 1) % capacity_;
        scanned++;
    }
    
    return false;
}

void ClockReplacer::pin(frame_id_t frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);
    if (frame_id < 0 || static_cast<size_t>(frame_id) >= capacity_) {
        return;
    }
    circular_[frame_id] = Status::EMPTY_OR_PINNED;
}

void ClockReplacer::unpin(frame_id_t frame_id) {
    const std::lock_guard<mutex_t> guard(mutex_);
    if (frame_id < 0 || static_cast<size_t>(frame_id) >= capacity_) {
        return;
    }
    if (circular_[frame_id] == Status::EMPTY_OR_PINNED) {
        circular_[frame_id] = Status::ACCESSED;
    }
}

size_t ClockReplacer::Size() {
    const std::lock_guard<mutex_t> guard(mutex_);
    size_t count = 0;
    for (size_t i = 0; i < capacity_; i++) {
        if (circular_[i] != Status::EMPTY_OR_PINNED) {
            count++;
        }
    }
    return count;
}

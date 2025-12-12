#include "lru_replacer.h"

LRUReplacer::LRUReplacer(size_t capacity) : capacity_(capacity) {}

bool LRUReplacer::victim(uint32_t &frame_id) {
    if (lru_list_.empty()) return false;

    frame_id = lru_list_.back();
    lru_list_.pop_back();
    lru_map_.erase(frame_id);
    return true;
}

void LRUReplacer::pin(uint32_t frame_id) {
    auto it = lru_map_.find(frame_id);
    if (it != lru_map_.end()) {
        lru_list_.erase(it->second);
        lru_map_.erase(it);
    }
}

void LRUReplacer::unpin(uint32_t frame_id) {
    if (lru_map_.count(frame_id) != 0) return;
    if (lru_list_.size() >= capacity_) return;

    lru_list_.push_front(frame_id);
    lru_map_[frame_id] = lru_list_.begin();
}

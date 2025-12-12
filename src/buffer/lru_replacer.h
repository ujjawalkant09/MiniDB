#pragma once
#include <list>
#include <unordered_map>

class LRUReplacer {
public:
    explicit LRUReplacer(size_t capacity);

    bool victim(uint32_t &frame_id);
    void pin(uint32_t frame_id);
    void unpin(uint32_t frame_id);

private:
    size_t capacity_;
    std::list<uint32_t> lru_list_;
    std::unordered_map<uint32_t, std::list<uint32_t>::iterator> lru_map_;
};

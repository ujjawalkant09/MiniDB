#pragma once
#include <unordered_map>
#include <vector>
#include "../storage/disk_manager.h"
#include "../common/config.h"
#include "lru_replacer.h"

struct Page {
    uint32_t page_id = INVALID_PAGE_ID;
    bool is_dirty = false;
    int pin_count = 0;
    char data[PAGE_SIZE];
};

class BufferPoolManager {
public:
    BufferPoolManager(size_t pool_size, DiskManager *disk_manager);

    Page* fetch_page(uint32_t page_id);
    void unpin_page(uint32_t page_id, bool is_dirty);
    Page* new_page(uint32_t &page_id);
    void flush_page(uint32_t page_id);

private:
    size_t pool_size_;
    DiskManager *disk_manager_;

    std::vector<Page> pages_;
    std::unordered_map<uint32_t, uint32_t> page_table_;
    std::vector<uint32_t> free_list_;
    LRUReplacer replacer_;
};

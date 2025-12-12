#include "buffer_pool_manager.h"
#include <cstring>

BufferPoolManager::BufferPoolManager(size_t pool_size, DiskManager *disk_manager)
    : pool_size_(pool_size),
      disk_manager_(disk_manager),
      replacer_(pool_size) {

    pages_.resize(pool_size);
    for (uint32_t i = 0; i < pool_size; i++) {
        free_list_.push_back(i);
    }
}

Page* BufferPoolManager::fetch_page(uint32_t page_id) {
    // Page already in buffer
    if (page_table_.count(page_id)) {
        uint32_t frame = page_table_[page_id];
        Page &page = pages_[frame];
        page.pin_count++;
        replacer_.pin(frame);
        return &page;
    }

    uint32_t frame;
    if (!free_list_.empty()) {
        frame = free_list_.back();
        free_list_.pop_back();
    } else {
        if (!replacer_.victim(frame)) {
            return nullptr; // all pages pinned
        }

        Page &victim = pages_[frame];
        if (victim.is_dirty) {
            disk_manager_->write_page(victim.page_id, victim.data);
        }
        page_table_.erase(victim.page_id);
    }

    Page &page = pages_[frame];
    disk_manager_->read_page(page_id, page.data);
    page.page_id = page_id;
    page.is_dirty = false;
    page.pin_count = 1;

    page_table_[page_id] = frame;
    replacer_.pin(frame);

    return &page;
}

void BufferPoolManager::unpin_page(uint32_t page_id, bool is_dirty) {
    if (!page_table_.count(page_id)) return;

    uint32_t frame = page_table_[page_id];
    Page &page = pages_[frame];

    if (page.pin_count <= 0) return;

    page.pin_count--;
    page.is_dirty |= is_dirty;

    if (page.pin_count == 0) {
        replacer_.unpin(frame);
    }
}

Page* BufferPoolManager::new_page(uint32_t &page_id) {
    uint32_t frame;
    if (!free_list_.empty()) {
        frame = free_list_.back();
        free_list_.pop_back();
    } else {
        if (!replacer_.victim(frame)) return nullptr;

        Page &victim = pages_[frame];
        if (victim.is_dirty) {
            disk_manager_->write_page(victim.page_id, victim.data);
        }
        page_table_.erase(victim.page_id);
    }

    page_id = disk_manager_->allocate_page();
    Page &page = pages_[frame];

    std::memset(page.data, 0, PAGE_SIZE);
    page.page_id = page_id;
    page.is_dirty = false;
    page.pin_count = 1;

    page_table_[page_id] = frame;
    replacer_.pin(frame);

    return &page;
}

void BufferPoolManager::flush_page(uint32_t page_id) {
    if (!page_table_.count(page_id)) return;
    uint32_t frame = page_table_[page_id];
    Page &page = pages_[frame];
    disk_manager_->write_page(page.page_id, page.data);
    page.is_dirty = false;
}

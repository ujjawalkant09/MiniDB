#include "table_page.h"
#include <cstring>
#include <cassert>

struct TablePageHeader {
    uint16_t num_slots;
    uint16_t free_space_offset;
};

TablePage::TablePage(char *data) : data_(data) {}

void TablePage::init() {
    auto *header = reinterpret_cast<TablePageHeader *>(data_);
    header->num_slots = 0;
    header->free_space_offset = PAGE_SIZE;
}

bool TablePage::insert_tuple(const char *tuple, uint16_t size, uint16_t &slot_id) {
    auto *header = reinterpret_cast<TablePageHeader *>(data_);

    // Safety guards
    assert(header->free_space_offset <= PAGE_SIZE);
    assert(header->num_slots < PAGE_SIZE / sizeof(uint16_t));

    uint16_t required =
        sizeof(TablePageHeader) +
        (header->num_slots + 1) * sizeof(uint16_t) +
        size;

    if (header->free_space_offset < required) {
        return false;
    }

    header->free_space_offset -= size;
    std::memcpy(data_ + header->free_space_offset, tuple, size);

    auto *slots = reinterpret_cast<uint16_t *>(data_ + sizeof(TablePageHeader));
    slots[header->num_slots] = header->free_space_offset;

    slot_id = header->num_slots;
    header->num_slots++;

    return true;
}

bool TablePage::read_tuple(uint16_t slot_id, char *out, uint16_t &size) {
    auto *header = reinterpret_cast<TablePageHeader *>(data_);
    if (slot_id >= header->num_slots) return false;

    auto *slots = reinterpret_cast<uint16_t *>(data_ + sizeof(TablePageHeader));
    uint16_t offset = slots[slot_id];

    uint16_t next_offset =
        (slot_id == 0)
            ? PAGE_SIZE
            : slots[slot_id - 1];

    size = next_offset - offset;
    std::memcpy(out, data_ + offset, size);
    return true;
}

uint16_t TablePage::num_slots() const {
    auto *header = reinterpret_cast<TablePageHeader *>(data_);
    return header->num_slots;
}

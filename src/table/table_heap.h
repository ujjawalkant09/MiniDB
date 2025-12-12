#pragma once
#include "../buffer/buffer_pool_manager.h"
#include "rid.h"
#include <vector>
#include <string>

class TableHeap {
public:
    explicit TableHeap(BufferPoolManager *bpm);

    bool insert_tuple(const char *tuple, uint16_t size, RID &rid);
    void scan_all(std::vector<std::string> &out);

private:
    BufferPoolManager *bpm_;
    std::vector<uint32_t> pages_;
};

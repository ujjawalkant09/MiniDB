#include "storage/disk_manager.h"
#include "buffer/buffer_pool_manager.h"
#include "table/table_heap.h"
#include <iostream>

int main() {
    DiskManager dm("test.db");
    BufferPoolManager bpm(10, &dm);
    TableHeap table(&bpm);

    RID rid1, rid2;
    table.insert_tuple("Alice", 5, rid1);
    table.insert_tuple("Bob", 3, rid2);

    std::vector<std::string> rows;
    table.scan_all(rows);

    for (auto &r : rows) {
        std::cout << r << std::endl;
    }

    return 0;
}

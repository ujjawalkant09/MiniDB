#include "storage/disk_manager.h"
#include "common/config.h"
#include <iostream>

int main() {
    DiskManager dm("test.db");

    char page[PAGE_SIZE]{};
    std::snprintf(page, PAGE_SIZE, "Hello, Mini DB");

    dm.write_page(0, page);

    char read_page[PAGE_SIZE]{};
    dm.read_page(0, read_page);

    std::cout << read_page << std::endl;
    return 0;
}

#include "disk_manager.h"
#include "../common/config.h"
#include <cstring>

DiskManager::DiskManager(const std::string &filename) {
    db_file_.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    if (!db_file_.is_open()) {
        db_file_.clear();
        db_file_.open(filename, std::ios::binary | std::ios::out);
        db_file_.close();
        db_file_.open(filename, std::ios::binary | std::ios::in | std::ios::out);
    }
}

DiskManager::~DiskManager() {
    db_file_.close();
}

void DiskManager::write_page(unsigned int page_id, const char *data) {
    db_file_.seekp(static_cast<std::streamoff>(page_id) * PAGE_SIZE);
    db_file_.write(data, PAGE_SIZE);
    db_file_.flush();
}

void DiskManager::read_page(unsigned int page_id, char *data) {
    db_file_.seekg(static_cast<std::streamoff>(page_id) * PAGE_SIZE);
    if (!db_file_.read(data, PAGE_SIZE)) {
        std::memset(data, 0, PAGE_SIZE);
    }
}

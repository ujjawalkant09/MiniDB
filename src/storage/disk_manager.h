#pragma once
#include <fstream>
#include <string>

class DiskManager {
public:
    explicit DiskManager(const std::string &filename);
    ~DiskManager();

    void write_page(unsigned int page_id, const char *data);
    void read_page(unsigned int page_id, char *data);

private:
    std::fstream db_file_;
};

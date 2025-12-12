#pragma once
#include <cstdint>
#include "../common/config.h"

class TablePage {
public:
    explicit TablePage(char *data);

    void init();

    bool insert_tuple(const char *tuple, uint16_t size, uint16_t &slot_id);
    bool read_tuple(uint16_t slot_id, char *out, uint16_t &size);

    uint16_t num_slots() const;

private:
    char *data_;
};

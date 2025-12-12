#include "table_heap.h"
#include "table_page.h"

TableHeap::TableHeap(BufferPoolManager *bpm) : bpm_(bpm) {}

bool TableHeap::insert_tuple(const char *tuple, uint16_t size, RID &rid) {
    // Try existing pages
    for (uint32_t page_id : pages_) {
        Page *page = bpm_->fetch_page(page_id);
        TablePage tp(page->data);

        // Ensure page is initialized (only first time)
        if (tp.num_slots() == 0) {
            tp.init();
        }

        uint16_t slot;
        if (tp.insert_tuple(tuple, size, slot)) {
            bpm_->unpin_page(page_id, true);
            rid = {page_id, slot};
            return true;
        }

        bpm_->unpin_page(page_id, false);
    }

    // None of the existing pages fit the tuple → allocate new page
    uint32_t new_page_id;
    Page *page = bpm_->new_page(new_page_id);
    if (!page) return false;

    TablePage tp(page->data);

    // Always initialize a new page
    tp.init();

    uint16_t slot;
    bool success = tp.insert_tuple(tuple, size, slot);
    bpm_->unpin_page(new_page_id, true);

    if (!success) return false;

    pages_.push_back(new_page_id);
    rid = {new_page_id, slot};
    return true;
}

void TableHeap::scan_all(std::vector<std::string> &out) {
    for (uint32_t page_id : pages_) {
        Page *page = bpm_->fetch_page(page_id);
        TablePage tp(page->data);

        uint16_t n = tp.num_slots();
        for (uint16_t i = 0; i < n; i++) {
            char buf[1024];
            uint16_t size = 0;

            bool ok = tp.read_tuple(i, buf, size);
            if (ok) {
                out.emplace_back(buf, size);
            }
        }

        bpm_->unpin_page(page_id, false);
    }
}


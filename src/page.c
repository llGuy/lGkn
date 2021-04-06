#include "page.h"

static uint32_t current_ptr;

uint32_t kernel_malloc(
    uint32_t size,
    bool32_t is_aligned,
    uint32_t *physical) {
    if (is_aligned && (current_ptr & 0xFFFFF000)) {
        current_ptr &= 0xFFFFF000;
        current_ptr += 0x1000;
    }

    if (physical) {
        *physical = current_ptr;
    }

    uint32_t tmp = current_ptr;
    current_ptr += size;
    return tmp;
}

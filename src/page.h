#ifndef PAGE_H
#define PAGE_H

#include "int.h"

uint32_t kernel_malloc(
    uint32_t size,
    bool32_t is_aligned,
    uint32_t *physical);

typedef struct {
    uint32_t present : 1;
    // If 0 -> read-only ; If 1 -> readwrite
    uint32_t is_readwrite : 1;
    // If 0 -> kernel-level ; If 1 -> user-level
    uint32_t is_user : 1;
    // Has the page been accessed since last refresh
    uint32_t accessed : 1;
    // Has the page been written to since last refresh
    uint32_t dirty : 1;
    // Reserved / unused
    uint32_t unused : 7;
    // Frame address (shifted right 12 bits because the last 12 will be 0 anyway because alignment)
    uint32_t frame : 20;
} page_t;

typedef struct {
    page_t pages[1024];
} page_table_t;

#endif

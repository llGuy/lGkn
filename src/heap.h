#ifndef HEAP_H
#define HEAP_H

#include "int.h"

typedef struct {
    uint32_t magic;
    uint8_t is_hole;
    uint32_t size;
} header_t;

typedef struct {
    uint32_t magic;
    header_t *header;
} footer_t;

#endif

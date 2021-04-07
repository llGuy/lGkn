#include "utility.h"

void memset(void *p, uint8_t value, uint32_t size) {
    for (uint32_t i = 0; i < size; ++i) {
        ((uint8_t *)p)[i] = value;
    }
}

#ifndef UTILITY_H
#define UTILITY_H

#include "int.h"

void memset(void *p, uint8_t value, uint32_t size);
void memcpy(void *dst, const void *src, uint32_t len);

#define PANIC(msg) panic(msg, __FILE__, __LINE__)
#define ASSERT(b) ((b) ? (void)0 : panic_assert(__FILE__, __LINE__, #b))

void panic(char *msg, char *file, uint32_t line);
void panic_assert(char *file, uint32_t line, char *desc);

#endif

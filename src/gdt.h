#ifndef GDT_H
#define GDT_H

#include "int.h"

typedef struct {
    uint16_t limit_low;

    /* 24bits for the base address of the segment */
    uint16_t base_low;
    uint8_t base_middle;

    /*
      Bit 7 = is segment present?
      Bits 5 and 6 = descriptor privilege level - ring 0 -> 3
      Bit 4 = descriptor type
      Bit 0 to 3 = segment type (code -> data segment)
     */
    uint8_t access;
    /*
      Bit 7 = granularity (0 -> 1 byte; 1 = 1kbyte)
      Bit 6 = operand size (0 = 16bit; 1 = 32bit)
      Bit 5 = always 0
      Bit 4 = always 0
      Bit 0 to 3 = another 4 bits for the limit_low variable
     */
    uint8_t granularity;
    /* The final 8 bits of the base address */
    uint8_t base_high;
} __attribute__((packed)) gdt_entry_t;

typedef struct {
    // Upper 16 bits of all selector limits
    uint16_t limit;
    // Address of the first gdb_entry
    uint32_t base;
} __attribute__((packed)) gdt_ptr_t;

void gdt_init();

#endif

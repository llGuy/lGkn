#ifndef IDT_H
#define IDT_H

#include "int.h"

typedef struct {
    uint16_t base_low;
    uint16_t segment_selector;
    uint8_t zero;
    /*
      Bit 0 - 4 = always set to 00110
     */
    uint8_t flags;
    uint16_t base_high;
} __attribute__((packed)) idt_entry_t;

typedef struct {
    uint16_t limit;
    uint32_t base;
} __attribute__((packed)) idt_ptr_t;

void idt_init();

/* 
   There are 32 special interrupts that the CPU will trigger

    0 - Division by zero exception
    1 - Debug exception
    2 - Non maskable interrupt
    3 - Breakpoint exception
    4 - 'Into detected overflow'
    5 - Out of bounds exception
    6 - Invalid opcode exception
    7 - No coprocessor exception
    8 - Double fault (pushes an error code)
    9 - Coprocessor segment overrun
    10 - Bad TSS (pushes an error code)
    11 - Segment not present (pushes an error code)
    12 - Stack fault (pushes an error code)
    13 - General protection fault (pushes an error code)
    14 - Page fault (pushes an error code)
    15 - Unknown interrupt exception
    16 - Coprocessor fault
    17 - Alignment check exception
    18 - Machine check exception
    19-31 - Reserved

*/

extern void isr0();
// ...
extern void isr31();

#endif

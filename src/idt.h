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
extern void isr1();
extern void isr2();
extern void isr3();
extern void isr4();
extern void isr5();
extern void isr6();
extern void isr7();
extern void isr8();
extern void isr9();
extern void isr10();
extern void isr11();
extern void isr12();
extern void isr13();
extern void isr14();
extern void isr15();
extern void isr16();
extern void isr17();
extern void isr18();
extern void isr19();
extern void isr20();
extern void isr21();
extern void isr22();
extern void isr23();
extern void isr24();
extern void isr25();
extern void isr26();
extern void isr27();
extern void isr28();
extern void isr29();
extern void isr30();
extern void isr31();
extern void irq0();
extern void irq1();
extern void irq2();
extern void irq3();
extern void irq4();
extern void irq5();
extern void irq6();
extern void irq7();
extern void irq8();
extern void irq9();
extern void irq10();
extern void irq11();
extern void irq12();
extern void irq13();
extern void irq14();
extern void irq15();

typedef struct {
    // Data segment selector (pushed with "push eax") - fourth thing to get pushed
    uint32_t ds;
    // Pushed by "pusha" - third thing to get pushed
    uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
    // Interrupt number and error code (if applicable) - second thing to get pushed
    uint32_t int_no, err_code;
    // Pushed by the processor automatically - first thing to get pushed
    uint32_t eip, cs, eflags, useresp, ss;
} registers_t;

typedef void (* isr_t)(registers_t);
void register_interrupt_handler(uint8_t n, isr_t handler);

#endif

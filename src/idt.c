#include "idt.h"
#include "monitor.h"

static idt_entry_t idt_entries[256];
static idt_ptr_t idt_ptr;

extern void idt_flush(uint32_t);

void idt_set_entry_fields(
    uint8_t num,
    uint32_t base,
    uint16_t selector,
    uint8_t flags) {
    /* Set bits for base */
    idt_entries[num].base_low = base & 0xFFFF;
    idt_entries[num].base_high = (base >> 16) & 0xFFFF;
    /* The rest */
    idt_entries[num].segment_selector = selector;
    idt_entries[num].zero = 0;
    idt_entries[num].flags = flags /* | 0x60 when we do user-mode*/;
}

void idt_init() {
    idt_ptr.limit = sizeof(idt_entry_t) * 256 - 1;
    idt_ptr.base = (uint32_t)&idt_entries;

    /* Implement memset soon */
    for (uint32_t i = 0; i < sizeof(idt_entry_t) * 256; ++i) {
        ((uint8_t *)idt_entries)[i] = 0;
    }

    idt_set_entry_fields(0, (uint32_t)isr0, 0x08, 0x8E);
    idt_set_entry_fields(1, (uint32_t)isr1, 0x08, 0x8E);
    idt_set_entry_fields(2, (uint32_t)isr2, 0x08, 0x8E);
    idt_set_entry_fields(3, (uint32_t)isr3, 0x08, 0x8E);
    idt_set_entry_fields(4, (uint32_t)isr4, 0x08, 0x8E);
    idt_set_entry_fields(5, (uint32_t)isr5, 0x08, 0x8E);
    idt_set_entry_fields(6, (uint32_t)isr6, 0x08, 0x8E);
    idt_set_entry_fields(7, (uint32_t)isr7, 0x08, 0x8E);
    idt_set_entry_fields(8, (uint32_t)isr8, 0x08, 0x8E);
    idt_set_entry_fields(9, (uint32_t)isr9, 0x08, 0x8E);
    idt_set_entry_fields(10, (uint32_t)isr10, 0x08, 0x8E);
    idt_set_entry_fields(11, (uint32_t)isr11, 0x08, 0x8E);
    idt_set_entry_fields(12, (uint32_t)isr12, 0x08, 0x8E);
    idt_set_entry_fields(13, (uint32_t)isr13, 0x08, 0x8E);
    idt_set_entry_fields(14, (uint32_t)isr14, 0x08, 0x8E);
    idt_set_entry_fields(15, (uint32_t)isr15, 0x08, 0x8E);
    idt_set_entry_fields(16, (uint32_t)isr16, 0x08, 0x8E);
    idt_set_entry_fields(17, (uint32_t)isr17, 0x08, 0x8E);
    idt_set_entry_fields(18, (uint32_t)isr18, 0x08, 0x8E);
    idt_set_entry_fields(19, (uint32_t)isr19, 0x08, 0x8E);
    idt_set_entry_fields(20, (uint32_t)isr20, 0x08, 0x8E);
    idt_set_entry_fields(21, (uint32_t)isr21, 0x08, 0x8E);
    idt_set_entry_fields(22, (uint32_t)isr22, 0x08, 0x8E);
    idt_set_entry_fields(23, (uint32_t)isr23, 0x08, 0x8E);
    idt_set_entry_fields(24, (uint32_t)isr24, 0x08, 0x8E);
    idt_set_entry_fields(25, (uint32_t)isr25, 0x08, 0x8E);
    idt_set_entry_fields(26, (uint32_t)isr26, 0x08, 0x8E);
    idt_set_entry_fields(27, (uint32_t)isr27, 0x08, 0x8E);
    idt_set_entry_fields(28, (uint32_t)isr28, 0x08, 0x8E);
    idt_set_entry_fields(29, (uint32_t)isr29, 0x08, 0x8E);
    idt_set_entry_fields(30, (uint32_t)isr30, 0x08, 0x8E);
    idt_set_entry_fields(31, (uint32_t)isr31, 0x08, 0x8E);

    idt_flush((uint32_t)&idt_ptr);
}

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

void isr_handler(registers_t regs) {
    monitor_write("Received interrupt: ");
    monitor_putbase10(regs.int_no);
    monitor_putch('\n');
}

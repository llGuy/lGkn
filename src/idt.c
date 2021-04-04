#include "idt.h"

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
}

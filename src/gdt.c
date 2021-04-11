#include "gdt.h"

static gdt_entry_t gdt_entries[3];
static gdt_ptr_t gdt_ptr;

extern void gdt_flush(uint32_t);

void gdt_set_entry_fields(
  int32_t num,
  uint32_t base,
  uint32_t limit,
  uint8_t access,
  uint8_t granularity) {
  /* Set fields of the base */
  gdt_entries[num].base_low = (base & 0xFFFF);
  gdt_entries[num].base_middle = (base >> 16) & 0xFF;
  gdt_entries[num].base_high = (base >> 24) & 0xFF;
  /* Set fields related to the limit */
  gdt_entries[num].limit_low = limit & 0xFFFF;
  gdt_entries[num].granularity = (limit >> 16) & 0x0F;
  /* The rest */
  gdt_entries[num].granularity |= granularity & 0xF0;
  gdt_entries[num].access = access;
}

void gdt_init() {
  gdt_ptr.limit = sizeof(gdt_entry_t) * 3 - 1;
  gdt_ptr.base = (uint32_t)&gdt_entries;

  /* Null segment */
  gdt_set_entry_fields(0, 0, 0, 0, 0);

  /* Code segment */
  gdt_set_entry_fields(1, 0, 0xFFFFFFFF, 0x9A, 0xCF);

  /* Data segment */
  gdt_set_entry_fields(2, 0, 0xFFFFFFFF, 0x92, 0xCF);

  /* User mode code segment */
  // gdt_set_entry_fields(3, 0, 0xFFFFFFFF, 0xFA, 0xCF);

  /* User mode data segment */
  // gdt_set_entry_fields(4, 0, 0xFFFFFFFF, 0xF2, 0xCF);

  gdt_flush((uint32_t)&gdt_ptr);
}

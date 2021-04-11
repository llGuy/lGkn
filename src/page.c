#include "page.h"
#include "monitor.h"
#include "utility.h"
#include "idt.h"

extern uint32_t end;
static uint32_t current_ptr = (uint32_t)&end;

uint32_t kernel_malloc(
  uint32_t size,
  bool32_t is_aligned,
  uint32_t *physical) {
  if (is_aligned && (current_ptr & 0xFFFFF000)) {
    current_ptr &= 0xFFFFF000;
    current_ptr += 0x1000;
  }

  if (physical) {
    *physical = current_ptr;
  }

  uint32_t tmp = current_ptr;
  current_ptr += size;
  return tmp;
}

static uint32_t *frames;
static uint32_t frame_count;

#define INDEX_FROM_BIT(a) (a / (8 * 4))
#define OFFSET_FROM_BIT(a) (a % (8 * 4))

static void set_frame(uint32_t frame_addr) {
  uint32_t frame = frame_addr / 0x1000;
  uint32_t idx = INDEX_FROM_BIT(frame);
  uint32_t off = OFFSET_FROM_BIT(frame);
  frames[idx] |= (0x1 << off);
}

static void clear_frame(uint32_t frame_addr) {
  uint32_t frame = frame_addr / 0x1000;
  uint32_t idx = INDEX_FROM_BIT(frame);
  uint32_t off = OFFSET_FROM_BIT(frame);
  frames[idx] &= ~(0x1 << off);
}

static uint32_t test_trame(uint32_t frame_addr) {
  uint32_t frame = frame_addr / 0x1000;
  uint32_t idx = INDEX_FROM_BIT(frame);
  uint32_t off = OFFSET_FROM_BIT(frame);
  return frames[idx] & (0x1 << off);
}

static uint32_t first_frame() {
  for (uint32_t i = 0; i < INDEX_FROM_BIT(frame_count); ++i) {
    if (frames[i] != 0xFFFFFFFF) {
      for (uint32_t j = 0; j < 32; ++j) {
        uint32_t to_test = 0x1 << j;
        if (!(frames[i] & to_test)) {
          return i * 4 * 8 + j;
        }
      }
    }
  }

  // Invalid
  return 0xFFFFFFFF;
}

void alloc_frame(page_t *page, bool32_t is_kernel, bool32_t is_writeable) {
  if (page->frame != 0) {
    return;
  }
  else {
    uint32_t idx = first_frame();

    if (idx == (uint32_t)-1) {
      monitor_write("No more frames!\n");
    }

    set_frame(idx * 0x1000);

    page->present = 1;
    page->is_readwrite = (is_writeable) ? 1 : 0;
    page->is_user = (is_kernel) ? 0 : 1;
    page->frame = idx;
  }
}

void free_frame(page_t *page) {
  uint32_t frame;
  if (!(frame = page->frame)) {
    return;
  }
  else {
    clear_frame(frame);
    page->frame = 0x0;
  }
}

page_directory_t *g_kernel_directory;
static page_directory_t *current_directory;

static void page_fault(registers_t regs);

void paging_init() {
  // Size of physical memory
  uint32_t mem_end_page = 0x1000000;

  frame_count = mem_end_page / 0x1000;
  frames = (uint32_t *)kernel_malloc(INDEX_FROM_BIT(frame_count), 0, 0);
  memset(frames, 0, sizeof(uint32_t) * INDEX_FROM_BIT(frame_count));

  g_kernel_directory = (page_directory_t *)kernel_malloc(
    sizeof(page_directory_t), 1, 0);

  memset(g_kernel_directory, 0, sizeof(page_directory_t));
  current_directory = g_kernel_directory;

  int i = 0;
  while (i < current_ptr) {
    alloc_frame(paging_get_page(i, 1, g_kernel_directory), 0, 0);
    i += 0x1000;
  }

  register_interrupt_handler(14, page_fault);

  paging_switch_directory(g_kernel_directory);
}

void paging_switch_directory(page_directory_t *new_dir) {
  current_directory = new_dir;
  asm volatile("mov %0, %%cr3" :: "r"(&new_dir->tables_physical));
  uint32_t cr0;
  asm volatile("mov %%cr0, %0" : "=r"(cr0));
  cr0 |= 0x80000000;
  asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

page_t *paging_get_page(uint32_t addr, bool32_t make, page_directory_t *dir) {
  addr /= 0x1000;
  uint32_t table_idx = addr / 1024;
  if (dir->tables[table_idx]) {
    return &dir->tables[table_idx]->pages[addr % 1024];
  }
  else if (make) {
    uint32_t tmp;

    dir->tables[table_idx] = (page_table_t *)kernel_malloc(
      sizeof(page_table_t), 1, &tmp);

    memset(dir->tables[table_idx], 0, 0x1000);
    dir->tables_physical[table_idx] = tmp | 0x7; // Present, RW, USER
    return &dir->tables[table_idx]->pages[addr % 1024];
  }
  else {
    return 0;
  }
}

static void page_fault(registers_t regs) {
  uint32_t faulting_address;
  asm volatile ("mov %%cr2, %0" : "=r" (faulting_address));

  int present = !(regs.err_code & 1);
  int rw = regs.err_code & 2;
  int us = regs.err_code & 4;
  int reserved = regs.err_code & 8;
  int id = regs.err_code & 16;

  monitor_write("Page fault! (");
  if (present) monitor_write("present ");
  if (rw) monitor_write("read-only ");
  if (us) monitor_write("user-mode ");
  if (reserved) monitor_write("reserved ");
  monitor_write(") at 0x");
  monitor_putbase16(faulting_address);
  monitor_write("\n");

  for (;;);
}

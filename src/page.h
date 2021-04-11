#ifndef PAGE_H
#define PAGE_H

#include "int.h"
#include "idt.h"

uint32_t kernel_malloc(
  uint32_t size,
  bool32_t is_aligned,
  uint32_t *physical);

typedef struct {
  // Is it present?
  uint32_t present : 1;
  // If 0 -> read-only ; If 1 -> readwrite
  uint32_t is_readwrite : 1;
  // If 0 -> kernel-level ; If 1 -> user-level
  uint32_t is_user : 1;
  // Has the page been accessed since last refresh
  uint32_t accessed : 1;
  // Has the page been written to since last refresh
  uint32_t dirty : 1;
  // Reserved / unused
  uint32_t unused : 7;
  /* 
     Frame address (shifted right 12 bits because the last 12 will be 0 
     anyway because alignment)
     This is the physical address of the frame
   */
  uint32_t frame : 20;
} page_t;

typedef struct {
  page_t pages[1024];
} page_table_t;

typedef struct {
  // Array of pointers to page tables
  page_table_t *tables[1024];
  // Array of pointers to the page tables but with physical address
  uint32_t  tables_physical[1024];
  // Physical address of tables_physical
  uint32_t ptr_physical_addr;
} page_directory_t;

void paging_init();
void paging_switch_directory(page_directory_t *new_dir);
// If make && page table wasn't created, create it
page_t *paging_get_page(uint32_t address, bool32_t make, page_directory_t *dir);
// ISR
void paging_fault(registers_t reg);

void alloc_frame(page_t *page, bool32_t is_kernel, bool32_t is_writeable);
void free_frame(page_t *page);

extern page_directory_t *g_kernel_directory;

#endif

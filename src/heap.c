#include "heap.h"
#include "page.h"
#include "utility.h"

static uint32_t s_find_smallest_hole(
  uint32_t size,
  uint8_t page_align,
  heap_t *heap) {
  uint32_t it = 0;
  while (it < heap->index.size) {
    header_t *hd = (header_t *)find_ordered_array(it, &heap->index);

    if (page_align > 0) {
      uint32_t loc = (uint32_t)hd;
      int32_t off = 0;

      if (((loc + sizeof(header_t)) & 0xFFFFF000) != 0)
        off = 0x1000 - (loc + sizeof(header_t)) % 0x1000;

      int32_t hole_size = (int32_t)hd->size - off;

      if (hole_size >= (int32_t)size)
        break;
    }
    else if (hd->size >= size) {
      break;
    }

    ++it;
  }

  if (it == heap->index.size) {
    return -1;
  }
  else {
    return it;
  }
}

static int8_t s_hd_less_than(void *a, void *b) {
  return (((header_t *)a) -> size < ((header_t *)b)->size) ? 1 : 0;
}

heap_t *heap_create(
  uint32_t start,
  uint32_t end,
  uint32_t max,
  uint8_t supervisor,
  uint8_t readonly) {
  ASSERT(start % 0x1000 == 0);
  ASSERT(end % 0x1000 == 0);

  heap_t *heap = (heap_t *)kernel_malloc(sizeof(heap_t), 0, NULL);

  heap->index = place_ordered_array(
    (void *)start,
    HEAP_INDEX_SIZE,
    &s_hd_less_than);

  start += sizeof(type_t) * HEAP_INDEX_SIZE;

  if ((start & 0xFFFFF000) != 0) {
    start &= 0xFFFFF000;
    start += 0x1000;
  }

  heap->start = start;
  heap->end = end;
  heap->max = max;
  heap->supervisor = supervisor;
  heap->readonly = readonly;

  header_t *hole = (header_t *)start;
  hole->size = end - start;
  hole->magic = HEAP_MAGIC;
  hole->is_hole = 1;

  insert_ordered_array((void *)hole, &heap->index);

  return heap;
}

static void s_expand(uint32_t new_size, heap_t *heap) {
  ASSERT(new_size > heap->end - heap->start);

  if ((new_size & 0xFFFFF000) != 0) {
    new_size &= 0xFFFFF000;
    new_size += 0x1000;
  }

  ASSERT(heap->start + new_size <= heap->max);

  uint32_t old_size = heap->end - heap->start;

  uint32_t i = old_size;
  while (i < new_size) {
    alloc_frame(
      paging_get_page(heap->start + i, 1, g_kernel_directory),
      (heap->supervisor) ? 1 : 0, (heap->readonly) ? 0 : 1);

    i += 0x1000;
  }

  heap->end = heap->start + new_size;
}

static uint32_t s_contract(uint32_t new_size, heap_t *heap) {
  ASSERT(new_size < heap->end - heap->start);

  if (new_size & 0x1000) {
    new_size &= 0x1000;
    new_size += 0x1000;
  }

  if (new_size < HEAP_MIN_SIZE) {
    new_size = HEAP_MIN_SIZE;
  }

  uint32_t old_size = heap->end - heap->start;

  uint32_t i = old_size - 0x1000;

  while (new_size < i) {
    free_frame(paging_get_page(heap->start + i, 0, g_kernel_directory));
    i -= 0x1000;
  }

  heap->end = heap->start + new_size;

  return new_size;
}

void *alloc(uint32_t size, uint8_t page_align, heap_t *heap) {
  uint32_t new_size = size + sizeof(header_t) + sizeof(footer_t);
  int32_t it = s_find_smallest_hole(new_size, page_align, heap);

  if (it == -1) {
    // There wasn't a hole that would fit this
  }

  header_t *orig_hole_hd = (header_t *)find_ordered_array(it, &heap->index);
  uint32_t orig_hole_pos = (uint32_t)orig_hole_hd;
  uint32_t orig_hole_size = orig_hole_hd->size;
  if (orig_hole_size - new_size < sizeof(header_t) + sizeof(footer_t)) {
    // Increase the requested size to the size of the hole we find
    size += orig_hole_size - new_size;
    new_size = orig_hole_size;
  }

  if (page_align && orig_hole_pos & 0xFFFFF000) {
    uint32_t new_loc = orig_hole_pos + 0x1000 -
      (orig_hole_pos & 0xFFF) - sizeof(header_t);

    header_t *hole_hd = (header_t *)orig_hole_pos;
    hole_hd->size = 0x1000 - (orig_hole_pos & 0xFFF) - sizeof(header_t);
  }
}

#include "oa.h"
#include "page.h"
#include "utility.h"

int8_t def_lessthan_pred(type_t a, type_t b) {
  return (a < b) ? 1 : 0;
}

ordered_array_t create_ordered_array(
  uint32_t max_size,
  lessthan_pred_t pred) {
  ordered_array_t to_ret = {
    .array = (type_t *)kernel_malloc(max_size * sizeof(type_t), 0, NULL),
    .size = 0,
    .max_size = max_size,
    .lessthan = pred
  };

  memset(to_ret.array, 0, max_size * sizeof(type_t));

  return to_ret;
}

ordered_array_t place_ordered_array(
  void *addr,
  uint32_t max_size,
  lessthan_pred_t pred) {
  ordered_array_t to_ret = {
    .array = (type_t *)addr,
    .size = 0,
    .max_size = max_size,
    .lessthan = pred
  };

  memset(to_ret.array, 0, max_size * sizeof(type_t));

  return to_ret;
}

void destroy_ordered_array(ordered_array_t *array) {
  // Free array->array
}

void insert_ordered_array(type_t item, ordered_array_t *array) {
  uint32_t it = 0;

  while (
    it < array->size &&
    array->lessthan(array->array[it], item)) {
    ++it;
  }

  if (it == array->size) {
    array->array[array->size++] = item;
  }
  else {
    type_t tmp = array->array[it];
    array->array[it] = item;

    while (it < array->size) {
      it++;
      type_t tmp2 = array->array[it];
      array->array[it] = tmp;
      tmp = tmp2;
    }

    ++array->size;
  }
}

type_t find_ordered_array(uint32_t i, ordered_array_t *array) {
  return array->array[i];
}

void remove_ordered_array(uint32_t i, ordered_array_t *array) {
  while (i < array->size) {
    array->array[i] = array->array[i + 1];
    ++i;
  }

  --array->size;
}

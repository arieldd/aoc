#pragma once
#include <assert.h>
#include <stdlib.h>

#define ASSERT_BOUNDS                                                          \
  do {                                                                         \
    assert(index >= 0 && index < arr->count && "Array index out of bounds");   \
  } while (0);

#define EXPAND_IF_NEEDED(item_type)                                            \
  do {                                                                         \
    if (arr->count >= arr->capacity) {                                         \
      void *new_mem =                                                          \
          realloc(arr->items, sizeof(item_type) * arr->capacity * 2);          \
      if (new_mem == NULL)                                                     \
        return 0;                                                              \
      arr->items = (item_type *)new_mem;                                       \
      arr->capacity *= 2;                                                      \
    }                                                                          \
  } while (0);

#define SHRINK_IF_NEEDED(item_type)                                            \
  do {                                                                         \
    if (arr->count < arr->capacity / 4) {                                      \
      void *new_mem =                                                          \
          realloc(arr->items, sizeof(item_type) * arr->capacity / 2);          \
      if (new_mem == NULL)                                                     \
        return 0;                                                              \
      arr->items = (item_type *)new_mem;                                       \
      arr->capacity /= 2;                                                      \
    }                                                                          \
  } while (0);

#define DECLARE_VECTOR_OF_TYPE(vector_name, item_type)                         \
  typedef struct {                                                             \
    item_type *items;                                                          \
    size_t count;                                                              \
    size_t capacity;                                                           \
  } vector_name;                                                               \
                                                                               \
  vector_name vector_name##_error = {0};                                       \
                                                                               \
  vector_name vector_name##_init(size_t capacity) {                            \
    vector_name arr;                                                           \
    if (capacity == 0)                                                         \
      return vector_name##_error;                                              \
    void *mem_block = calloc(capacity, sizeof(item_type));                     \
    if (mem_block == NULL)                                                     \
      return vector_name##_error;                                              \
    arr.items = (item_type *)mem_block;                                        \
    arr.count = 0;                                                             \
    arr.capacity = capacity;                                                   \
                                                                               \
    return arr;                                                                \
  }                                                                            \
                                                                               \
  item_type *vector_name##_at(vector_name *arr, size_t index) {                \
    ASSERT_BOUNDS                                                              \
    return &arr->items[index];                                                 \
  }                                                                            \
                                                                               \
  size_t vector_name##_append(vector_name *arr, item_type element) {           \
    EXPAND_IF_NEEDED(item_type)                                                \
    arr->items[arr->count++] = element;                                        \
    return arr->count;                                                         \
  }                                                                            \
                                                                               \
  size_t vector_name##_insert(vector_name *arr, item_type element,             \
                              size_t index) {                                  \
    ASSERT_BOUNDS                                                              \
    EXPAND_IF_NEEDED(item_type)                                                \
    for (size_t i = arr->count; i > index; i--) {                              \
      arr->items[i] = arr->items[i - 1];                                       \
    }                                                                          \
    arr->items[index] = element;                                               \
    return ++arr->count;                                                       \
  }                                                                            \
                                                                               \
  size_t vector_name##_swap_remove(vector_name *arr, int index) {              \
    ASSERT_BOUNDS                                                              \
    arr->items[index] = arr->items[arr->count - 1];                            \
    arr->count--;                                                              \
    SHRINK_IF_NEEDED(item_type)                                                \
    return arr->count;                                                         \
  }                                                                            \
                                                                               \
  size_t vector_name##_shift_remove(vector_name *arr, int index) {             \
    ASSERT_BOUNDS                                                              \
    for (size_t i = 0; i < arr->count - 1; i++) {                              \
      arr->items[i] = arr->items[i + 1];                                       \
    }                                                                          \
    arr->count--;                                                              \
    SHRINK_IF_NEEDED(item_type)                                                \
    return arr->count;                                                         \
  }                                                                            \
                                                                               \
  void vector_name##_free(vector_name arr) { free(arr.items); }                \
                                                                               \
  void vector_name##_clear(vector_name *arr) { arr->count = 0; }

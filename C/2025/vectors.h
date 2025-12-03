#pragma once
#include <stdlib.h>

#define DECLARE_VECTOR_TYPE(item_type, type_name)                              \
  typedef struct {                                                             \
    item_type *items;                                                          \
    size_t count;                                                              \
    size_t capacity;                                                           \
  } type_name;                                                                 \
                                                                               \
  void type_name##_free(type_name *arr) { free(arr->items); }                  \
                                                                               \
  void type_name##_clear(type_name *arr) { arr->count = 0; }                   \
                                                                               \
  int type_name##_init(type_name *arr, size_t capacity) {                      \
    if (capacity == 0)                                                         \
      return -1;                                                               \
    void *mem_block = calloc(capacity, sizeof(item_type));                     \
    if (mem_block == NULL)                                                     \
      return -1;                                                               \
    arr->items = (item_type *)mem_block;                                       \
    arr->count = 0;                                                            \
    arr->capacity = capacity;                                                  \
                                                                               \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  item_type type_name##_at(type_name *arr, size_t idx) {                       \
    if (idx >= arr->count)                                                     \
      return 0;                                                                \
    return arr->items[idx];                                                    \
  }                                                                            \
                                                                               \
  int type_name##_append(type_name *arr, item_type element) {                  \
    if (arr->count >= arr->capacity) {                                         \
      void *new_mem =                                                          \
          realloc(arr->items, sizeof(item_type) * arr->capacity * 2);          \
      if (new_mem == NULL)                                                     \
        return -1;                                                             \
      arr->items = (item_type *)new_mem;                                       \
      arr->capacity *= 2;                                                      \
    }                                                                          \
    arr->items[arr->count++] = element;                                        \
    return arr->count;                                                         \
  }                                                                            \
                                                                               \
  int type_name##_remove(type_name *arr, item_type element) {                  \
    int i = 0;                                                                 \
    for (i = 0; i < arr->count; i++) {                                         \
      if (arr->items[i] == element) {                                          \
        arr->items[i] = arr->items[arr->count - 1];                            \
        break;                                                                 \
      }                                                                        \
    }                                                                          \
    if (i == arr->count)                                                       \
      return -1; /*Not found*/                                                 \
    arr->count--;                                                              \
    if (arr->count < arr->capacity / 4) {                                      \
      void *new_mem =                                                          \
          realloc(arr->items, sizeof(item_type) * arr->capacity / 2);          \
      arr->items = (item_type *)new_mem;                                       \
      arr->capacity /= 2;                                                      \
    }                                                                          \
    return 0;                                                                  \
  }

// Basic vector types
DECLARE_VECTOR_TYPE(int, vi)
DECLARE_VECTOR_TYPE(long, vl)
DECLARE_VECTOR_TYPE(long long, vll)
DECLARE_VECTOR_TYPE(float, vf)
DECLARE_VECTOR_TYPE(double, vd)
DECLARE_VECTOR_TYPE(char, str)

#pragma once
#include <stdlib.h>

#define DECLARE_VECTOR_TYPE(item_type, type_name, cmp)                         \
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
  int type_name##_remove_at(type_name *arr, int index) {                       \
    if (index < 0 || index >= arr->count)                                      \
      return -1;                                                               \
    arr->items[index] = arr->items[arr->count - 1];                            \
    arr->count--;                                                              \
    if (arr->count < arr->capacity / 4) {                                      \
      void *new_mem =                                                          \
          realloc(arr->items, sizeof(item_type) * arr->capacity / 2);          \
      arr->items = (item_type *)new_mem;                                       \
      arr->capacity /= 2;                                                      \
    }                                                                          \
    return 0;                                                                  \
  }                                                                            \
                                                                               \
  int type_name##_remove(type_name *arr, item_type element) {                  \
    int i = 0;                                                                 \
    for (i = 0; i < arr->count; i++) {                                         \
      if (cmp(arr->items[i], element)) {                                       \
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

#define EQ_CMP(type, name)                                                     \
  char cmp_##name(type e1, type e2) { return e1 == e2; };

EQ_CMP(int, i);
EQ_CMP(long, l);
EQ_CMP(long long, ll);
EQ_CMP(float, f);
EQ_CMP(double, d);
EQ_CMP(char, c);

// Basic vector types
DECLARE_VECTOR_TYPE(int, vi, cmp_i)
DECLARE_VECTOR_TYPE(long, vl, cmp_l)
DECLARE_VECTOR_TYPE(long long, vll, cmp_ll)
DECLARE_VECTOR_TYPE(float, vf, cmp_f)
DECLARE_VECTOR_TYPE(double, vd, cmp_d)
DECLARE_VECTOR_TYPE(char, str, cmp_c)

#include <stdio.h>
#include <stdlib.h>

typedef struct {
  int *items;
  unsigned int size;
  unsigned int capacity;
} dyn_arr;

dyn_arr *arr_init(unsigned int capacity) {
  dyn_arr *result = malloc(sizeof(dyn_arr));
  result->items = calloc(capacity, sizeof(int));
  result->capacity = capacity;
  result->size = 0;
  return result;
}

void arr_expand(dyn_arr *arr) {
  arr->capacity *= 2;
  arr->items = realloc(arr->items, arr->capacity * sizeof(int));
}

void arr_append(dyn_arr *arr, int elem) {
  if (arr->size >= arr->capacity) {
    arr_expand(arr);
  }
  arr->items[arr->size++] = elem;
}

int main(int argc, char **argv) {
  if (argc < 2) {
    printf("Please provide the input file\n");
    return 1;
  }

  dyn_arr *buffer = arr_init(100);
  FILE *f = fopen(argv[1], "r");

  if (f == NULL) {
    printf("Could not open file\n");
    return 1;
  }

  int digit, i;

  while ((digit = getc(f)) != EOF) {
    arr_append(buffer, digit - '0');
  }

  unsigned int length = buffer->size - 1;

  int sum1 = 0, sum2 = 0;
  for (i = 0; i < length; i++) {
    if (buffer->items[i] == buffer->items[(i + 1) % length]) {
      sum1 += buffer->items[i];
    }
    if (buffer->items[i] == buffer->items[(i + length / 2) % length]) {
      sum2 += buffer->items[i];
    }
  }

  printf("Part 1: %d\n", sum1);
  printf("Part 2: %d\n", sum2);

  fclose(f);
  return 0;
}

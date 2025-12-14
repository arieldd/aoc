#include "basic_vectors.c"
#include "hashmap.h"
#include <stdio.h>

int main() {

  vi arr = vi_init(10);

  for (int i = 0; i < 100; i++) {
    vi_append(&arr, i);
  }

  for (int i = 0; i < arr.count; i++) {
    printf("%d\n", vi_at(&arr, i));
  }
  return 0;
}

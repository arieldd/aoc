#include "../../base/vectors.h"
#include <stdio.h>

DECLARE_VECTOR_OF_TYPE(str, char)

int remove_rolls(str *grid, int size);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Please specify an input file\n");
    return 1;
  }

  int size = 0;
  str grid = str_init(140 * 140);

  FILE *f = fopen(argv[1], "r+");
  int ch;
  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case '@':
      str_append(&grid, 1);
      break;
    case '.':
      str_append(&grid, 0);
      break;
    case '\n':
      size++;
      break;
    }
  }
  fclose(f);

  int p1 = 0, p2 = 0, removed;
  while ((removed = remove_rolls(&grid, size))) {
    if (!p1)
      p1 = removed;
    p2 += removed;
  }

  printf("Part 1: %d\n", p1);
  printf("Part 2: %d\n", p2);

  return 0;
}

typedef struct {
  int i;
  int j;
} Dir;

Dir dir8[] = {{0, 1},  {1, 1},   {1, 0},  {1, -1},
              {0, -1}, {-1, -1}, {-1, 0}, {-1, 1}};

static inline char is_valid_pos(int i, int j, int r, int c) {
  return i >= 0 && j >= 0 && i < r && j < c;
}

int remove_rolls(str *grid, int size) {
  int removed = 0, adj;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (!grid->items[i * size + j])
        continue;

      adj = 0;
      for (int k = 0; k < 8; k++) {
        int ni = i + dir8[k].i, //
            nj = j + dir8[k].j;

        if (!is_valid_pos(ni, nj, size, size) || !grid->items[ni * size + nj])
          continue;

        adj++;
        if (adj >= 4)
          break;
      }
      if (adj < 4) {
        // Mark for removal
        grid->items[i * size + j] = -1;
      }
    }
  }

  // Remove marked
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (grid->items[i * size + j] == -1) {
        grid->items[i * size + j] = 0;
        removed++;
      }
    }
  }
  return removed;
}

#include "../vectors.h"
#include <stdio.h>

#define ll long long

int count_splits(str *grid, int columns, int start, str *visited);
ll count_timelines(str *grid, int columns, int start, vll *cache);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Please specify an input file\n");
    return 1;
  }

  FILE *f = fopen(argv[1], "r+");
  if (f == NULL) {
    printf("Input file not found");
    return 1;
  }

  str grid;
  str_init(&grid, 1000);

  int columns = 0, start = 0, counting_columns = 1;

  int ch;
  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case '\n':
      counting_columns = 0;
      break;
    case 'S':
      start = grid.count;
      // no break we want to fallback to default
    default:
      str_append(&grid, ch);
      if (counting_columns) {
        columns++;
      }
      break;
    }
  }
  fclose(f);

  str visited;
  str_init(&visited, grid.count);
  int p1 = count_splits(&grid, columns, start, &visited);
  str_free(&visited);

  vll cache;
  vll_init(&cache, grid.count);
  ll p2 = count_timelines(&grid, columns, start, &cache);
  vll_free(&cache);

  printf("Part 1: %d\n", p1);
  printf("Part 1: %lld\n", p2);

  str_free(&grid);

  return 0;
}

int count_splits(str *grid, int columns, int start, str *visited) {
  if (visited->items[start])
    return 0;

  visited->items[start] = 1;
  int splits = 0;

  for (;;) {
    start += columns;
    if (start >= grid->count) // Beam exited
      return 0;

    if (grid->items[start] == '^') {
      splits = 1;

      int j = start % columns;
      if (j + 1 < columns) {
        splits += count_splits(grid, columns, start + 1, visited);
      }
      if (j - 1 >= 0) {
        splits += count_splits(grid, columns, start - 1, visited);
      }

      break;
    }

    if (visited->items[start])
      return 0;
    visited->items[start] = 1;
  }
  return splits;
}

ll count_timelines(str *grid, int columns, int start, vll *cache) {
  for (;;) {
    start += columns;
    if (start >= grid->count) // Beam exited
      return 1;

    if (grid->items[start] == '^') {
      if (cache->items[start]) {
        break;
      }

      ll timelines = 0;
      int j = start % columns;
      if (j + 1 < columns) {
        timelines += count_timelines(grid, columns, start + 1, cache);
      }
      if (j - 1 >= 0) {
        timelines += count_timelines(grid, columns, start - 1, cache);
      }

      cache->items[start] = timelines;
      break;
    }
  }
  return cache->items[start];
}

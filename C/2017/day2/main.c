#include "../../base/vectors.h"
#include <stdint.h>
#include <stdio.h>

#define u16 uint16_t
#define u32 uint32_t

DECLARE_VECTOR_OF_TYPE(vi, u16);

void solve(FILE *f, u32 *p1, u32 *p2);

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

  u32 p1 = 0, p2 = 0;
  solve(f, &p1, &p2);
  fclose(f);

  printf("Part 1: %d\n", p1);
  printf("Part 1: %d\n", p2);

  return 0;
}

u32 part1(vi row) {
  u16 min = row.items[0], max = row.items[0];

  for (int i = 1; i < row.count; ++i) {
    if (row.items[i] > max)
      max = row.items[i];
    else if (row.items[i] < min)
      min = row.items[i];
  }
  return max - min;
}

u32 part2(vi row) {
  for (int i = 0; i < row.count; ++i) {
    for (int j = i + 1; j < row.count; ++j) {
      if (row.items[i] % row.items[j] == 0)
        return row.items[i] / row.items[j];

      if (row.items[j] % row.items[i] == 0)
        return row.items[j] / row.items[i];
    }
  }
  return 0;
}

void solve(FILE *f, u32 *p1, u32 *p2) {
  vi row = vi_init(100);

  u16 value = 0;
  int ch, ws = 0;
  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case ' ':
    case '\t':
      if (ws)
        break;
      vi_append(&row, value);
      value = 0;
      ws = 1;
      break;
    case '\n':
      vi_append(&row, value);
      value = 0;
      *p1 += part1(row);
      *p2 += part2(row);
      vi_clear(&row);
      break;
    default:
      value *= 10;
      value += ch - '0';
      ws = 0;
      break;
    }
  }
}

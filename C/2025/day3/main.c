#include "../vectors.h"
#include <memory.h>
#include <stdio.h>

#define ll long long
#define ROW_SIZE 100

ll max_jolt(str *bank);
ll max_jolt_rec(str *bank, int batteries, int index, ll cache[]);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Please specify an input file\n");
    return 1;
  }

  str bank;
  str_init(&bank, ROW_SIZE);

  /*
   * We need a cache by number of batteries and position.
   *
   *                      Position
   *          ---------------------------------
   *          | 0 | 1 | 2 | ... | 98 | 99 |
   *          ---------------------------------
   *          |   |   |   | ... |    |    |
   * Batteries| 0 |   |   | ... |    |    |
   *          ---------------------------------
   *          |   |   |   | ... |    |    |
   *          | . |   |   | ... |    |    |
   *          | . |   |   | ... |    |    |
   *          | . |   |   | ... |    |    |
   *          |   |   |   | ... |    |    |
   *          | 12|   |   | ... |    |    |
   *          ---------------------------------
   */
  ll cache[ROW_SIZE * 13];
  size_t cache_size = sizeof(cache);

  ll p1, p2;

  FILE *f = fopen(argv[1], "r+");
  int ch;
  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case '\n': {
      memset(cache, 0, cache_size);
      ll j1 = max_jolt_rec(&bank, 2, 0, cache);
      memset(cache, 0, cache_size);
      ll j2 = max_jolt_rec(&bank, 12, 0, cache);

      p1 += j1;
      p2 += j2;
      str_clear(&bank);
      break;
    }
    default:
      str_append(&bank, ch - '0');
      break;
    }
  }
  fclose(f);

  printf("Part 1: %lld\n", p1);
  printf("Part 1: %lld\n", p2);

  return 0;
}

static ll pow10[] = {
    1,           10,           100,          1000,      10000,
    100000,      1000000,      10000000,     100000000, 1000000000,
    10000000000, 100000000000, 1000000000000};

ll max_jolt_rec(str *bank, int batteries, int index, ll cache[]) {
  if (!batteries || batteries > bank->count - index) {
    return 0;
  }

  int key = batteries * ROW_SIZE + index;
  if (cache[key]) {
    return cache[key];
  }

  ll jolt_with = (ll)bank->items[index] * pow10[batteries - 1] +
                 max_jolt_rec(bank, batteries - 1, index + 1, cache),
     jolt_without = max_jolt_rec(bank, batteries, index + 1, cache);

  cache[key] = (jolt_with > jolt_without) ? jolt_with : jolt_without;

  return cache[key];
}

// Keeping it around for the record :D
ll max_jolt(str *bank) {
  int m1 = bank->items[0], m2 = bank->items[1], i1 = 0, i2 = 1;
  for (int i = 1; i < bank->count; i++) {
    int current = bank->items[i];
    if (current > m1) {
      m2 = m1;
      i2 = i1;
      m1 = current;
      i1 = i;
    } else if (current > m2) {
      m2 = current;
      i2 = i;
    }
  }
  // Find largest to the right
  if (i1 < bank->count - 1) {
    m2 = bank->items[i1 + 1], i2 = i1 + 1;
    for (int i = i1 + 1; i < bank->count; i++) {
      int current = bank->items[i];
      if (current > m2) {
        m2 = current;
        i2 = i;
      }
    }
  }
  return (i1 < i2) ? m1 * 10 + m2 : m2 * 10 + m1;
}

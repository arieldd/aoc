#include "../vectors.h"
#include <memory.h>
#include <stdio.h>

#define ll long long

ll max_jolt(str *bank);
ll max_jolt_rec(str *bank, int batteries, int index, ll cache[]);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Please specify an input file\n");
    return 1;
  }

  str bank;
  str_init(&bank, 100);

  ll cache[10000];

  ll p1 = 0, p2;

  FILE *f = fopen(argv[1], "r+");
  int ch;
  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case '\n': {
      for (int i = 0; i < bank.count; i++) {
        printf("%d", bank.items[i]);
      }
      printf("\n");
      memset(cache, 0, 10000 * sizeof(ll));
      ll j1 = max_jolt_rec(&bank, 2, 0, &cache);
      memset(cache, 0, 1200 * sizeof(ll));
      ll j2 = max_jolt_rec(&bank, 12, 0, &cache);

      printf("j1: %lld, j2: %lld\n", j1, j2);
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

// Keeping it around for the record :D
ll max_jolt(str *bank) {
  int m1 = str_at(bank, 0), m2 = str_at(bank, 1), i1 = 0, i2 = 1;
  for (int i = 1; i < bank->count; i++) {
    int current = str_at(bank, i);
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
    m2 = str_at(bank, i1 + 1), i2 = i1 + 1;
    for (int i = i1 + 1; i < bank->count; i++) {
      int current = str_at(bank, i);
      if (current > m2) {
        m2 = current;
        i2 = i;
      }
    }
  }
  return (i1 < i2) ? m1 * 10 + m2 : m2 * 10 + m1;
}

static ll pow10[] = {
    1,           10,           100,          1000,      10000,
    100000,      1000000,      10000000,     100000000, 1000000000,
    10000000000, 100000000000, 1000000000000};

ll max_jolt_rec(str *bank, int batteries, int index, ll cache[]) {
  int key = batteries * 100 + index;
  // printf("On %d with %d batteries key is %d\n", index, batteries, key);
  if (cache[key])
    return cache[key];

  if (!batteries || batteries > bank->count - index) {
    cache[key] = 0;
    return 0;
  }

  ll jolt_with = (ll)bank->items[index] * pow10[batteries - 1] +
                 max_jolt_rec(bank, batteries - 1, index + 1, cache),
     jolt_without = max_jolt_rec(bank, batteries, index + 1, cache);

  cache[key] = (jolt_with > jolt_without) ? jolt_with : jolt_without;

  return cache[key];
}

#include "../vectors.h"
#include <stdio.h>

#define ll long long
#define bool char

typedef struct {
  ll low;
  ll high;
} Range;

static inline bool cmp_r(Range r1, Range r2) {
  return r1.low == r2.low && r1.high == r2.high;
}

DECLARE_VECTOR_TYPE(Range, vr, cmp_r)

int count_fresh(vr ranges, vll ids);
ll count_fresh_ranges(vr *ranges);
void merge_overlapping_ranges(vr *ranges);
bool overlaps(Range r1, Range r2);
Range merge(Range r1, Range r2);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Please specify an input file\n");
    return 1;
  }

  vr ranges;
  vr_init(&ranges, 200);

  vll ids;
  vll_init(&ids, 1000);

  ll value = 0;
  char read_ids = 0;
  Range current;

  FILE *f = fopen(argv[1], "r+");
  int ch, last;
  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case '\n':
      if (!read_ids) {
        if (last == '\n') {
          read_ids = 1;
          break;
        }
        current.high = value;
        vr_append(&ranges, current);
      } else {
        vll_append(&ids, value);
      }

      value = 0;
      break;
    case '-':
      current.low = value;
      value = 0;
      break;
    default:
      value *= 10;
      value += ch - '0';
      break;
    }
    last = ch;
  }
  fclose(f);

  ll p1 = count_fresh(ranges, ids), //
      p2 = count_fresh_ranges(&ranges);
  printf("Part 1: %lld\n", p1);
  printf("Part 2: %lld\n", p2);

  vll_free(&ids);
  vr_free(&ranges);
  return 0;
}

int count_fresh(vr ranges, vll ids) {
  int fresh = 0;

  for (int i = 0; i < ids.count; i++) {
    ll id = ids.items[i];
    for (int r = 0; r < ranges.count; r++) {
      Range current = ranges.items[r];
      if (current.low <= id && current.high >= id) {
        fresh++;
        break;
      }
    }
  }
  return fresh;
}

ll count_fresh_ranges(vr *ranges) {
  ll fresh = 0;

  merge_overlapping_ranges(ranges);
  for (int i = 0; i < ranges->count; i++) {
    Range r = ranges->items[i];
    fresh += r.high - r.low + 1;
  }

  return fresh;
}

void merge_overlapping_ranges(vr *ranges) {
  for (int i = 0; i < ranges->count; i++) {
    Range r1 = ranges->items[i], //
        new = {-1, -1};
    for (int j = i + 1; j < ranges->count; j++) {
      Range r2 = ranges->items[j];
      if (overlaps(r1, r2)) {
        new = merge(r1, r2);
        vr_remove_at(ranges, j);
        break;
      }
    }
    if (new.low != -1) {
      ranges->items[i] = new;
      i--; // Check this range again since is a new one
    }
  }
}

bool overlaps(Range r1, Range r2) {
  Range lower = (r1.low < r2.low) ? r1 : r2,
        higher = (r1.high > r2.high) ? r1 : r2;

  return cmp_r(lower, higher) // contained
         || (lower.high >= higher.low);
}

Range merge(Range r1, Range r2) {
  ll low = (r1.low < r2.low) ? r1.low : r2.low;
  ll high = (r1.high > r2.high) ? r1.high : r2.high;
  return (Range){low, high};
}

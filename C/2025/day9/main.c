#include "../vectors.h"
#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define bool char

typedef struct {
  long x, y;
  int pos;
} Point;

static inline char eq_point(Point a, Point b) {
  return a.x == b.x && a.y == b.y;
}

DECLARE_VECTOR_TYPE(Point, vp, eq_point)

static inline long rect_area(Point a, Point b) {
  return (labs(a.x - b.x) + 1) * (labs(a.y - b.y) + 1);
}

// Can we draw a straight line without intersecting the pattern
bool straight_line(vp *tiles, Point from, Point to);
bool is_valid_rect(vp *tiles, Point a, Point b);

void solve(vp *tiles, long *p1, long *p2);

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

  vp tiles;
  vp_init(&tiles, 100);

  int ch;
  long value = 0;
  Point next;
  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case ',':
      next.x = value;
      value = 0;
      break;
    case '\n':
      next.y = value;
      next.pos = tiles.count;
      vp_append(&tiles, next);
      value = 0;
      break;
    default:
      value *= 10;
      value += ch - '0';
      break;
    }
  }
  fclose(f);

  long p1 = 0, p2 = 0;
  solve(&tiles, &p1, &p2);
  printf("Part 1: %ld\n", p1);
  printf("Part 2: %ld\n", p2);

  vp_free(&tiles);

  return 0;
}

void solve(vp *tiles, long *p1, long *p2) {
  for (int i = 0; i < tiles->count - 1; i++) {
    Point a = tiles->items[i];
    for (int j = i + 1; j < tiles->count - 1; j++) {
      Point b = tiles->items[j];

      long area = rect_area(a, b);
      *p1 = MAX(*p1, area);
      if (is_valid_rect(tiles, a, b)) {
        *p2 = MAX(*p2, area);
      }
    }
  }
}

bool is_valid_rect(vp *tiles, Point a, Point b) {
  // Find corners
  Point c = (Point){a.x, b.y}, //
      d = (Point){b.x, a.y};

  if (!straight_line(tiles, a, c) || !straight_line(tiles, a, d) ||
      !straight_line(tiles, b, c) || !straight_line(tiles, b, d))
    return 0;

  return 1;
}

bool straight_line(vp *tiles, Point from, Point to) {
  long min_x = MIN(from.x, to.x), //
      min_y = MIN(from.y, to.y),  //
      max_x = MAX(from.x, to.x),  //
      max_y = MAX(from.y, to.y);

  for (int i = 0; i < tiles->count; i++) {
    Point p1 = tiles->items[i], p2 = tiles->items[(i + 1) % tiles->count];
    if (from.x == to.x) {
      if (p1.x == p2.x)
        continue; // parallel
      if ((min_y < p1.y && p1.y < max_y) &&
          ((p1.x <= from.x && from.x <= p2.x) ||
           (p2.x <= from.x && from.x <= p1.x)))
        return 0;
    } else {
      if (p1.y == p2.y)
        continue; // parallel
      if ((min_x < p1.x && p1.x < max_x) &&
          ((p1.y <= from.y && from.y <= p2.y) ||
           (p2.y <= from.y && from.y <= p1.y)))
        return 0;
    }
  }
  return 1;
}

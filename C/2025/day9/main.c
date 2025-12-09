#include "../vectors.h"
#include <stdio.h>
#include <stdlib.h>

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define bool char

typedef struct {
  long x, y;
} Point;

static inline char eq_point(Point a, Point b) {
  return a.x == b.x && a.y == b.y;
}

DECLARE_VECTOR_TYPE(Point, vp, eq_point)

typedef struct {
  bool *grid;
  long rows, columns, y_offset, x_offset;
} Floor;

void floor_set(Floor *map, long x, long y, bool value) {
  map->grid[(y - map->y_offset) * map->columns + (x - map->x_offset)] = value;
}

bool floor_at(Floor *map, long x, long y) {
  return map->grid[(y - map->y_offset) * map->columns + (x - map->x_offset)];
}

static inline long rect_area(Point a, Point b) {
  return (labs(a.x - b.x) + 1) * (labs(a.y - b.y) + 1);
}

void bounding_rect(vp *tiles, Point bounds[2]);
void fill_map(vp *red_tiles, Floor *map);
bool is_valid_rect(Floor *map, Point a, Point b);
// bool is_in_shape(Floor *map, Point p);
bool contains(Floor *map, Point p);

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

void draw_perimeter(Floor *map) {
  for (int y = 0; y < map->rows; y++) {
    for (int x = 0; x < map->columns; x++) {
      if (contains(map, (Point){x, y}))
        printf("X");
      else
        printf(".");
    }

    printf("\n");
  }
}

void solve(vp *tiles, long *p1, long *p2) {
  Point bounds[2] = {0};
  bounding_rect(tiles, bounds);
  long bounding_area = rect_area(bounds[0], bounds[1]);

  Floor map = {0};
  map.grid = calloc(bounding_area, sizeof(bool));
  map.rows = bounds[1].y - bounds[0].y + 1;
  map.columns = bounds[1].x - bounds[0].x + 1;
  map.y_offset = bounds[0].y;
  map.x_offset = bounds[0].x;

  fill_map(tiles, &map);
  // draw_perimeter(&map);

  for (int i = 0; i < tiles->count - 1; i++) {
    Point a = tiles->items[i];
    for (int j = i + 1; j < tiles->count - 1; j++) {
      Point b = tiles->items[j];

      long area = rect_area(a, b);
      // printf("have area %ld\n", rg_area);
      *p1 = MAX(*p1, area);
      if (is_valid_rect(&map, a, b)) {
        *p2 = MAX(*p2, area);
      }
    }
  }
}

bool is_valid_rect(Floor *map, Point a, Point b) {
  // Find corners
  Point c = (Point){a.x, b.y}, //
      d = (Point){b.x, a.y};

  // a to c
  for(int y = MIN(a.y, b.y); y <= MAX(a.y, b.y); y++)
        if(!contains(map, (Point){a.x, y}))
            return 0;

  // a to d
  for(int x = MIN(a.x, b.x); x <= MAX(a.x, b.x); x++)
        if(!contains(map, (Point){x, a.y}))
            return 0;
  

  // b to d
  for(int y = MIN(a.y, b.y); y <= MAX(a.y, b.y); y++)
        if(!contains(map, (Point){b.x, y}))
            return 0;

  // b to c
  for(int x = MIN(a.x, b.x); x <= MAX(a.x, b.x); x++)
        if(!contains(map, (Point){x, b.y}))
            return 0;

  return 1;
}

bool contains(Floor *map, Point p) { return floor_at(map, p.x, p.y); }

void fill_map(vp *red_tiles, Floor *map) {
  // Perimeter
  for (int i = 0; i < red_tiles->count; i++) {
    Point a = red_tiles->items[i];
    Point b = red_tiles->items[(i + 1) % red_tiles->count];
    if (a.x == b.x) {
      for (long y = MIN(a.y, b.y); y <= MAX(a.y, b.y); y++) {
        floor_set(map, a.x, y, 1);
      }
    } else {
      for (long x = MIN(a.x, b.x); x <= MAX(a.x, b.x); x++) {
        floor_set(map, x, a.y, 1);
      }
    }
  }

  for (int i = 2; i < map->rows - 1; i++) {
    bool in = 0;
    for (int j = 0; j < map->columns; j++) {
      bool tile = map->grid[i * map->columns + j];
      bool valid = tile;
      while (tile) {
        j++;
        tile = map->grid[i * map->columns + j];
      }

      if (valid) {
        in = !in;
        j--;
      } else if (!valid && in) {
        map->grid[i * map->columns + j] = 1;
      }
    }
  }
}

void bounding_rect(vp *tiles, Point bounds[2]) {
  Point *top_left = &bounds[0], *bottom_right = &bounds[1];
  for (int i = 1; i < tiles->count; i++) {
    Point next = tiles->items[i];
    top_left->x = MIN(next.x, top_left->x);
    top_left->y = MIN(next.y, top_left->y);

    bottom_right->x = MAX(next.x, bottom_right->x);
    bottom_right->y = MAX(next.y, bottom_right->y);
  }
}

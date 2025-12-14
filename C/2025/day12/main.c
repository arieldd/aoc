#include "../../base/vectors.h"
#include <stdio.h>

#define bool char
DECLARE_VECTOR_OF_TYPE(vi, int)
DECLARE_VECTOR_OF_TYPE(str, char)

typedef struct {
  int index, area;
  str grid;
} Shape;

typedef struct {
  int width, length, pos;
  vi presents;
} Region;

DECLARE_VECTOR_OF_TYPE(vs, Shape);
DECLARE_VECTOR_OF_TYPE(vr, Region);

void print_shape(Shape *s);
void print_region(Region *r);
void set_area(Shape *s);
Shape rotate(Shape *s);

int count_fits(vr *regions, vs *shapes);
bool can_fit(Region *r, vs *shapes);
bool try_place(str *grid, vs *shapes, vi remaining, int index);

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

  vs shapes = vs_init(5);

  vr regions = vr_init(100);

  int ch, read_shapes = 1, value = 0, last;
  Shape shape;
  Region region;
  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case '\n':
      if (read_shapes) {
        if (last == '\n') {
          set_area(&shape);
          vs_append(&shapes, shape);
        }
      } else {
        vi_append(&region.presents, value);
        vr_append(&regions, region);
      }
      value = 0;
      break;
    case ':':
      if (read_shapes) {
        shape.index = value;
        shape.grid = str_init(9);
      } else {
        region.length = value;
        region.presents = vi_init(10);
        value = -1;
      }
      break;
    case 'x':
      if (read_shapes)
        read_shapes = 0;
      region.width = value;
      value = 0;
      break;
    case ' ':
      if (value >= 0)
        vi_append(&region.presents, value);
      value = 0;
      break;
    case '#':
      str_append(&shape.grid, 1);
      break;
    case '.':
      str_append(&shape.grid, 0);
      break;
    default:
      value *= 10;
      value += ch - '0';
      break;
    }
    last = ch;
  }
  fclose(f);

  int p1 = count_fits(&regions, &shapes);
  printf("Part 1: %d\n", p1);

  return 0;
}

int count_fits(vr *regions, vs *shapes) {
  int count = 0;
  for (int i = 0; i < regions->count; i++) {
    count += can_fit(&regions->items[i], shapes);
  }
  return count;
}

bool can_fit(Region *r, vs *shapes) {
  int region_area = r->width * r->length, //
      min_area = 0,                       //
      required;
  for (int i = 0; i < r->presents.count; i++) {
    required = r->presents.items[i] * shapes->items[i].area;
    min_area += required;
  }
  if (min_area > region_area)
    return 0;
  return 1;
}

bool try_place(str *grid, vs *shapes, vi remaining, int index) {
  int rotations = 0;
  return 0;
}

Shape rotate(Shape *s) {
  Shape rotated;
  rotated.index = s->index;
  rotated.area = s->area;
  rotated.grid = str_init(9);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      rotated.grid.items[3 * (2 - j) + i] = s->grid.items[3 * i + j];
    }
  }
  return rotated;
}

void set_area(Shape *s) {
  s->area = 0;
  for (int i = 0; i < 9; i++) {
    s->area += s->grid.items[i];
  }
}

void print_shape(Shape *s) {
  printf("%d: area=%d\n", s->index, s->area);
  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      char symbol = s->grid.items[i * 3 + j];
      printf("%c", symbol ? '#' : '.');
    }
    printf("\n");
  }
  printf("\n");
}

void print_region(Region *r) {
  printf("%dx%d:", r->width, r->length);
  for (int i = 0; i < r->presents.count; i++) {
    printf(" %d", r->presents.items[i]);
  }
  printf("\n");
}

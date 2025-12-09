#include "../vectors.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  long x;
  long y;
  long z;
  // For connecting
  int pos;
  long set;
} JBox;

static inline char eq_j(JBox j1, JBox j2) {
  return j1.x == j2.x && j1.y == j2.y && j1.z == j2.z;
}

static inline double dist(JBox j1, JBox j2) {
  double                                  //
      x2 = (j1.x - j2.x) * (j1.x - j2.x), //
      y2 = (j1.y - j2.y) * (j1.y - j2.y), //
      z2 = (j1.z - j2.z) * (j1.z - j2.z);
  return sqrt(x2 + y2 + z2);
}

DECLARE_VECTOR_TYPE(JBox, vj, eq_j)

typedef struct {
  int i1, i2;
  double dist;
} Conn;

static inline char eq_conn(Conn c1, Conn c2) {
  return (c1.i1 == c2.i1 && c1.i2 == c2.i2) ||
         (c1.i1 == c2.i2 && c1.i2 == c2.i1);
}
DECLARE_VECTOR_TYPE(Conn, vcon, eq_conn)

int cmp_size(const void *s1, const void *s2) {
  return (*(long *)s2) - (*(long *)s1);
}

int cmp_conn(const void *c1, const void *c2) {
  return ((Conn *)c1)->dist - ((Conn *)c2)->dist;
}

void solve(vj *boxes, long *p1, long *p2);

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

  vj boxes;
  vj_init(&boxes, 1000);

  int ch, index = 0, value = 0;
  JBox next;

  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case '\n':
      next.z = value;
      next.pos = boxes.count;
      next.set = -1;
      vj_append(&boxes, next);
      index = 0;
      value = 0;
      break;
    case ',':
      if (index == 0)
        next.x = value;
      else if (index == 1)
        next.y = value;
      index++;
      value = 0;
      break;
    default:
      value *= 10;
      value += ch - '0';
      break;
    }
  }
  fclose(f);

  long p1, p2;
  solve(&boxes, &p1, &p2);
  printf("Part 1: %ld\n", p1);
  printf("Part 2: %ld\n", p2);

  vj_free(&boxes);
  return 0;
}

void solve(vj *boxes, long *p1, long *p2) {
  // Calculate distances and sort connections ascending
  vcon connections;
  vcon_init(&connections, 1000);
  for (int i = 0; i < boxes->count - 1; i++) {
    for (int j = i + 1; j < boxes->count; j++) {
      JBox j1 = boxes->items[i], j2 = boxes->items[j];
      vcon_append(&connections,
                  (Conn){.i1 = j1.pos, .i2 = j2.pos, .dist = dist(j1, j2)});
    }
  }
  qsort(connections.items, connections.count, sizeof(Conn), cmp_conn);

  long set_id = 0;
  for (int i = 0;; i++) {
    Conn next = connections.items[i];
    JBox *j1 = &boxes->items[next.i1], *j2 = &boxes->items[next.i2];
    if (j1->set == -1 && j2->set != -1) {
      j1->set = j2->set;
    } else if (j1->set != -1 && j2->set == -1) {
      j2->set = j1->set;
    } else {
      set_id++;
      if (j1->set != j2->set) {
        long s1 = j1->set, s2 = j2->set;
        for (int i = 0; i < boxes->count; i++) {
          if (boxes->items[i].set == s1 || boxes->items[i].set == s2) {
            boxes->items[i].set = set_id;
          }
        }
      } else if (j1->set == -1 && j2->set == -1) {
        j1->set = set_id;
        j2->set = set_id;
      }
    }

    if (i == 999) { // Part 1 1000 connections
      long *sizes = calloc(set_id + 1, sizeof(long));
      for (int k = 0; k < boxes->count; k++) {
        JBox *current = &boxes->items[k];
        if (current->set != -1)
          sizes[current->set]++;
      }

      qsort(sizes, set_id + 1, sizeof(long), cmp_size);
      *p1 = sizes[0] * sizes[1] * sizes[2];
      free(sizes);
    }

    char full_set = 1;
    long last_set = boxes->items[0].set;
    for (int i = 1; i < boxes->count; i++) {
      if (boxes->items[i].set != last_set) {
        full_set = 0;
        break;
      }
    }
    if (full_set) {
      *p2 = j1->x * j2->x;
      return;
    }
  }
}

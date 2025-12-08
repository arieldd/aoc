#include "../vectors.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct {
  long x;
  long y;
  long z;
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
  JBox j1;
  JBox j2;
  double dist;
} Conn;

static inline char eq_conn(Conn c1, Conn c2) {
  return (eq_j(c1.j1, c2.j1) && eq_j(c1.j2, c2.j2)) ||
         (eq_j(c1.j1, c2.j2) && eq_j(c1.j2, c2.j1));
}

int cmp_conn(const void *c1, const void *c2) {
  return ((Conn *)c1)->dist - ((Conn *)c2)->dist;
}

DECLARE_VECTOR_TYPE(Conn, vcon, eq_conn)

static inline char eq_vvj(vj c1, vj c2) { return c1.items == c2.items; };
DECLARE_VECTOR_TYPE(vj, vvj, eq_vvj);

int cmp_circuit(const void *c1, const void *c2) {
  return ((vvj *)c2)->count - ((vvj *)c1)->count; // Descending
}

char contains(vj *circuit, JBox *box) {
  for (int i = 0; i < circuit->count; i++) {
    if (eq_j(circuit->items[i], *box))
      return 1;
  }
  return 0;
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

  long p1;
  long p2;
  solve(&boxes, &p1, &p2);
  printf("Part 1: %ld\n", p1);
  printf("Part 2: %ld\n", p2);

  vj_free(&boxes);
  return 0;
}

void print_circuit(vj *c) {
  printf("{");
  for (int i = 0; i < c->count; i++) {
    JBox box = c->items[i];
    printf(" {%ld, %ld, %ld} ", box.x, box.y, box.z);
  }
  printf("} : %zu\n", c->count);
}

void merge_circuits(vvj *circuits) {
  for (int i = 0; i < circuits->count; i++) {
    vj *c1 = &circuits->items[i];
    char merged = 0;
    for (int j = i + 1; j < circuits->count; j++) {
      vj *c2 = &circuits->items[j];
      for (int k = 0; k < c2->count; k++) {
        if (contains(c1, &c2->items[k])) {
          for (int l = 0; l < c2->count; l++) {
            if (!contains(c1, &c2->items[l])) {
              vj_append(c1, c2->items[l]);
            }
          }
          vvj_remove_at(circuits, j);

          merged = 1;
          break;
        }
      }
      if (merged)
        break;
    }
    if (merged)
      i--; // Check this one again
  }
}

void solve(vj *boxes, long *p1, long *p2) {
  // Calculate distances and sort connections ascending
  vcon connections;
  vcon_init(&connections, 1000);
  for (int i = 0; i < boxes->count - 1; i++) {
    for (int j = i + 1; j < boxes->count; j++) {
      JBox j1 = boxes->items[i], j2 = boxes->items[j];
      vcon_append(&connections,
                  (Conn){.j1 = j1, .j2 = j2, .dist = dist(j1, j2)});
    }
  }
  qsort(connections.items, connections.count, sizeof(Conn), cmp_conn);

  // Build circuits and merge them
  vvj circuits;
  vvj_init(&circuits, connections.count);
  for (int i = 0;; i++) {
    Conn next = connections.items[i];
    char found = 0;
    for (int j = 0; j < circuits.count; j++) {
      vj *circuit = &circuits.items[j];
      char c1 = contains(circuit, &next.j1), c2 = contains(circuit, &next.j2);
      if (c1 && !c2) {
        vj_append(circuit, next.j2);
        found++;
        break;
      }

      if (c2 && !c1) {
        vj_append(circuit, next.j1);
        found++;
      }

      if (c1 && c2) {
        found = 1;
        break;
      }

      if (found > 1)
        break;
    }
    if (!found) {
      vj new_circuit;
      vj_init(&new_circuit, 100);
      vj_append(&new_circuit, next.j1);
      vj_append(&new_circuit, next.j2);

      vvj_append(&circuits, new_circuit);
    }
    merge_circuits(&circuits);
    if (i == 999 /* P1 1000 cycles */) {
      qsort(circuits.items, circuits.count, sizeof(vj), cmp_circuit);
      *p1 = circuits.items[0].count * circuits.items[1].count *
            circuits.items[2].count;
    }
    if (circuits.items[0].count == boxes->count) {
      *p2 = next.j1.x * next.j2.x;
      break;
    }
  }

  for (int i = 0; i < circuits.count; i++) {
    vj_free(&circuits.items[i]);
  }

  vvj_free(&circuits);
  vcon_free(&connections);
}

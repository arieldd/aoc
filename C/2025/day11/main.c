#include "../hashmap.h"
#include "../vectors.h"
#include <stdio.h>
#include <stdlib.h>

#define ll long long

static inline char cmp_tuple(char *a, char *b) {
  return 0;
} // Dummy for the macro
DECLARE_VECTOR_TYPE(char *, vtuple, cmp_tuple)

typedef struct Node {
  char label[3];
  vtuple outputs;
  ll paths, trouble_paths;
} Node;

Node *create_node() {
  Node *node = malloc(sizeof(Node));
  vtuple_init(&node->outputs, 10);
  node->paths = -1;
  node->trouble_paths = -1;
  return node;
}

static inline int eq_key(void *a, void *b) {
  char *k1 = (char *)a, *k2 = (char *)b;
  for (int i = 0; i < 3; i++) {
    if (k1[i] != k2[i])
      return 0;
  }
  return 1;
}

static inline long str_hash(void *k) {
  char *key = (char *)k;
  long hashed = 5381;

  int c;
  for (int i = 0; i < 3; i++) {
    int c = key[i];
    hashed = ((hashed << 5) + hashed) + c; /* hashedh * 33 + c */
  }

  return hashed;
}

ll count_paths(HashMap *graph, char *from);
ll count_trouble_paths(HashMap *graph, char *from, char dac, char fft);

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

  HashMap *graph = hm_create(500, eq_key, str_hash);
  Node *current = create_node();

  str value;
  str_init(&value, 3);

  int ch, read_label = 1, index = 0;
  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case '\n':
      vtuple_append(&current->outputs, value.items);
      hm_insert(graph, &current->label, current);

      str_init(&value, 3);
      current = create_node();
      read_label = 1;
      break;
    case ':':
      for (int i = 0; i < 3; i++) {
        current->label[i] = value.items[i];
      }
      str_init(&value, 3);
      break;
    case ' ':
      if (read_label) // Already added in ':'
        read_label = 0;
      else {
        vtuple_append(&current->outputs, value.items);
        str_init(&value, 3);
      }
      break;
    default:
      str_append(&value, ch);
      break;
    }
  }
  fclose(f);

  ll p1 = count_paths(graph, "you"), //
      p2 = count_trouble_paths(graph, "svr", 0, 0);
  printf("Part 1: %lld\n", p1);
  printf("Part 2: %lld\n", p2);

  return 0;
}

ll count_paths(HashMap *graph, char *from) {
  if (eq_key(from, "out"))
    return 1;

  Node *node = hm_at(graph, from);
  if (!node)
    return 0; // This label goes nowhere

  if (node->paths != -1)
    return node->paths;

  long paths = 0;
  for (int i = 0; i < node->outputs.count; i++) {
    paths += count_paths(graph, node->outputs.items[i]);
  }

  node->paths = paths;
  return paths;
}

ll count_trouble_paths(HashMap *graph, char *from, char dac, char fft) {
  if (eq_key(from, "out")) {
    return 1 && dac && fft;
  }

  if (eq_key(from, "dac"))
    dac = 1;

  if (eq_key(from, "fft"))
    fft = 1;

  Node *node = hm_at(graph, from);
  if (!node)
    return 0; // This label goes nowhere

  if (node->trouble_paths != -1)
    return node->trouble_paths;

  ll paths = 0;
  for (int i = 0; i < node->outputs.count; i++) {
    char *next = node->outputs.items[i];
    paths += count_trouble_paths(graph, next, dac, fft);
  }

  printf("%s has %lld\n", from, paths);
  node->trouble_paths = paths;
  return paths;
}

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
} Node;

Node *create_node() {
  Node *node = malloc(sizeof(Node));
  vtuple_init(&node->outputs, 10);

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

ll count_paths(HashMap *graph, char *from, char *to, HashMap *cache);
ll count_trouble_paths(HashMap *graph);

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

  HashMap *cache = hm_create(graph->size, eq_key, str_hash);
  ll p1 = count_paths(graph, "you", "out", cache);
  ll p2 = count_trouble_paths(graph);

  printf("Part 1: %lld\n", p1);
  printf("Part 2: %lld\n", p2);

  free(cache);
  free(graph);
  return 0;
}

ll count_paths(HashMap *graph, char *current, char *to, HashMap *cache) {
  if (eq_key(current, to)) {
    return 1;
  }
  Node *node = hm_at(graph, current);
  if (!node)
    return 0; // This label goes nowhere

  if (hm_contains(cache, current)) {
    ll cached = *((ll *)hm_at(cache, current));
    return cached;
  }

  ll *paths = malloc(sizeof(ll));
  *paths = 0;
  for (int i = 0; i < node->outputs.count; i++) {
    char *label = node->outputs.items[i];
    *paths += count_paths(graph, label, to, cache);
  }

  hm_insert(cache, current, paths);
  return *paths;
}

ll count_trouble_paths(HashMap *graph) {
  HashMap *cache = hm_create(graph->size, eq_key, str_hash);
  ll svr_dac = count_paths(graph, "svr", "dac", cache);

  hm_clear(cache);
  ll svr_fft = count_paths(graph, "svr", "fft", cache);

  hm_clear(cache);
  ll fft_dac = count_paths(graph, "fft", "dac", cache);

  hm_clear(cache);
  ll dac_fft = count_paths(graph, "dac", "fft", cache);

  hm_clear(cache);
  ll dac_out = count_paths(graph, "dac", "out", cache);

  hm_clear(cache);
  ll fft_out = count_paths(graph, "fft", "out", cache);

  return svr_dac * dac_fft * fft_out + svr_fft * fft_dac * dac_out;
}

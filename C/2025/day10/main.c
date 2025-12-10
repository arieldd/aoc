#include "../vectors.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>

#define ll long long
#define bool char

#define MIN(x, y) ((x) < (y)) ? (x) : (y)

static inline char eq_switch(vi *a, vi *b) {
  if (a->count != b->count)
    return 0;
  for (int i = 0; i < a->count; i++) {
    if (a->items[i] != b->items[i])
      return 0;
  }
  return 1;
}

DECLARE_VECTOR_TYPE(vi *, vvi, eq_switch)

typedef struct {
  str *lights;
  vvi *buttons;
  vi *joltage;
  int pos;
} Machine;

static inline char eq_machine(Machine a, Machine b) { return a.pos == b.pos; }

DECLARE_VECTOR_TYPE(Machine, vm, eq_machine)

void print_machine(Machine *m);
void configure(Machine *m, int *lights, int *jolts);
int configure_lights(Machine *m, str state, int index, int presses);
bool compare_lights(str *target, str *current);

int configure_joltage(Machine *m, vi state, int index, int presses);
bool compare_joltage(vi *target, vi *current);
bool can_press(vi *target, vi *current, vi *button);

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

  vm schematics;
  vm_init(&schematics, 100);

  Machine current = {0};
  bool read_joltage = 0;
  int ch, value = 0;

  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case '\n':
      current.pos = schematics.count;
      vm_append(&schematics, current);
      memset(&current, 0, sizeof(current));
      read_joltage = 0;
      break;

    case '[': {
      current.lights = malloc(sizeof(str));
      str_init(current.lights, 10);
      break;
    }

    case '(':
      if (current.buttons == NULL) {
        current.buttons = malloc(sizeof(vvi));
        vvi_init(current.buttons, 10);
      }
      vi *button = malloc(sizeof(vi));
      vi_init(button, 10);
      vvi_append(current.buttons, button);
      break;

    case '{':
      read_joltage = 1;
      current.joltage = malloc(sizeof(vi));
      vi_init(current.joltage, 10);
      break;

    // Ignore
    case ']':
      break;
    case ' ':
      break;

    case ',':
      if (!read_joltage) {
        vi_append(current.buttons->items[current.buttons->count - 1], value);
      } else {
        vi_append(current.joltage, value);
      }
      value = 0;
      break;

    case ')':
      vi_append(current.buttons->items[current.buttons->count - 1], value);
      value = 0;
      break;
    case '}':
      vi_append(current.joltage, value);
      value = 0;
      break;

    case '.':
      str_append(current.lights, 0);
      break;
    case '#':
      str_append(current.lights, 1);
      break;

    default:
      value *= 10;
      value += ch - '0';
      break;
    }
  }
  fclose(f);

  ll p1 = 0, p2 = 0;
  for (int i = 0; i < schematics.count; i++) {
    int lights, jolt;
    configure(&schematics.items[i], &lights, &jolt);
    printf("Configured %d/%zu with %d and %d\n", i + 1, schematics.count,
           lights, jolt);
    p1 += lights;
    p2 += jolt;
  }

  printf("Part 1: %lld\n", p1);
  printf("Part 1: %lld\n", p2);

  vm_free(&schematics);

  return 0;
}

void print_machine(Machine *m) {
  printf("[");
  for (int i = 0; i < m->lights->count; i++) {
    char light = m->lights->items[i];
    printf("%c", light ? '#' : '.');
  }
  printf("] ");

  for (int i = 0; i < m->buttons->count; i++) {
    printf("(");
    vi *button = m->buttons->items[i];
    for (int j = 0; j < button->count; j++) {
      printf("%d,", button->items[j]);
    }
    printf(") ");
  }

  printf("{");
  for (int i = 0; i < m->joltage->count; i++) {
    int jolt = m->joltage->items[i];
    printf("%d,", jolt);
  }
  printf("}");
  printf("\n");
}

bool compare_lights(str *target, str *current) {
  for (int i = 0; i < target->count; i++) {
    if (target->items[i] != current->items[i])
      return 0;
  }
  return 1;
}

bool compare_joltage(vi *target, vi *current) {
  for (int i = 0; i < target->count; i++) {
    if (target->items[i] != current->items[i])
      return 0;
  }
  return 1;
}

bool can_press(vi *target, vi *current, vi *button) {
  for (int i = 0; i < button->count; i++) {
    int pos = button->items[i];
    if (target->items[pos] <= current->items[pos])
      return 0;
  }
  return 1;
}

void configure(Machine *m, int *lights, int *jolts) {
  str current_lights;
  str_init(&current_lights, m->lights->count);
  current_lights.count = current_lights.capacity;

  *lights = configure_lights(m, current_lights, 0, 0);

  vi current_joltage;
  vi_init(&current_joltage, m->joltage->count);
  current_joltage.count = current_joltage.capacity;

  *jolts = configure_joltage(m, current_joltage, 0, 0);

  str_free(&current_lights);
  vi_free(&current_joltage);
}

int configure_lights(Machine *m, str state, int index, int presses) {
  if (compare_lights(m->lights, &state)) {
    return presses;
  }

  if (presses == m->buttons->count || index >= m->buttons->count)
    return INT32_MAX;

  int not_pressing = configure_lights(m, state, index + 1, presses);

  str new_state;
  str_init(&new_state, state.count);

  for (int i = 0; i < state.count; i++) {
    str_append(&new_state, state.items[i]);
  }
  vi *button = m->buttons->items[index];
  for (int i = 0; i < button->count; i++) {
    int pos = button->items[i];
    new_state.items[pos] = !new_state.items[pos];
  }

  int pressing = configure_lights(m, new_state, index + 1, presses + 1);

  str_free(&new_state);
  return MIN(not_pressing, pressing);
}

int configure_joltage(Machine *m, vi state, int index, int presses) {
  if (compare_joltage(m->joltage, &state)) {
    return presses;
  }

  if (index >= m->buttons->count)
    return INT32_MAX;

  int not_pressing = configure_joltage(m, state, index + 1, presses);

  vi *button = m->buttons->items[index];

  vi new_state;
  vi_init(&new_state, state.count);

  for (int i = 0; i < state.count; i++) {
    vi_append(&new_state, state.items[i]);
  }

  int pressing = INT32_MAX;
  for (; can_press(m->joltage, &new_state, button); presses++) {
    // printf("Pressing %d times\n", presses);
    for (int i = 0; i < button->count; i++) {
      int pos = button->items[i];
      new_state.items[pos] += 1;
    }

    pressing =
        MIN(configure_joltage(m, new_state, index + 1, presses + 1), pressing);
  }

  vi_free(&new_state);
  return MIN(not_pressing, pressing);
}

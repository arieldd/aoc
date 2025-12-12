#include "../vectors.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ll long long
#define bool char

#define MIN(x, y) ((x) < (y)) ? (x) : (y)

static inline char eq_vi(vi *a, vi *b) {
  if (a->count != b->count)
    return 0;
  for (int i = 0; i < a->count; i++) {
    if (a->items[i] != b->items[i])
      return 0;
  }
  return 1;
}

DECLARE_VECTOR_TYPE(vi *, vvi, eq_vi)

typedef struct {
  str *lights;
  vvi *buttons;
  vi *joltage;
  int pos;
} Machine;

static inline char eq_machine(Machine a, Machine b) { return a.pos == b.pos; }

static inline int cmp_vi(const void *a, const void *b) { // For sorting buttons
  const vi **v1 = (const vi **)a, **v2 = (const vi **)b;
  return (*v2)->count - (*v1)->count;
}

DECLARE_VECTOR_TYPE(Machine, vm, eq_machine)

int eq_key(void *a, void *b) { return eq_vi((vi *)a, (vi *)b); }

void print_machine(Machine *m);
void configure(Machine *m, int *lights, int *jolts);
int configure_lights(Machine *m, str state, int index, int presses);
int configure_joltage(Machine *m, vi *state, int index, int presses, int *best);
bool compare_lights(str *target, str *current);
char compare_joltage(vi *target, vi *current);
int max_press(vi *target, vi *current, vi *button);

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

  // Parse input
  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case '\n':
      current.pos = schematics.count;
      qsort(current.buttons->items, current.buttons->count, sizeof(vi *),
            &cmp_vi);
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
    int lights, jolts;
    configure(&schematics.items[i], &lights, &jolts);
    printf("Configured %d with %d, %d\n", schematics.items[i].pos + 1, lights,
           jolts);
    assert(jolts != INT32_MAX);
    p1 += lights;
    p2 += jolts;
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

char compare_joltage(vi *target, vi *current) {
  for (int i = 0; i < target->count; i++) {
    if (target->items[i] != current->items[i]) {
      return current->items[i] - target->items[i];
    }
  }
  return 0;
}

int max_press(vi *target, vi *current, vi *button) {

  int max_presses = INT32_MAX;
  for (int i = 0; i < button->count; i++) {
    int pos = button->items[i];
    if (target->items[pos] <= current->items[pos])
      return 0;
    max_presses = MIN(target->items[pos] - current->items[pos], max_presses);
  }

  return max_presses;
}

void configure(Machine *m, int *lights, int *jolts) {
  str current_lights;
  str_init(&current_lights, m->lights->count);
  current_lights.count = current_lights.capacity;

  *lights = configure_lights(m, current_lights, 0, 0);

  vi current_joltage;
  vi_init(&current_joltage, m->joltage->count);
  current_joltage.count = current_joltage.capacity;

  *jolts = INT32_MAX;
  configure_joltage(m, &current_joltage, 0, 0, jolts);

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

bool can_configure(Machine *m, vi *state, int from_button) {
  if (from_button == 0)
    return 1;

  int *slots = calloc(state->count, sizeof(int));
  for (int i = from_button; i < m->buttons->count; i++) {
    vi *button = m->buttons->items[i];
    for (int j = 0; j < button->count; j++) {
      slots[button->items[j]] = 1;
    }
  }
  for (int i = 0; i < state->count; i++) {
    if (slots[i] || state->items[i] == m->joltage->items[i])
      continue;

    free(slots);
    return 0;
  }
  free(slots);
  return 1;
}

int configure_joltage(Machine *m, vi *state, int index, int presses,
                      int *best) {
  char check = compare_joltage(m->joltage, state);
  if (check == 0) {
    *best = MIN(*best, presses);
    return 0;
  }

  if (!can_configure(m, state, index))
    return -1;

  vi *button = m->buttons->items[index];

  int button_presses = max_press(m->joltage, state, button);
  button_presses = MIN(*best - presses, button_presses);

  int times_pressed = INT32_MAX, result = 1;
  for (; button_presses >= 0; button_presses--) {
    if (presses + button_presses > *best)
      continue;

    for (int i = 0; i < button->count; i++) {
      int pos = button->items[i];
      state->items[pos] += button_presses;
    }

    int attempt =
        configure_joltage(m, state, index + 1, presses + button_presses, best);

    for (int i = 0; i < button->count; i++) {
      int pos = button->items[i];
      state->items[pos] -= button_presses;
    }

    if (attempt < 0)
      break; // Won't succeed with less presses
    if (attempt == 0) {
      result = 0;
    }
  }

  return result;
}

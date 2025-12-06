#include "../vectors.h"
#include <stdio.h>

#define ll long long
#define bool char

ll part1(vll *numbers, str *operations, int rows);

ll part2(vll *numbers, str *operations);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Please specify an input file\n");
    return 1;
  }

  vll p1_numbers;
  vll_init(&p1_numbers, 1000);
  vll p2_numbers;
  vll_init(&p2_numbers, 1000);

  str operations;
  str_init(&operations, 100);

  bool reading_numbers = 1;
  ll current = 0;

  FILE *f = fopen(argv[1], "r+");
  int ch, rows = 0, column = 0;
  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case ' ':
      if (reading_numbers) {
        // p1
        if (current) {
          vll_append(&p1_numbers, current);
          current = 0;
        }
        // p2
        if (p2_numbers.count <= column)
          vll_append(&p2_numbers, 0);
        column++;
      }
      break;
    case '\n':
      if (reading_numbers) {
        vll_append(&p1_numbers, current); // read last number (no space after)
        current = 0;
        rows++;
        column = 0;
      }
      break;
    default:
      if (ch < '0' || ch > '9') {
        reading_numbers = 0;
      }
      if (reading_numbers) {
        // p1
        current *= 10;
        current += ch - '0';

        // p2
        if (p2_numbers.count <= column)
          vll_append(&p2_numbers, 0);
        p2_numbers.items[column] *= 10;
        p2_numbers.items[column++] += ch - '0';
      } else {
        str_append(&operations, ch);
      }
      break;
    }
  }
  fclose(f);

  ll p1 = part1(&p1_numbers, &operations, rows), //
      p2 = part2(&p2_numbers, &operations);
  printf("Part 1: %lld\n", p1);
  printf("Part 1: %lld\n", p2);

  str_free(&operations);
  vll_free(&p2_numbers);
  vll_free(&p1_numbers);
  return 0;
}

ll part1(vll *numbers, str *operations, int rows) {
  ll total = 0;
  int columns = operations->count;
  for (int j = 0; j < columns; j++) {
    ll result = numbers->items[0 * columns + j];
    char op = operations->items[j];
    for (int i = 1; i < rows; i++) {
      ll num = numbers->items[i * columns + j];
      result = (op == '+') ? result + num : result * num;
    }
    total += result;
  }
  return total;
}

ll part2(vll *numbers, str *operations) {
  ll total = 0;
  int index = 0;
  for (int i = 0; i < operations->count; i++) {
    char op = operations->items[i];
    ll result = numbers->items[index], num;
    while ((num = numbers->items[++index])) {
      result = (op == '+') ? result + num : result * num;
    }
    index++;
    total += result;
  }
  return total;
}

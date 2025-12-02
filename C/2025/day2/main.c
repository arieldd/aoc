#include <stdio.h>

#define ll long long

ll add_invalids(ll lower, ll upper, char (*is_invalid)(ll));

char is_invalid_1(ll value);
char is_invalid_2(ll value);

int to_digits(ll value, char *buffer);
ll from_digits(char *buffer, int length);

int main(int argc, char *argv[]) {
  if (argc < 2) {
    printf("Please specify an input file\n");
    return 1;
  }

  ll p1 = 0, p2 = 0, lower, upper, value = 0;

  FILE *f = fopen(argv[1], "r+");
  int ch;
  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case '-':
      lower = value;
      value = 0;
      break;
    case ',':
    case '\n': {
      upper = value;
      value = 0;
      p1 += add_invalids(lower, upper, is_invalid_1);
      p2 += add_invalids(lower, upper, is_invalid_2);
      break;
    }
    default:
      value *= 10;
      value += ch - '0';
      break;
    }
  }

  fclose(f);

  printf("Part 1: %lld\n", p1);
  printf("Part 1: %lld\n", p2);

  return 0;
}

ll add_invalids(ll lower, ll upper, char (*is_invalid)(ll)) {
  ll invalids = 0;
  for (ll current = lower; current <= upper; current++) {
    if (is_invalid(current)) {
      invalids += current;
    }
  }
  return invalids;
}

char is_invalid_1(ll value) {
  char buffer[20];
  int length = to_digits(value, buffer);
  if (length % 2)
    return 0;
  int half = length / 2;

  for (int i = 0; i < half; i++) {
    if (buffer[length - 1 - i] != buffer[half - 1 - i])
      return 0;
  }
  return 1;
}

char is_invalid_2(ll value) {
  char buffer[20];
  int length = to_digits(value, buffer), half = length / 2;

  int size = 1;
  while (size <= half) {
    ll pattern = from_digits(buffer + length - size, size);

    char invalid = 1;
    for (int i = size; i < length; i += size) {
      if (i + size > length) {
        invalid = 0;
        break;
      }
      ll next = from_digits(buffer + length - size - i, size);
      if (pattern != next) {
        invalid = 0;
        break;
      }
    }
    if (invalid) {
      return 1;
    }

    size++;
  }

  return 0;
}

int to_digits(ll value, char *buffer) {
  int index = 0;
  while (value) {
    buffer[index++] = value % 10;
    value /= 10;
  }
  buffer[index] = '\0';
  return index;
}

ll from_digits(char *buffer, int length) {
  ll number = 0;
  while (length) {
    number *= 10;
    number += buffer[--length];
  }
  return number;
}

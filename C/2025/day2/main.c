#include <stdio.h>

#define ll long long

void solve(ll lower, ll upper, ll *p1, ll *p2);
char is_invalid(ll value, char digits[], int length, int size);

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
      solve(lower, upper, &p1, &p2);
      value = 0;
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

static const int pow10[] = {1, 10, 100, 1000, 10000, 100000, 1000000, 10000000};
void solve(ll lower, ll upper, ll *p1, ll *p2) {
  char digits[20];
  int length;

  for (ll current = lower; current <= upper; current++) {
    length = to_digits(current, digits);
    int half = length / 2;
    if (length % 2 == 0 && current / pow10[half] == current % pow10[half]) {
        *p1 += current;
    }

    if (is_invalid(current, digits, length, 1)) {
      *p2 += current;
    }
  }
}

char is_invalid(ll valuee, char digits[], int length, int size) {
  int half = length / 2;

  while (size <= half) {
    ll pattern = from_digits(digits + length - size, size);

    char invalid = 1;
    for (int i = size; i < length; i += size) {
      if (i + size > length) {
        invalid = 0;
        break;
      }
      ll next = from_digits(digits + length - size - i, size);
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

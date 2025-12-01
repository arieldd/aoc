#include <stdio.h>

int main(int argc, char *argv[]) {

  if (argc < 2) {
    printf("Please specify an input file \n");
    return 1;
  }

  int dial = 50, p1 = 0, p2 = 0, dir = 1, move = 0;

  FILE *f = fopen(argv[1], "r+");
  int ch;
  while ((ch = getc(f)) != EOF) {
    switch (ch) {
    case 'R': {
      dir = 1;
      break;
    }
    case 'L': {
      dir = -1;
      break;
    }
    case '\n': {
      int div = move / 100, rem = move % 100;
      p2 += div;

      if (dial && ((dir == 1 && rem > 100 - dial) || (dir == -1 && rem > dial)))
        p2++;

      dial = (dial + rem * dir) % 100;

      if (dial < 0) // Take positive modulo
        dial += 100;

      if (dial == 0)
        p1++;

      move = 0;
      break;
    }
    default: {
      move *= 10;
      move += ch - '0';
      break;
    }
    }
  }
  printf("Part 1: %d\n", p1);
  printf("Part 2: %d\n", p1 + p2);

  return 0;
}

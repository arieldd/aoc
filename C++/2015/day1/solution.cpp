#include "utils.h"

using namespace std;
using namespace aoc_utils;

int part1(const vector<string> &seq) {
  auto floor = 0;
  for (auto &line : seq) {
    for (auto &c : line) {
      floor += (c == '(') ? 1 : -1;
    }
  }
  return floor;
}

int part2(const vector<string> &seq) {
  int floor = 0, pos = 0;
  for (auto &line : seq) {
    for (auto &c : line) {
      pos++;
      floor += (c == '(') ? 1 : -1;
      if (floor < 0)
        return pos;
    }
  }
  return pos;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);

  auto r1 = part1(lines);
  println("Part 1: ", r1);

  auto r2 = part2(lines);
  println("Part 2: ", r2);
  return 0;
}
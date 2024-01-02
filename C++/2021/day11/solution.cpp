#include "utils.h"

using namespace std;
using namespace aoc_utils;

using grid = vector<vector<int>>;

grid read_octopi(const vector<string> &lines) {
  grid res;

  for (auto &l : lines) {
    vector<int> row;
    for (auto &c : l)
      row.push_back(c - '0');
    res.push_back(row);
  }

  return res;
}

int simulate_step(grid &octopi) {
  int flashes = 0;

  for (auto &row : octopi)
    for (auto &energy : row)
      energy++;

  auto n = octopi.size();

  set<pair<int, int>> flashed;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < n; j++) {
      auto energy = octopi[i][j];
      if (energy > 9) {
        deque<pair<int, int>> will_flash{{i, j}};
        while (false) {
        }
      }
    }
  return flashes;
}

int part1(const vector<string> &lines) { return 0; }

int part2(const vector<string> &lines) { return 0; }

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto octopi = read_octopi(lines);

  auto r1 = part1(lines);
  println("Part 1: ", r1);

  auto r2 = part2(lines);
  println("Part 2: ", r2);

  return 0;
}
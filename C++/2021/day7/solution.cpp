#include "utils.h"

using namespace std;
using namespace aoc_utils;

vector<int> read_crabs(const vector<string> &lines) {
  vector<int> res;

  for (auto &l : lines) {
    auto timers = split(l, ',');

    for (auto &t : timers)
      res.push_back(stoi(t));
  }

  return res;
}

int solve(const vector<int> &positions, bool part1) {
  auto min_pos = *min_element(positions.begin(), positions.end());
  auto max_pos = *max_element(positions.begin(), positions.end());

  auto best = INT_MAX;
  for (int i = min_pos; i <= max_pos; i++) {
    auto fuel_spent = 0;
    for (auto &p : positions) {
      auto steps = abs(p - i);
      fuel_spent += (part1) ? steps : (steps * (steps + 1)) / 2;
    }
    if (fuel_spent < best)
      best = fuel_spent;
  }

  return best;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto positions = read_crabs(lines);

  auto r1 = solve(positions, true);
  println("Part 1: ", r1);

  auto r2 = solve(positions, false);
  println("Part 2: ", r2);

  return 0;
}
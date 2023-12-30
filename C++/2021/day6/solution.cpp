#include "utils.h"

using namespace std;
using namespace aoc_utils;

vector<int> read_lanternfish(const vector<string> &lines) {
  vector<int> res;

  for (auto &l : lines) {
    auto timers = split(l, ',');

    for (auto &t : timers)
      res.push_back(stoi(t));
  }

  return res;
}

int64_t solve(const vector<int> &timers, int days) {

  int cycle_length = 9;
  vector<int64_t> fish(cycle_length, 0);

  for (auto &t : timers) {
    fish[t]++;
  }

  for (int day = 1; day <= days; day++) {
    auto hatching = fish[0];
    for (int i = 0; i < cycle_length - 1; i++) {
      fish[i] = fish[i + 1];
    }
    fish[8] = hatching;
    fish[6] += hatching;
  }

  return accumulate(fish.begin(), fish.end(), (int64_t)0);
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto timers = read_lanternfish(lines);

  auto r1 = solve(timers, 80);
  println("Part 1: ", r1);

  auto r2 = solve(timers, 256);
  println("Part 2: ", r2);

  return 0;
}
#include "utils.h"
#include <cassert>

using namespace std;
using namespace aoc_utils;

#define ll long long

struct Machine {
  array<int, 2> A;
  array<int, 2> B;
  array<int, 2> P;
};

vector<Machine> read_input(const string &filename) {
  vector<Machine> claws{};
  ifstream fs(filename);

  string line;
  Machine next;
  int row = 0;
  while (getline(fs, line)) {
    if (!line.size()) {
      claws.push_back(next);
      next = {};
      row = 0;
      continue;
    }
    auto values = nums<ll>(line);
    if (row == 0) {
      next.A[0] = values[0];
      next.A[1] = values[1];
    } else if (row == 1) {
      next.B[0] = values[0];
      next.B[1] = values[1];
    } else {
      next.P[0] = values[0];
      next.P[1] = values[1];
    }
    row++;
  }
  claws.push_back(next);
  return claws;
}

int brute_force(const Machine &claw) {
  int best = INT_MAX;
  for (int i = 1; i < 101; i++)
    for (int j = 1; j < 101; j++) {
      if (claw.A[0] * i + claw.B[0] * j == claw.P[0] and
          claw.A[1] * i + claw.B[1] * j == claw.P[1])
        best = min(best, 3 * i + j);
    }
  return best;
}

int part1(const vector<Machine> &claws) {
  auto total = 0;
  for (auto &c : claws) {
    auto tokens = brute_force(c);

    if (tokens < INT_MAX)
      total += tokens;
  }
  return total;
}

ll alg_solve(const Machine &claw, ll offset) {

  array<ll, 2> b = {claw.P[0] + offset, claw.P[1] + offset};
  ll x1 = (b[0] * claw.B[1] - b[1] * claw.B[0]) /
          (claw.B[1] * claw.A[0] - claw.A[1] * claw.B[0]),
     x2 = (b[1] - claw.A[1] * x1) / claw.B[1];

  if (claw.A[0] * x1 + claw.B[0] * x2 != b[0] or
      claw.A[1] * x1 + claw.B[1] * x2 != b[1])
    return 0;

  return 3 * x1 + x2;
}

ll solve(const vector<Machine> &claws, ll offset) {
  ll total = 0;
  for (auto &c : claws) {
    auto tokens = alg_solve(c, offset);
    total += tokens;
  }
  return total;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto claws = read_input(argv[1]);
  cout << "Part 1:" << solve(claws, 0) << '\n';
  cout << "Part 2:" << solve(claws, 10000000000000) << '\n';
  return 0;
}

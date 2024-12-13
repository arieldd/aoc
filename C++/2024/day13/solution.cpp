#include "z3++.h"
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>

using namespace std;
using namespace z3;

#define ll long long
#define point pair<ll, ll>

struct Machine {
  point buttonA;
  point buttonB;
  point prize;
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
    auto plus_x = line.find("+");
    auto comma = line.find(", ");
    auto plus_y = line.find("+", comma + 2);
    if (row == 0) {
      next.buttonA.first = stoll(line.substr(plus_x + 1, comma - plus_x));
      next.buttonA.second = stoll(line.substr(plus_y + 1, line.size()));
    } else if (row == 1) {
      next.buttonB.first = stoll(line.substr(plus_x + 1, comma - plus_x));
      next.buttonB.second = stoll(line.substr(plus_y + 1, line.size()));
    } else {
      auto eq_x = line.find('=');
      auto eq_y = line.find('=', eq_x + 1);
      next.prize.first = stoll(line.substr(eq_x + 1, comma - eq_x));
      next.prize.second = stoll(line.substr(eq_y + 1, line.size()));
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
      if (claw.buttonA.first * i + claw.buttonB.first * j ==
              claw.prize.first and
          claw.buttonA.second * i + claw.buttonB.second * j ==
              claw.prize.second)
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

ll z3_solve(const Machine &claw, ll offset) {
  context c;

  expr x = c.int_const("x");
  expr y = c.int_const("y");
  solver s(c);

  expr eq1 =
      c.int_val(claw.buttonA.first) * x + c.int_val(claw.buttonB.first) * y ==
      c.int_val(claw.prize.first + offset);
  expr eq2 =
      c.int_val(claw.buttonA.second) * x + c.int_val(claw.buttonB.second) * y ==
      c.int_val(claw.prize.second + offset);

  s.add(eq1);
  s.add(eq2);
  s.add(x > 0);
  s.add(y > 0);

  switch (s.check()) {
  case unsat:
  case unknown:
    return 0;
  case sat:
    return s.get_model().eval(3 * x + y).as_uint64();
  }
}

ll alg_solve(const Machine &claw, ll offset) {

  point b = {claw.prize.first + offset, claw.prize.second + offset};
  ll x1 = (b.first * claw.buttonB.second - b.second * claw.buttonB.first) /
          (claw.buttonB.second * claw.buttonA.first -
           claw.buttonA.second * claw.buttonB.first),
     x2 = (b.second - claw.buttonA.second * x1) / claw.buttonB.second;

  if (claw.buttonA.first * x1 + claw.buttonB.first * x2 != b.first or
      claw.buttonA.second * x1 + claw.buttonB.second * x2 != b.second)
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

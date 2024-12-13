#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <vector>
using namespace std;

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

ll explore(const Machine claw, int tokens, int pressA, int pressB, point pos,
           map<tuple<point, int, int, int>, ll> &cache) {
  if (pressA > 100 or pressB > 100)
    return INT64_MAX;

  if (pos == claw.prize)
    return tokens;

  if (cache.contains({pos, tokens, pressA, pressB}))
    return cache.at({pos, tokens, pressA, pressB});

  point next = {pos.first + claw.buttonA.first,
                pos.second + claw.buttonA.second};
  auto tokensA = explore(claw, tokens + 3, pressA + 1, pressB, next, cache);
  next = {pos.first + claw.buttonB.first, pos.second + claw.buttonB.second};
  auto tokensB = explore(claw, tokens + 1, pressA, pressB + 1, next, cache);

  auto min_tokens = min(tokensA, tokensB);
  cache[{pos, tokens, pressA, pressB}] = min_tokens;
  return min_tokens;
}

int part1(const vector<Machine> &claws) {
  auto total = 0;
  for (auto &c : claws) {
    map<tuple<point, int, int, int>, ll> cache;
    auto tokens = explore(c, 0, 0, 0, {0, 0}, cache);

    if (tokens < INT64_MAX)
      total += tokens;
  }
  return total;
}

int part2(const vector<Machine> &claws) { return 0; }

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto claws = read_input(argv[1]);
  cout << "Part 1:" << part1(claws) << '\n';
  cout << "Part 2:" << part2(claws) << '\n';
  return 0;
}

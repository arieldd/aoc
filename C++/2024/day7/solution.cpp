#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#define ll long long

struct Calibration {
  ll result;
  vector<ll> operands;
};

vector<Calibration> read_input(const string &filename) {
  vector<Calibration> lines{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line)) {
    Calibration next;
    auto sep = line.find(": ");
    next.result = stoll(line.substr(0, sep));

    stringstream ss(line.substr(sep + 2, line.size()));
    string value;
    while (getline(ss, value, ' '))
      next.operands.push_back(stoll(value));
    lines.push_back(next);
  }
  return lines;
}

ll concatenate(ll v1, ll v2) {
  ll digits = 0, appending = v2;
  while (v2 > 0) {
    v2 /= 10;
    digits++;
  }

  return v1 * pow(10, digits) + appending;
}

bool is_possible(const Calibration &c, ll total, ll index, bool use_concat) {
  if (index >= c.operands.size())
    return total == c.result;
  if (total > c.result)
    return false;

  auto value = c.operands[index];

  bool result = is_possible(c, total + value, index + 1, use_concat) or
                is_possible(c, total * value, index + 1, use_concat);

  return (use_concat) ? result or is_possible(c, concatenate(total, value),
                                              index + 1, true)
                      : result;
}

pair<ll, ll> solve(const vector<Calibration> &lines) {
  ll part1 = 0, part2 = 0;
  for (const auto &calibration : lines) {
    if (is_possible(calibration, calibration.operands[0], 1, false)) {
      part1 += calibration.result;
      part2 += calibration.result;
    } else if (is_possible(calibration, calibration.operands[0], 1, true)) {
      part2 += calibration.result;
    }
  }
  return {part1, part2};
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto lines = read_input(argv[1]);
  auto solutions = solve(lines);
  cout << "Part 1:" << solutions.first << '\n';
  cout << "Part 2:" << solutions.second << '\n';
  return 0;
}

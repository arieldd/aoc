#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

#include "utils.h"

using namespace std;

tuple<int, int, int> get_dimensions(const string &line) {
  int w, l, h;

  auto sep = line.find('x');
  w = stoi(line.substr(0, sep));

  auto rest = line.substr(sep + 1, string::npos);
  sep = rest.find('x');

  l = stoi(rest.substr(0, sep));
  h = stoi(rest.substr(sep + 1, string::npos));

  return {w, l, h};
}

vector<tuple<int, int, int>> parse_input(const string &file_name) {
  vector<std::string> ret;

  ifstream fs(file_name);
  string line;
  while (getline(fs, line)) {
    ret.push_back(line);
  }

  vector<tuple<int, int, int>> result;
  for (auto &line : ret) {
    result.push_back(get_dimensions(line));
  }
  return result;
}

int part1(const vector<tuple<int, int, int>> &boxes) {
  auto total = 0;
  for (auto &box : boxes) {
    auto w = get<0>(box), l = get<1>(box), h = get<2>(box);
    auto needed = 2 * (w * h + w * l + h * l);
    needed += min({w * h, w * l, h * l});

    total += needed;
  }
  return total;
}

int part2(const vector<tuple<int, int, int>> &boxes) {
  auto total = 0;
  for (auto &box : boxes) {
    auto w = get<0>(box), l = get<1>(box), h = get<2>(box);
    auto needed = min({2 * w + 2 * h, 2 * w + 2 * l, 2 * l + 2 * h});
    needed += w * h * l;
    total += needed;
  }
  return total;
}

int main() {
  auto boxes = parse_input("input.txt");
  auto r1 = part1(boxes);
  println("Part 1: ", r1);
  auto r2 = part2(boxes);
  println("Part 2: ", r2);
  return 0;
}
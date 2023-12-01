#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

#include "..\utils.h"

using namespace std;

vector<string> parse_input(const string &file_name) {
  vector<std::string> ret;

  ifstream fs(file_name);
  string line;
  while (getline(fs, line)) {
    ret.push_back(line);
  }
  return ret;
}

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

int main() {
  auto lines = parse_input("input.txt");
  // auto lines = parse_input("test.txt");
  auto r1 = part1(lines);
  println("Part 1: ", r1);
  auto r2 = part2(lines);
  println("Part 2: ", r2);
  return 0;
}
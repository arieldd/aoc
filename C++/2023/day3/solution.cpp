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

vector<string> parse_input(const string &file_name) {
  vector<string> ret;

  ifstream fs(file_name);
  string line;
  while (getline(fs, line)) {
    ret.push_back(line);
  }
  return ret;
}

int part1() { return 0; }

int part2() { return 0; }

int main() {
  auto lines = parse_input("input.txt");
  // auto lines = parse_input("test.txt");
  auto r1 = part1();
  println("Part 1: ", r1);
  auto r2 = part2();
  println("Part 2: ", r2);
  return 0;
}
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

struct interval_pair_t {
  int x1;
  int x2;
  int y1;
  int y2;
};

using pair_list_t = std::vector<interval_pair_t>;

pair_list_t parse_input(const std::string &file_name) {
  pair_list_t ret;

  std::ifstream fs(file_name);
  std::string line;
  while (std::getline(fs, line)) {
    interval_pair_t pair;

    sscanf_s(line.c_str(), "%d-%d,%d-%d", &pair.x1, &pair.x2, &pair.y1,
             &pair.y2);

    ret.push_back(pair);
  }
  return ret;
}

int part1(const pair_list_t &pairs) {

  int fully_overlapping = 0;
  for (auto p : pairs) {
    if ((p.x1 - p.y1) * (p.x2 - p.y2) <= 0)
      fully_overlapping += 1;
  }

  return fully_overlapping;
}

int part2(const pair_list_t &pairs) {
  int fully_overlapping = 0;
  for (auto p : pairs) {
    if ((p.x1 - p.y2) * (p.y1 - p.x2) >= 0)
      fully_overlapping += 1;
  }

  return fully_overlapping;
}

int main() {

  auto pairs = parse_input("input.txt");
  // auto pairs = parse_input("test.txt");

  auto r1 = part1(pairs);
  std::cout << "Part 1: " << r1 << std::endl;

  auto r2 = part2(pairs);
  std::cout << "Part 2: " << r2 << std::endl;
  return 0;
}
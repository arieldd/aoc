#include "..\utils.h"
#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

using namespace std;
std::vector<std::string> parse_input(const std::string &file_name) {
  std::vector<std::string> ret;

  std::ifstream fs(file_name);
  std::string line;
  while (std::getline(fs, line)) {
    ret.push_back(line);
  }
  return ret;cd 
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
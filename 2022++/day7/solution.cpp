#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

std::string parse_input(const std::string &file_name) {
  std::string ret;

  std::ifstream fs(file_name);
  std::string line;
  while (std::getline(fs, line)) {
    ret = line;
  }
  return ret;
}

int part1(const std::string &data) { return 0; }

int part2(std::string data) { return 0; }

int main() {

  auto data = parse_input("input.txt");
  // auto data = parse_input("test.txt");

  auto r1 = part1(data);
  std::cout << "Part 1: " << r1 << std::endl;

  auto r2 = part2(data);
  std::cout << "Part 2: " << r2 << std::endl;
  return 0;
}
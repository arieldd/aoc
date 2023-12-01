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
  return ret;
}

const map<string, int> numbers = {{"one", 1},   {"two", 2},   {"three", 3},
                                  {"four", 4},  {"five", 5},  {"six", 6},
                                  {"seven", 7}, {"eight", 8}, {"nine", 9}};

tuple<int, int> first_last_digit(const string &line, bool letters = false) {
  vector<int> digits{};

  for (int i = 0; i < line.length(); i++) {
    if (isdigit(line[i])) {
      digits.push_back(line[i] - '0');
    } else if (letters) {
      for (auto l : {3, 4, 5}) { // possible string lengths
        auto substr = line.substr(i, l);
        if (auto it = numbers.find(substr); it != numbers.end()) {
          digits.push_back(it->second);
        }
      }
    }
  }
  return {digits.front(), digits.back()};
}

int part1(const vector<string> &lines) {
  auto sum = 0;
  for (auto &line : lines) {
    auto pair = first_last_digit(line);
    sum += get<0>(pair) * 10 + get<1>(pair);
  }
  return sum;
}

int part2(const vector<string> &lines) {
  auto sum = 0;
  for (auto &line : lines) {
    auto pair = first_last_digit(line, true);
    sum += get<0>(pair) * 10 + get<1>(pair);
  }
  return sum;
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
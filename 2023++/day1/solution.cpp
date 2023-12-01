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

int calibration_value(const string &line) {

  vector<int> digits{};
  for (auto &c : line) {
    if (isdigit(c)) {
      digits.push_back(c - '0');
    }
  }
  // println(line, " ", digits.front() * 10 + digits.back());
  return digits.front() * 10 + digits.back();
}

int part1(const vector<string> &lines) {
  auto sum = 0;
  for (auto &line : lines) {
    sum += calibration_value(line);
  }
  return sum;
}

int part2(const vector<string> &lines) {
  map<string, string> numbers = {{"one", "1"},   {"two", "2"},   {"three", "3"},
                                 {"four", "4"},  {"five", "5"},  {"six", "6"},
                                 {"seven", "7"}, {"eight", "8"}, {"nine", "9"}};

  vector<int> lenghts{3, 4, 5};

  auto sum = 0;
  for (const auto &line : lines) {
    auto str = line;

    for (int i = 0; i < str.length(); i++) {
      if (isdigit(str[i]))
        break;

      for (auto l : lenghts) {
        auto substr = str.substr(i, l);
        if (numbers.find(substr) != numbers.end()) {
          str.replace(i, l, numbers[substr]);
          i = str.length();
          break;
        }
      }
    }

    for (int i = str.length(); i >= 0; i--) {
      for (auto l : lenghts) {
        if (i < l) {
          i = 0;
          break;
        }

        auto substr = str.substr(i - l, l);
        if (numbers.find(substr) != numbers.end()) {
          str.replace(i - l, l, numbers[substr]);
          i = 0;
          break;
        }
      }
    }
    sum += calibration_value(str);
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
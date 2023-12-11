#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <deque>
#include <numeric>
#include <set>
#include <sstream>
#include <string>
#include <vector>

#pragma region Print
template <typename... Args> void print(Args &&...args) {
  (std::cout << ... << args);
}

template <typename... Args> void println(Args &&...args) {
  print(std::forward<Args>(args)...);

  std::cout << std::endl;
}
#pragma endregion

#pragma region String utils
const char *ws = " \t\n\r\f\v";

// trim from end of string (right)
inline std::string &rtrim(std::string &s, const char *t = ws) {
  s.erase(s.find_last_not_of(t) + 1);
  return s;
}

// trim from beginning of string (left)
inline std::string &ltrim(std::string &s, const char *t = ws) {
  s.erase(0, s.find_first_not_of(t));
  return s;
}

// trim from both ends of string (right then left)
inline std::string &trim(std::string &s, const char *t = ws) {
  return ltrim(rtrim(s, t), t);
}

std::vector<std::string> split(const std::string &str, char delim) {
  std::vector<std::string> result;
  std::stringstream ss(str);
  std::string element;

  while (getline(ss, element, delim)) {
    result.push_back(trim(element));
  }

  return result;
}
#pragma endregion

#pragma region Grid utils

bool is_valid_pos(int i, int j, int r, int c) {
  return i >= 0 && j >= 0 && i < r && j < c;
}

// Start from the right clockwise
const std::vector<int> dy{0, 1, 1, 1, 0, -1, -1, -1},
    dx{1, 1, 0, -1, -1, -1, 0, 1};

#pragma endregion

#pragma region Math
int ipow(int base, int exp) {
  int result = 1;
  for (;;) {
    if (exp & 1)
      result *= base;
    exp >>= 1;
    if (!exp)
      break;
    base *= base;
  }

  return result;
}
#pragma endregion
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

struct Part {
  int number;
  int row_start;
};

using schematic = vector<vector<Part>>;

schematic parse_map(const vector<string> &lines) {
  schematic result;

  for (auto &line : lines) {
    vector<Part> row;
    for (int i = 0; i < line.length(); i++) {
      auto c = line[i];

      if (c == '.') {
        row.push_back({-1, 0});
      } else if (c >= '0' && c <= '9') {
        auto value = c - '0';

        auto j = i + 1;
        for (; j < line.length() && (line[j] >= '0' && line[j] <= '9'); j++) {
          value *= 10;
          value += line[j] - '0';
        }

        for (auto k = i; k < j; k++)
          row.push_back({value, i});

        i = j - 1;

      } else if (c == '*') {
        row.push_back({-2, 0});
      } else {
        row.push_back({-3, 0});
      }
    }

    result.push_back(row);
  }

  return result;
}

void print_schematic(const schematic &map) {
  for (auto &row : map) {
    for (auto elem : row) {
      if (elem.number < 0)
        print(elem.number, " ");
      else
        print("(", elem.number, ",", elem.row_start, ") ");
    }
    println();
  }
}

vector<string> parse_input(const string &file_name) {
  vector<string> ret;

  ifstream fs(file_name);
  string line;
  while (getline(fs, line)) {
    ret.push_back(line);
  }
  return ret;
}

bool is_valid_pos(int i, int j, int r, int c) {
  return i >= 0 && j >= 0 && i < r && j < c;
}

int find_digits(int number) {
  int result = 1;

  while (number / 10) {
    result++;
    number /= 10;
  }

  return result;
}

const vector<int> dy{0, 0, 1, 1, 1, -1, -1, -1}, dx{-1, 1, -1, 0, 1, -1, 0, 1};

int part1(const schematic &map) {
  vector<int> parts;

  int r = map.size();
  for (int i = 0; i < r; i++) {
    auto row = map[i];
    int c = row.size();
    for (int j = 0; j < c; j++) {
      auto elem = row[j];
      if (elem.number <= -2) { // symbol
        for (auto k = 0; k < 2; k++) {
          auto di = i + dy[k], dj = j + dx[k];

          if (is_valid_pos(di, dj, r, c) && map[di][dj].number > 0) {
            auto part = map[di][dj];

            parts.push_back(part.number);
          }
        }
        for (auto k = 2; k < 5; k++) {
          auto di = i + dy[k], dj = j + dx[k];

          if (is_valid_pos(di, dj, r, c) && map[di][dj].number > 0) {
            auto part = map[di][dj];

            parts.push_back(part.number);
            auto digits = find_digits(part.number);

            auto delta = digits - (dj - part.row_start + 1);

            k += delta;
          }
        }
        for (auto k = 5; k < 8; k++) {
          auto di = i + dy[k], dj = j + dx[k];

          if (is_valid_pos(di, dj, r, c) && map[di][dj].number > 0) {
            auto part = map[di][dj];

            parts.push_back(part.number);

            auto digits = find_digits(part.number);

            auto delta = digits - (dj - part.row_start + 1);

            k += delta;
          }
        }
      }
    }
  }

  return accumulate(parts.begin(), parts.end(), 0);
}

int part2(const schematic &map) {
  vector<int> ratios;

  int r = map.size();
  for (int i = 0; i < r; i++) {
    auto row = map[i];
    int c = row.size();
    for (int j = 0; j < c; j++) {

      auto elem = row[j];
      if (elem.number == -2) { // symbol *
        vector<int> adjacents;

        for (auto k = 0; k < 2; k++) {
          auto di = i + dy[k], dj = j + dx[k];

          if (is_valid_pos(di, dj, r, c) && map[di][dj].number > 0) {
            auto part = map[di][dj];

            adjacents.push_back(part.number);
          }
        }
        for (auto k = 2; k < 5; k++) {
          auto di = i + dy[k], dj = j + dx[k];

          if (is_valid_pos(di, dj, r, c) && map[di][dj].number > 0) {
            auto part = map[di][dj];

            adjacents.push_back(part.number);
            auto digits = find_digits(part.number);

            auto delta = digits - (dj - part.row_start + 1);

            k += delta;
          }
        }
        for (auto k = 5; k < 8; k++) {
          auto di = i + dy[k], dj = j + dx[k];

          if (is_valid_pos(di, dj, r, c) && map[di][dj].number > 0) {
            auto part = map[di][dj];

            adjacents.push_back(part.number);

            auto digits = find_digits(part.number);

            auto delta = digits - (dj - part.row_start + 1);

            k += delta;
          }
        }

        if (adjacents.size() == 2) {
          ratios.push_back(adjacents[0] * adjacents[1]);
        }
      }
    }
  }

  return accumulate(ratios.begin(), ratios.end(), 0);
}

int main() {
  auto lines = parse_input("input.txt");
  // auto lines = parse_input("test.txt");

  auto map = parse_map(lines);
  auto r1 = part1(map);
  println("Part 1: ", r1);
  auto r2 = part2(map);
  println("Part 2: ", r2);
  return 0;
}
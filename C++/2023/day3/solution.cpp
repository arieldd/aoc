#include "utils.h"

using namespace std;
using namespace aoc_utils;

struct Part {
  int number;
  int i;
  int j;
};

const bool operator<(const Part &lhs, const Part &rhs) {
  return (lhs.i < rhs.i) || (!(rhs.i < lhs.i) && lhs.j < rhs.j);
}

using schematic = vector<vector<Part>>;

schematic parse_map(const vector<string> &lines) {
  schematic result;

  for (int i = 0; i < lines.size(); i++) {
    auto line = lines[i];

    vector<Part> row;
    for (int j = 0; j < line.length(); j++) {
      auto c = line[j];

      if (c == '.') {
        row.push_back({-1, i, j});

      } else if (c >= '0' && c <= '9') {
        auto value = c - '0';

        auto k = j + 1;
        for (; k < line.length() && (line[k] >= '0' && line[k] <= '9'); k++) {
          value *= 10;
          value += line[k] - '0';
        }

        for (auto l = j; l < k; l++)
          row.push_back({value, i, j});

        j = k - 1;

      } else if (c == '*') {
        row.push_back({-2, i, j});
      } else {
        row.push_back({-3, i, j});
      }
    }

    result.push_back(row);
  }

  return result;
}

int part1(const schematic &map) {
  int total = 0;

  int r = map.size();
  for (int i = 0; i < r; i++) {
    auto row = map[i];
    int c = row.size();
    for (int j = 0; j < c; j++) {
      auto elem = row[j];
      if (elem.number <= -2) { // symbol
        set<Part> adjacents;

        for (auto k = 0; k < 8; k++) {
          auto di = i + dy[k], dj = j + dx[k];

          if (is_valid_pos(di, dj, r, c) && map[di][dj].number > 0) {
            auto part = map[di][dj];
            if (adjacents.find(part) == adjacents.end())
              adjacents.insert(part);
          }
        }

        for (auto &part : adjacents) {
          total += part.number;
        }
      }
    }
  }

  return total;
}

int part2(const schematic &map) {
  int total_ratios = 0;

  int r = map.size();
  for (int i = 0; i < r; i++) {
    auto row = map[i];
    int c = row.size();
    for (int j = 0; j < c; j++) {
      auto elem = row[j];
      if (elem.number == -2) { // symbol *
        set<Part> adjacents;

        for (auto k = 0; k < 8; k++) {
          auto di = i + dy[k], dj = j + dx[k];

          if (is_valid_pos(di, dj, r, c) && map[di][dj].number > 0) {
            auto part = map[di][dj];
            if (adjacents.find(part) == adjacents.end())
              adjacents.insert(part);
          }
        }

        if (adjacents.size() == 2) {
          auto ratio = 1;
          for (auto &part : adjacents)
            ratio *= part.number;

          total_ratios += ratio;
        }
      }
    }
  }

  return total_ratios;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto map = parse_map(lines);

  auto r1 = part1(map);
  println("Part 1: ", r1);
  auto r2 = part2(map);
  println("Part 2: ", r2);
  return 0;
}
#include "utils.h"

using namespace std;
using namespace aoc_utils;

struct Reflection {
  int line;
  int value;
  bool row;
};

struct Pattern {
  vector<vector<char>> map;
  Reflection reflection;
};

vector<Pattern> read_patterns(const vector<string> &lines) {

  vector<Pattern> result;
  Pattern current;
  for (auto &l : lines) {
    if (l.empty()) {
      result.push_back(current);
      current = {};
      continue;
    }
    current.map.push_back({});
    for (auto &c : l) {
      current.map.back().push_back(c);
    }
  }
  result.push_back(current);

  return result;
}

void print_pattern(const Pattern &p) {
  for (auto &row : p.map) {
    for (auto &c : row) {
      print(c);
    }
    println();
  }
  println();
}

void print_reflection(const Reflection &r) {
  println(r.line, ",", r.value, ", ", r.row);
}

Reflection find_reflection(const Pattern &p) {

  int n = p.map.size(), m = p.map.front().size();

  Reflection result{0, 0, true};
  for (int i = 0; i < n; i++) {
    auto row_reflection = 0;
    bool perfect = false;

    for (int j = 0; j < n; j++) {
      if (i - j < 0 || i + j + 1 >= n) {
        perfect = true;
        break;
      }
      if (p.map[i - j] == p.map[i + j + 1])
        row_reflection++;
      else
        break;
    }
    if (perfect && row_reflection > result.value) {
      result.value = row_reflection;
      result.line = i + 1;
    }
  }

  for (int i = 0; i < m; i++) {
    auto column_reflection = 0;
    bool perfect = false;

    for (int j = 0; j < m; j++) {

      if (i - j < 0 || i + j + 1 >= m) {
        perfect = true;
        break;
      }
      bool equal = true;
      for (int k = 0; k < n; k++) {
        if (p.map[k][i - j] != p.map[k][i + j + 1]) {
          equal = false;
          break;
        }
      }
      if (equal)
        column_reflection++;
      else
        break;
    }
    if (perfect && column_reflection > result.value) {
      result.row = false;
      result.value = column_reflection;
      result.line = i + 1;
    }
  }

  return result;
}

Reflection find_reflection_with_smudge(const Pattern &p) {

  int n = p.map.size(), m = p.map.front().size();

  Reflection result{0, 0, true};
  for (int i = 0; i < n; i++) {
    auto row_reflection = 0;
    int diff = 0;

    if (p.reflection.row && i + 1 == p.reflection.line)
      continue;

    for (int j = 0; j < n; j++) {
      if (i - j < 0 || i + j + 1 >= n) {
        break;
      }
      int not_equal = 0;
      for (int k = 0; k < m; k++) {
        if (p.map[i - j][k] != p.map[i + j + 1][k]) {
          not_equal++;
        }
      }
      diff += not_equal;

      if (not_equal <= 1) {
        row_reflection++;
      } else
        break;
    }
    if (diff == 1 && row_reflection > result.value) {
      result.value = row_reflection;
      result.line = i + 1;
    }
  }

  for (int i = 0; i < m; i++) {
    auto column_reflection = 0;
    int diff = 0;

    if (!p.reflection.row && i + 1 == p.reflection.line)
      continue;

    for (int j = 0; j < m; j++) {

      if (i - j < 0 || i + j + 1 >= m) {
        break;
      }
      int not_equal = 0;
      for (int k = 0; k < n; k++) {
        if (p.map[k][i - j] != p.map[k][i + j + 1]) {
          not_equal++;
        }
      }
      diff += not_equal;

      if (not_equal <= 1) {
        column_reflection++;
      } else
        break;
    }
    if (diff == 1 && column_reflection > result.value) {
      result.row = false;
      result.value = column_reflection;
      result.line = i + 1;
    }
  }

  return result;
}

int part1(vector<Pattern> &patterns) {

  int rows = 0, columns = 0;
  for (auto &p : patterns) {
    auto r = find_reflection(p);
    p.reflection = r;
    auto size = r.line;
    if (r.row)
      rows += size;
    else
      columns += size;
  }
  return columns + 100 * rows;
}

int part2(const vector<Pattern> &patterns) {
  int rows = 0, columns = 0;
  for (auto &p : patterns) {
    auto r = find_reflection_with_smudge(p);
    auto size = r.line;

    if (r.row)
      rows += size;
    else
      columns += size;
  }
  return columns + 100 * rows;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto patterns = read_patterns(lines);

  auto r1 = part1(patterns);
  println("Part 1: ", r1);

  auto r2 = part2(patterns);
  println("Part 2: ", r2);

  return 0;
}

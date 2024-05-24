#include "utils.h"

using namespace std;
using namespace aoc_utils;

using platform_t = vector<vector<char>>;

platform_t read_platform(const vector<string> &lines) {
  platform_t result;

  for (auto &l : lines) {
    vector<char> row;
    for (auto &c : l) {
      row.push_back(c);
    }
    result.push_back(row);
  }

  return result;
}

platform_t tilt(platform_t rocks, int direction) {
  platform_t result = rocks;

  int n = rocks.size(), m = rocks.front().size();

  if (direction == 0) {
    for (int j = 0; j < m; j++) {
      for (int i = 0; i < n; i++) {
        if (result[i][j] == 'O') {
          int k = i;
          while (k - 1 >= 0 && result[k - 1][j] == '.') {
            k--;
          }

          if (k != i) {
            result[k][j] = 'O';
            result[i][j] = '.';
          }
        }
      }
    }
  } else if (direction == 1) {
    for (int i = 0; i < n; i++) {
      for (int j = 0; j < m; j++) {
        if (result[i][j] == 'O') {
          int k = j;
          while (k - 1 >= 0 && result[i][k - 1] == '.') {
            k--;
          }

          if (k != j) {
            result[i][k] = 'O';
            result[i][j] = '.';
          }
        }
      }
    }
  } else if (direction == 2) {
    for (int j = 0; j < m; j++) {
      for (int i = n - 1; i >= 0; i--) {
        if (result[i][j] == 'O') {
          int k = i;
          while (k + 1 < n && result[k + 1][j] == '.') {
            k++;
          }

          if (k != i) {
            result[k][j] = 'O';
            result[i][j] = '.';
          }
        }
      }
    }
  } else {
    for (int i = 0; i < n; i++) {
      for (int j = m - 1; j >= 0; j--) {
        if (result[i][j] == 'O') {
          int k = j;
          while (k + 1 < m && result[i][k + 1] == '.') {
            k++;
          }

          if (k != j) {
            result[i][k] = 'O';
            result[i][j] = '.';
          }
        }
      }
    }
  }

  return result;
}

int north_load(const platform_t &rocks) {
  int total_load = 0;
  int n = rocks.size(), m = rocks.front().size();

  for (int j = 0; j < m; j++) {
    for (int i = 0; i < n; i++) {
      if (rocks[i][j] == 'O') {
        total_load += n - i;
      }
    }
  }

  return total_load;
}

void print_platform(const platform_t &p) {
  for (auto &row : p) {
    for (auto c : row) {
      print(c);
    }
    println();
  }
  println();
}

int part1(const platform_t &rocks) {

  auto p = tilt(rocks, 0);

  return north_load(p);
}

int part2(const platform_t &rocks, int cycles) {

  auto p = rocks;

  int i, start;
  map<platform_t, int> rotations{{p, 0}};
  for (i = 1; i <= cycles; i++) {
    for (auto k = 0; k < 4; k++) {
      p = tilt(p, k);
    }

    if (rotations.find(p) != rotations.end()) {
      start = rotations.at(p);
      break;
    }

    rotations[p] = i;
  }

  auto remaining = (cycles - start) % (i - start);

  for (; remaining > 0; remaining--) {
    for (auto k = 0; k < 4; k++) {
      p = tilt(p, k);
    }
  }

  return north_load(p);
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto rocks = read_platform(lines);

  auto r1 = part1(rocks);
  println("Part 1: ", r1);

  auto r2 = part2(rocks, 1000000000);
  println("Part 2: ", r2);

  return 0;
}

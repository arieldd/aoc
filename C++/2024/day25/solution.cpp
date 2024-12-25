#include "utils.h"
#include <cassert>
#include <mutex>
using namespace std;
using namespace aoc_utils;

#define ll long long
#define arr array

vector<vector<string>> read_input(const string &filename) {
  vector<vector<string>> schematics{{}};
  ifstream fs(filename);

  string line;
  while (getline(fs, line)) {
    if (line.empty()) {
      schematics.push_back({});
      continue;
    }
    schematics.back().push_back(line);
  }
  return schematics;
}

int part1(const vector<vector<string>> &schematics) {
  vector<vector<string>> locks, keys;

  for (auto grid : schematics) {
    if (grid[0][0] == '#') {
      locks.push_back(grid);
    } else {
      keys.push_back(grid);
    }
  }

  auto find_heights = [](vector<vector<string>> grids) -> vector<arr<int, 5>> {
    vector<arr<int, 5>> heights;
    for (auto &key : grids) {
      int n = key.size(), m = 5;
      heights.push_back({});
      for (int j = 0; j < m; j++) {
        int colum_height = 0;
        for (int i = 0; i < n; i++) {
          if (key[i][j] == '#')
            colum_height++;
        }
        heights.back()[j] = colum_height;
      }
    }
    return heights;
  };
  auto key_heights = find_heights(keys);
  auto lock_heights = find_heights(locks);

  int result = 0;
  for (int i = 0; i < keys.size(); i++) {
    auto key = keys[i].size();
    for (int j = 0; j < locks.size(); j++) {
      auto lock = locks[j].size();
      bool fits = true;
      for (int k = 0; k < 5; k++) {
        auto column = key_heights[i][k] + lock_heights[j][k];
        if (column > key or column > lock) {
          fits = false;
          break;
        }
      }
      if (fits)
        result++;
    }
  }

  return result;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto schematics = read_input(argv[1]);
  cout << "Part 1:" << part1(schematics) << '\n';
  return 0;
}

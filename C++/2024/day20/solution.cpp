#include "utils.h"
#include <cassert>
#include <climits>
#include <iostream>
using namespace std;
using namespace aoc_utils;

#define ll long long
#define arr array

vector<string> read_input(const string &filename) {
  vector<string> grid;
  ifstream fs(filename);

  string line;
  while (getline(fs, line)) {
    grid.push_back(line);
  }
  return grid;
}

int solve(const vector<string> &grid, int cheat_size, int threshold) {
  int n = grid.size(), m = grid[0].size();

  vector<int> distances(n * m, INT_MAX);
  arr<int, 2> start, end;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (grid[i][j] == 'S')
        start = {i, j};
      if (grid[i][j] == 'E') {
        distances[i * m + j] = 0;
        end = {i, j};
      }
    }
  }

  vector<arr<int, 2>> path{end};
  arr<int, 2> prev = end;
  for (int k = 0; k < path.size(); k++) {
    auto [i, j] = path[k];

    for (auto [di, dj] : adj4) {
      int ni = i + di, nj = j + dj;
      if (prev == arr<int, 2>{ni, nj})
        continue;

      if (is_valid_pos(ni, nj, n, m) and grid[ni][nj] != '#') {
        path.push_back({ni, nj});
        distances[ni * m + nj] = distances[i * m + j] + 1;
        break;
      }
    }
    prev = {i, j};
  }

  int result = 0;
  for (auto &pos : path) {
    if (pos == end)
      continue;

    int distance = distances[pos[0] * m + pos[1]];

    if (distance < threshold)
      continue;

    for (int i = pos[0] - cheat_size; i <= pos[0] + cheat_size; i++) {
      for (int j = pos[1] - cheat_size; j <= pos[1] + cheat_size; j++) {
        int walked = abs(pos[0] - abs(i)) + abs(pos[1] - abs(j));
        if (walked > cheat_size)
          continue;
        if (is_valid_pos(i, j, n, m) and grid[i][j] != '#') {
          auto next = arr<int, 2>{i, j};
          int new_dist = distances[next[0] * m + next[1]];
          if (new_dist + walked < distance) {
            // This saves time, is it enough?
            auto saved = distance - (new_dist + walked);
            if (saved >= threshold)
              result++;
          }
        }
      }
    }
  }
  return result;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto grid = read_input(argv[1]);
  cout << "Part 1:" << solve(grid, 2, 100) << '\n';
  cout << "Part 2:" << solve(grid, 20, 100) << '\n';
  return 0;
}

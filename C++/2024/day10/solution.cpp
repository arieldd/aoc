#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

struct Map {
  vector<vector<int>> grid;
  set<pair<int, int>> trailheads;
};

Map read_input(const string &filename) {
  Map result{};
  ifstream fs(filename);

  string line;
  for (int i = 0; getline(fs, line); i++) {
    result.grid.push_back({});
    int j = 0;
    for (auto c : line) {
      int height = c - '0';
      result.grid.back().push_back(height);
      if (!height)
        result.trailheads.insert({i, j});
      j++;
    }
  }
  return result;
}

inline bool is_valid_pos(int i, int j, int r, int c) {
  return i >= 0 && j >= 0 && i < r && j < c;
}

// Start from the right clockwise
const std::vector<int> /**/
    dy{0, 1, 1, 1, 0, -1, -1, -1}, dx{1, 1, 0, -1, -1, -1, 0, 1};

int explore(const Map &map, int i, int j, vector<bool> &visited,
            bool count_trails) {
  int n = map.grid.size(), m = map.grid[0].size();

  visited[i * m + j] = 1;
  if (map.grid[i][j] == 9) {
    if (count_trails)
      // Unmark the 9 so it can be counted for part 2.
      visited[i * m + j] = 0;
    return 1;
  }

  int reachable = 0;

  for (int k = 0; k < 8; k += 2) {
    int ni = i + dy[k], nj = j + dx[k];
    if (is_valid_pos(ni, nj, n, m) and
        map.grid[ni][nj] == map.grid[i][j] + 1 and !visited[ni * m + nj])
      reachable += explore(map, ni, nj, visited, count_trails);
  }

  visited[i * m + j] = 0;

  return reachable;
}

pair<int, int> solve(const Map &map) {
  int part1 = 0, part2 = 0;

  int n = map.grid.size(), m = map.grid[0].size();
  vector<bool> visited(n * m);
  for (auto head : map.trailheads) {
    int i = head.first, j = head.second;

    visited.assign(n * m, false);
    auto score = explore(map, i, j, visited, false);
    part1 += score;

    visited.assign(n * m, false);
    auto trails = explore(map, i, j, visited, true);
    part2 += trails;
  }
  return {part1, part2};
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto map = read_input(argv[1]);
  auto anwers = solve(map);
  cout << "Part 1:" << anwers.first << '\n';
  cout << "Part 2:" << anwers.second << '\n';
  return 0;
}

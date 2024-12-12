#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#define ll long long

const vector<pair<int, int>> adj4 = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

inline bool is_valid_pos(int i, int j, int r, int c) {
  return i >= 0 && j >= 0 && i < r && j < c;
}

vector<string> read_input(const string &filename) {
  vector<string> grid{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    grid.push_back(line);
  return grid;
}

int dfs(const vector<string> &grid, vector<int> &scan, int i, int j,
        int region_id, int &area) {
  int n = grid.size(), m = grid[0].size();

  if (scan[i * m + j] == region_id)
    return 0;

  scan[i * m + j] = region_id;
  area++;

  char label = grid[i][j];
  int perimeter = 0;

  for (auto [di, dj] : adj4) {
    int ni = i + di, nj = j + dj;
    if (!is_valid_pos(ni, nj, n, m) or grid[ni][nj] != label)
      perimeter++;
    else if (grid[ni][nj] == label) {
      perimeter += dfs(grid, scan, ni, nj, region_id, area);
    }
  }
  return perimeter;
}

int count_sides(const vector<int> &scan, int n, int m, int region_id) {
  int total_sides = 0;
  bool inside;

  vector<float> sides, prev;
  for (int i = 0; i < n; i++) {
    inside = false;
    sides.clear();

    int j = 0;
    for (; j < m; j++) {
      if (scan[i * m + j] == region_id and !inside) {
        sides.push_back(j - 0.1);
        inside = true;
      } else if (scan[i * m + j] != region_id and inside) {
        sides.push_back(j + 0.1);
        inside = false;
      }
    }
    if (j == m and inside)
      sides.push_back(j + 0.1);

    for (auto s : sides)
      if (find(prev.begin(), prev.end(), s) == prev.end()) {
        total_sides++;
      }
    prev = sides;
  }

  prev.clear();
  for (int j = 0; j < m; j++) {
    inside = false;
    sides.clear();

    int i = 0;
    for (; i < n; i++) {
      if (scan[i * m + j] == region_id and !inside) {
        sides.push_back(i - 0.1);
        inside = true;
      } else if (scan[i * m + j] != region_id and inside) {
        sides.push_back(i + 0.1);
        inside = false;
      }
    }
    if (i == n and inside)
      sides.push_back(i + 0.1);

    for (auto s : sides) {
      if (find(prev.begin(), prev.end(), s) == prev.end()) {
        total_sides++;
      }
    }
    prev = sides;
  }
  return total_sides;
}

pair<int, int> solve(const vector<string> &grid) {
  int n = grid.size(), m = grid[0].size();

  vector<int> scan(n * m, 0);
  int region_id = 0, part1 = 0, part2 = 0;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (scan[i * m + j])
        continue;
      region_id++;
      int area = 0;
      auto perimeter = dfs(grid, scan, i, j, region_id, area);
      part1 += perimeter * area;
      part2 += count_sides(scan, n, m, region_id) * area;
    }
  }

  return {part1, part2};
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto grid = read_input(argv[1]);
  auto answers = solve(grid);
  cout << "Part 1:" << answers.first << '\n';
  cout << "Part 2:" << answers.second << '\n';
  return 0;
}

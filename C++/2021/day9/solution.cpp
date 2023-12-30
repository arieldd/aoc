#include "utils.h"
#include <ranges>

using namespace std;
using namespace aoc_utils;

using grid = vector<vector<int>>;

grid read_heights(const vector<string> &lines) {
  grid res;

  for (auto &l : lines) {
    vector<int> row;
    for (auto &c : l) {
      row.push_back(c - '0');
    }
    res.push_back(row);
  }
  return res;
}

vector<pair<int, int>> find_low_points(const grid &heights) {
  auto n = heights.size(), m = heights[0].size();

  vector<pair<int, int>> res;
  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      bool low_point = true;
      for (auto d = 0; d < 8; d += 2) {
        auto ni = i + dy[d], nj = j + dx[d];
        if (is_valid_pos(ni, nj, n, m) && heights[ni][nj] <= heights[i][j]) {
          low_point = false;
          break;
        }
      }
      if (low_point)
        res.push_back({i, j});
    }

  return res;
}

int find_basin_size(const pair<int, int> low_point, const grid &heights) {
  set<pair<int, int>> basin;
  auto n = heights.size(), m = heights[0].size();

  deque<pair<int, int>> exploring{low_point};
  while (!exploring.empty()) {
    auto current = exploring.front();
    exploring.pop_front();

    basin.insert(current);

    for (auto d = 0; d < 8; d += 2) {
      pair<int, int> next{current.first + dy[d], current.second + dx[d]};
      if (is_valid_pos(next.first, next.second, n, m) &&
          heights[next.first][next.second] < 9 &&
          basin.find(next) == basin.end()) {
        exploring.push_back(next);
      }
    }
  }

  return basin.size();
}

int part1(const grid &heights, const vector<pair<int, int>> &low_points) {

  return accumulate(low_points.begin(), low_points.end(), 0,
                    [&](int acc, const pair<int, int> &location) {
                      return acc + heights[location.first][location.second] + 1;
                    });
}

int part2(const grid &heights, const vector<pair<int, int>> &low_points) {

  vector<int> basin_sizes;
  for (auto &point : low_points) {
    basin_sizes.push_back(find_basin_size(point, heights));
  }
  ranges::sort(basin_sizes, ranges::greater());

  return basin_sizes[0] * basin_sizes[1] * basin_sizes[2];
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto heights = read_heights(lines);

  auto low_points = find_low_points(heights);

  auto r1 = part1(heights, low_points);
  println("Part 1: ", r1);

  auto r2 = part2(heights, low_points);
  println("Part 2: ", r2);

  return 0;
}
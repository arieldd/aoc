#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

std::vector<std::string> parse_input(const std::string &file_name) {
  std::vector<std::string> ret;

  std::ifstream fs(file_name);
  std::string line;
  while (std::getline(fs, line)) {
    ret.push_back(line);
  }
  return ret;
}

bool is_visible(std::vector<std::vector<int>> &map, int row, int column) {
  int n = map.size(), m = map.front().size(), height = map[row][column];

  int i = row - 1;
  for (; i >= 0; i--)
    if (map[i][column] >= height)
      break;

  if (i < 0)
    return true;

  i = row + 1;
  for (; i < n; i++)
    if (map[i][column] >= height)
      break;

  if (i == n)
    return true;

  i = column - 1;
  for (; i >= 0; i--)
    if (map[row][i] >= height)
      break;

  if (i < 0)
    return true;

  i = column + 1;
  for (; i < m; i++)
    if (map[row][i] >= height)
      break;

  return i == m;
}

bool is_visible_2(std::vector<std::vector<int>> &map, int row, int column) {
  int n = map.size(), m = map.front().size(), height = map[row][column];

  std::vector<int> dy{0, -1, 0, 1}, dx{-1, 0, 1, 0};

  for (auto i = 0; i < 4; i++) {
    auto x = column, y = row;
    for (;;) {
      x += dx[i];
      y += dy[i];

      if (x < 0 || y < 0 || x >= m || y >= n)
        return true;

      if (map[y][x] >= height)
        break;
    }
  }

  return false;
}

int scenic_score(std::vector<std::vector<int>> &map, int row, int column) {
  int n = map.size(), m = map.front().size(), height = map[row][column],
      view_distance, result = 1;

  std::vector<int> dy{0, -1, 0, 1}, dx{-1, 0, 1, 0};

  for (auto i = 0; i < 4; i++) {
    view_distance = 0;
    auto x = column, y = row;
    for (;;) {
      x += dx[i];
      y += dy[i];

      if (x < 0 || y < 0 || x >= m || y >= n)
        break;

      view_distance++;

      if (map[y][x] >= height)
        break;
    }
    result *= view_distance;
  }

  return result;
}

int part1(std::vector<std::vector<int>> &map) {

  int r = map.size(), c = map.front().size(),
      visible_trees = r * 2 + (c - 2) * 2;

  for (int i = 1; i < r - 1; i++) {
    for (int j = 1; j < c - 1; j++) {
      if (is_visible_2(map, i, j)) {
        visible_trees++;
      }
    }
  }

  return visible_trees;
}
int part2(std::vector<std::vector<int>> &map) {
  int r = map.size(), c = map.front().size();

  std::vector<int> scores;

  for (int i = 1; i < r - 1; i++) {
    for (int j = 1; j < c - 1; j++) {
      scores.push_back(scenic_score(map, i, j));
    }
  }

  return *std::max_element(scores.begin(), scores.end());
}

std::vector<std::vector<int>> read_map(std::vector<std::string> &lines) {
  std::vector<std::vector<int>> result;

  for (auto &line : lines) {
    std::vector<int> row;
    for (auto &tree : line) {
      if (tree < '0')
        continue;
      auto height = tree - '0';
      row.push_back(height);
    }

    result.push_back(row);
  }

  return result;
}

void print_map(std::vector<std::vector<int>> &map) {
  for (auto &row : map) {
    for (auto &entry : row) {
      std::cout << entry << " ";
    }
    std::cout << std::endl;
  }
}

int main() {
  auto lines = parse_input("input.txt");
  // auto lines = parse_input("test.txt");

  auto map = read_map(lines);
  // print_map(map);

  auto r1 = part1(map);
  std::cout << "Part 1: " << r1 << std::endl;
  auto r2 = part2(map);
  std::cout << "Part 2: " << r2 << std::endl;
  return 0;
}
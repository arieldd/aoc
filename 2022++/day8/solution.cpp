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
    std::cout << ret.back() << std::endl;
  }
  return ret;
}

bool is_visible(std::vector<std::vector<int>> &map, int row, int column) {
  int n = map.size(), height = map[row][column];

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
  for (; i < n; i++)
    if (map[row][i] >= height)
      break;

  return i == n;
}

int part1(std::vector<std::vector<int>> &map) {

  int n = map.size(), visible_trees = n * 4 - 4;

  for (int i = 1; i < n - 1; i++) {
    for (int j = 1; j < n - 1; j++) {
      if (is_visible(map, i, j)) {
        // std::cout << map[i][j] << " (" << i << "," << j << ")" << std::endl;
        visible_trees++;
      }
    }
  }

  return visible_trees;
}
int part2(std::vector<std::vector<int>> &map) { return 0; }

std::vector<std::vector<int>> read_map(std::vector<std::string> &lines) {
  int n = lines.size();

  std::vector<std::vector<int>> result;

  for (auto &line : lines) {
    result.push_back(std::vector<int>());
    for (auto &tree : line) {
      int height = tree - '0';
      result.back().push_back(height);
    }
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
  //print_map(map);

  auto r1 = part1(map);
  std::cout << "Part 1: " << r1 << std::endl;
  auto r2 = part2(map);
  std::cout << "Part 2: " << r2 << std::endl;
  return 0;
}
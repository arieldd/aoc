#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <vector>
using namespace std;

inline bool is_valid_pos(int i, int j, int r, int c) {
  return i >= 0 && j >= 0 && i < r && j < c;
}

struct Grid {
  map<char, vector<pair<int, int>>> antennas;
  int height, width;
};

Grid read_input(const string &filename) {
  Grid result;
  ifstream fs(filename);

  string line;
  int i = 0;
  for (; getline(fs, line); i++) {
    result.width = line.size();
    for (int j = 0; j < result.width; j++) {
      char label = line[j];
      if (label == '.')
        continue;
      if (!result.antennas.count(label))
        result.antennas.insert({label, {}});
      result.antennas[label].push_back({i, j});
    }
  }
  result.height = i;
  return result;
}

int part1(const Grid &map) {
  vector<bool> antinodes(map.height * map.width);
  for (auto &group : map.antennas) {
    auto antennas = group.second;
    auto size = antennas.size();
    for (int i = 0; i < size - 1; i++) {
      auto p1 = antennas[i];
      for (int j = i + 1; j < size; j++) {
        auto p2 = antennas[j];
        pair<int, int> node1 = {p1.first - 2 * (p1.first - p2.first),
                                p1.second - 2 * (p1.second - p2.second)};

        pair<int, int> node2 = {p2.first - 2 * (p2.first - p1.first),
                                p2.second - 2 * (p2.second - p1.second)};

        if (is_valid_pos(node1.first, node1.second, map.height, map.width)) {
          antinodes[node1.first * map.width + node1.second] = true;
        }
        if (is_valid_pos(node2.first, node2.second, map.height, map.width)) {
          antinodes[node2.first * map.width + node2.second] = true;
        }
      }
    }
  }
  return accumulate(antinodes.begin(), antinodes.end(), 0);
}

int part2(const Grid &map) {
  vector<bool> antinodes(map.height * map.width);
  for (auto &group : map.antennas) {
    auto antennas = group.second;
    auto size = antennas.size();
    for (int i = 0; i < size - 1; i++) {
      auto p1 = antennas[i];
      for (int j = i + 1; j < size; j++) {
        auto p2 = antennas[j];
        pair<int, int> node1 = p1;
        pair<int, int> node2 = p2;

        while (is_valid_pos(node1.first, node1.second, map.height, map.width)) {
          antinodes[node1.first * map.width + node1.second] = true;
          node1 = {node1.first - (p1.first - p2.first),
                   node1.second - (p1.second - p2.second)};
        }

        while (is_valid_pos(node2.first, node2.second, map.height, map.width)) {
          antinodes[node2.first * map.width + node2.second] = true;
          node2 = {node2.first - (p2.first - p1.first),
                   node2.second - (p2.second - p1.second)};
        }
      }
    }
  }
  return accumulate(antinodes.begin(), antinodes.end(), 0);
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto region = read_input(argv[1]);
  cout << "Part 1:" << part1(region) << '\n';
  cout << "Part 2:" << part2(region) << '\n';
  return 0;
}

#include <cassert>
#include <fstream>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

inline bool is_valid_pos(int i, int j, int r, int c) {
  return i >= 0 && j >= 0 && i < r && j < c;
}

// Start from the right clockwise
const std::vector<int> /**/
    dy{0, 1, 1, 1, 0, -1, -1, -1}, dx{1, 1, 0, -1, -1, -1, 0, 1};

struct Lab {
  vector<vector<char>> map;
  pair<int, int> guard;
  int dir = 6;
};

Lab read_input(const string &filename) {
  Lab result;
  ifstream fs(filename);

  string line;
  int i = 0;
  while (getline(fs, line)) {
    result.map.push_back({});
    for (int j = 0; j < line.size(); j++) {
      result.map[i].push_back(line[j]);
      if (result.map[i][j] == '^')
        result.guard = {i, j};
    }
    i++;
  }
  return result;
}

set<pair<int, int>> part1(const Lab &input) {
  int n = input.map.size(), m = input.map[0].size();
  Lab explore = input;
  set<pair<int, int>> path{{input.guard.first, input.guard.second}};
  for (;;) {
    auto ni = explore.guard.first + dy[explore.dir];
    auto nj = explore.guard.second + dx[explore.dir];
    if (!is_valid_pos(ni, nj, n, m))
      break;
    if (explore.map[ni][nj] == '#')
      explore.dir = (explore.dir + 2) % 8;
    else {
      path.insert({ni, nj});
      explore.guard = {ni, nj};
    }
  }

  return path;
}

bool in_a_loop(Lab &explore) {
  int n = explore.map.size(), m = explore.map[0].size();
  set<tuple<int, int, int>> visited;
  for (;;) {
    auto ni = explore.guard.first + dy[explore.dir];
    auto nj = explore.guard.second + dx[explore.dir];
    if (!is_valid_pos(ni, nj, n, m))
      return false;
    if (explore.map[ni][nj] == '#')
      explore.dir = (explore.dir + 2) % 8;
    else {
      tuple<int, int, int> next = {ni, nj, explore.dir};
      if (visited.contains(next))
        return true;
      visited.insert(next);
      explore.guard = {ni, nj};
    }
  }

  return false;
}

int part2(const Lab &input, const set<pair<int, int>> &path) {
  int n = input.map.size(), m = input.map[0].size();

  int obstacles = 0;
  Lab explore = input;
  for (auto &pos : path) {
    int i = pos.first, j = pos.second;
    if (i == input.guard.first and j == input.guard.second)
      continue;

    explore.guard = input.guard;
    explore.dir = input.dir;
    explore.map[i][j] = '#';
    if (in_a_loop(explore))
      obstacles++;
    explore.map[i][j] = '.';
  }
  return obstacles;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto lab = read_input(argv[1]);
  auto path = part1(lab);
  cout << "Part 1 " << path.size() << '\n';
  cout << "Part 2 " << part2(lab, path) << '\n';
  return 0;
}

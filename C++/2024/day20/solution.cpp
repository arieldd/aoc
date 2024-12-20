#include "utils.h"
#include <cassert>
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

auto cheat_path(const vector<string> &grid, arr<int, 2> start, arr<int, 2> from,
                arr<int, 2> end, const vector<arr<int, 2>> &race) {

  int n = grid.size(), m = grid[0].size();
  vector<arr<int, 2>> path;

  auto [i, j] = start;

  pair<arr<int, 4>, int> cheat = {{i, j, -1, -1}, INT_MAX};
  for (auto [di, dj] : adj4) {
    arr<int, 2> pos = {i + di, j + dj};
    if (pos == from)
      continue;

    if (is_valid_pos(pos[0], pos[1], n, m) and grid[pos[0]][pos[1]] != '#') {
      auto it = find(race.begin(), race.end(), pos);
      int local_dist = distance(it, race.end());
      if (local_dist < cheat.second) {
        cheat.first[2] = pos[0];
        cheat.first[3] = pos[1];
        cheat.second = local_dist;
      }
    }
  }
  return cheat;
}

void find_cheats(const vector<string> &grid, int index, arr<int, 2> end,
                 const vector<arr<int, 2>> &race,
                 map<arr<int, 4>, int> &cheats) {

  int n = grid.size(), m = grid[0].size();
  auto current = race[index];
  int distance = race.size() - index - 1;
  for (auto [di, dj] : adj4) {
    int ni = current[0] + di, nj = current[1] + dj;
    if (is_valid_pos(ni, nj, n, m) and grid[ni][nj] == '#') {
      auto cheat = cheat_path(grid, {ni, nj}, current, end, race);
      if (cheat.second < distance) {
        cheat.second = distance - cheat.second - 1;
        cheats.insert(cheat);
      }
    }
  }
}

int part1(const vector<string> &grid) {
  int n = grid.size(), m = grid[0].size();

  arr<int, 2> start, end;
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (grid[i][j] == 'S')
        start = {i, j};
      else if (grid[i][j] == 'E')
        end = {i, j};
    }
  }

  vector<arr<int, 2>> race;
  vector<arr<int, 2>> dq{start};

  for (int k = 0; k < dq.size(); k++) {
    auto [i, j] = dq[k];

    race.push_back(dq[k]);
    for (auto [di, dj] : adj4) {
      int ni = i + di, nj = j + dj;
      if (is_valid_pos(ni, nj, n, m) and grid[ni][nj] != '#' and
          find(dq.begin(), dq.end(), arr<int, 2>{ni, nj}) == dq.end())
        dq.push_back({ni, nj});
    }
  }

  map<arr<int, 4>, int> cheats;
  for (int i = 0; i < race.size(); i++) {
    find_cheats(grid, i, end, race, cheats);
  }

  int result = 0;
  map<int, int> savings;
  for (auto entry : cheats) {
    savings[entry.second]++;
    if (entry.second >= 100)
      result++;
  }

  return result;
}

int part2(const vector<string> &lines) { return 0; }

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto grid = read_input(argv[1]);
  cout << "Part 1:" << part1(grid) << '\n';
  cout << "Part 2:" << part2(grid) << '\n';
  return 0;
}

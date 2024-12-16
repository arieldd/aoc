#include "utils.h"
#include <algorithm>
#include <cassert>
using namespace std;
using namespace aoc_utils;

#define ll long long
#define arr array

struct Entry {
  int i, j, dir, score;
};

vector<string> read_input(const string &filename) {
  vector<string> grid{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line)) {
    grid.push_back(line);
  }
  return grid;
}

arr<int, 2> solve(const vector<string> &grid) {

  auto n = grid.size(), m = grid[0].size();

  arr<int, 2> start, end;

  for (int i = 0; i < n; i++)
    for (int j = 0; j < m; j++) {
      if (grid[i][j] == 'S') {
        start[0] = i;
        start[1] = j;
      } else if (grid[i][j] == 'E') {
        end[0] = i;
        end[1] = j;
      }
    }

  int score = INT_MAX;

  auto cmp = [](const Entry &lhs, const Entry &rhs) {
    return lhs.score > rhs.score;
  };

  priority_queue<Entry, vector<Entry>, decltype(cmp)> pq;
  pq.push({.i = start[0], .j = start[1], .dir = 0, .score = 0});

  set<arr<int, 3>> seen;
  // For Part 2
  vector<char> best_tiles(n * m);
  best_tiles[start[0] * m + start[1]] = 1;
  best_tiles[end[0] * m + end[1]] = 1;

  map<arr<int, 3>, vector<arr<int, 3>>> prev;

  while (!pq.empty()) {
    auto current = pq.top();
    pq.pop();

    if (seen.contains({current.i, current.j, current.dir}))
      continue;

    if (current.i == end[0] and current.j == end[1]) {
      if (score == INT_MAX)
        score = current.score;
      if (current.score == score) {
        deque<arr<int, 3>> dq{{current.i, current.j, current.score}};
        set<arr<int, 3>> seen_tiles;
        while (!dq.empty()) {
          auto key = dq.front();
          dq.pop_front();

          if (seen_tiles.contains(key))
            continue;
          seen_tiles.insert(key);

          if (prev.contains(key))
            for (auto node : prev.at(key)) {
              best_tiles[node[0] * m + node[1]] = 1;
              dq.push_back(node);
            }
        }
      }
      continue;
    }

    seen.insert({current.i, current.j, current.dir});

    for (int dir = 0; dir < 4; dir++) {
      int di = adj4[dir].first, dj = adj4[dir].second;
      int ni = current.i + di, nj = current.j + dj;
      if (dir == current.dir and grid[ni][nj] != '#') {
        pq.push({ni, nj, dir, current.score + 1});
        prev[{ni, nj, current.score + 1}].push_back(
            {current.i, current.j, current.score});
      } else if (dir != (current.dir + 2) % 4) {
        pq.push({current.i, current.j, dir, current.score + 1000});
        prev[{current.i, current.j, current.score + 1000}].push_back(
            {current.i, current.j, current.score});
      }
    }
  }

  return {score, accumulate(best_tiles.begin(), best_tiles.end(), 0)};
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto grid = read_input(argv[1]);
  auto answers = solve(grid);
  cout << "Part 1:" << answers[0] << '\n';
  cout << "Part 2:" << answers[1] << '\n';
  return 0;
}

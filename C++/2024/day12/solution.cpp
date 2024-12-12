#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <map>
#include <set>
#include <vector>
using namespace std;

#define ll long long

const vector<pair<int, int>> adj4 = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

inline bool is_valid_pos(int i, int j, int r, int c) {
  return i >= 0 && j >= 0 && i < r && j < c;
}

struct Region {
  char id;
  vector<pair<int, int>> pos;

  int area() { return 0; }

  int perimeter() { return 0; }
};

vector<string> read_input(const string &filename) {
  vector<string> lines{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    lines.push_back(line);
  return lines;
}

int dfs(const vector<string> &lines, vector<bool> &seen, int i, int j,
        int &area) {

  int n = lines.size(), m = lines[0].size();
  seen[i * m + j] = 1;

  char id = lines[i][j];
  area++;
  int price = 0, perimeter = 0;

  for (auto [di, dj] : adj4) {
    int ni = i + di, nj = j + dj;
    if (!is_valid_pos(ni, nj, n, m) or lines[ni][nj] != id)
      perimeter++;
    else if (lines[ni][nj] == id and !seen[ni * m + nj]) {
      price += dfs(lines, seen, ni, nj, area);
    }
  }
  price += perimeter;
  return price;
}

int part1(const vector<string> &lines) {
  int result = 0;
  int n = lines.size(), m = lines[0].size();

  vector<bool> seen(n * m);

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (seen[i * m + j])
        continue;
      int area = 0, sides = 2;
      auto price = dfs(lines, seen, i, j, area);
      result += price * area;
    }
  }

  return result;
}

void dfs2(const vector<string> &lines, vector<bool> &seen, int i, int j) {
  int n = lines.size(), m = lines[0].size();
  if (seen[i * m + j])
    return;

  seen[i * m + j] = 1;

  char id = lines[i][j];

  for (auto [di, dj] : adj4) {
    int ni = i + di, nj = j + dj;
    if (is_valid_pos(ni, nj, n, m) and lines[ni][nj] == id) {
      dfs2(lines, seen, ni, nj);
    }
  }
}

int part2(const vector<string> &lines) {
  int result = 0;
  int n = lines.size(), m = lines[0].size();

  vector<pair<char, vector<bool>>> regions;
  vector<bool> seen(n * m);
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (seen[i * m + j])
        continue;
      vector<bool> scan(n * m);
      dfs2(lines, scan, i, j);
      for (int k = 0; k < n * m; k++)
        if (scan[k])
          seen[k] = scan[k];
      regions.push_back({lines[i][j], scan});
    }
  }

  for (auto &region : regions) {
    auto &scan = region.second;
    int area = 0;
    for (auto pos : scan)
      if (pos)
        area++;

    int sides = 0;
    vector<float> prev;
    for (int i = 0; i < n; i++) {
      vector<float> vsides;
      bool inside = false;
      int j = 0;
      for (; j < m; j++) {
        if (scan[i * m + j] and !inside) {
          vsides.push_back(j - 0.1);
          inside = true;
        } else if (!scan[i * m + j] and inside) {
          vsides.push_back(j + 0.1);
          inside = false;
        }
      }
      if (j == m and inside)
        vsides.push_back(j + 0.1);

      for (auto s : vsides)
        if (find(prev.begin(), prev.end(), s) == prev.end()) {
          sides++;
        }
      prev = vsides;
    }
    prev.clear();
    for (int j = 0; j < m; j++) {
      vector<float> hsides;
      bool inside = false;
      int i = 0;
      for (; i < n; i++) {
        if (scan[i * m + j] and !inside) {
          hsides.push_back(i - 0.1);
          inside = true;
        } else if (!scan[i * m + j] and inside) {
          hsides.push_back(i + 0.1);
          inside = false;
        }
      }
      if (i == n and inside)
        hsides.push_back(i + 0.1);

      for (auto s : hsides) {
        if (find(prev.begin(), prev.end(), s) == prev.end()) {
          sides++;
        }
      }
      prev = hsides;
    }
    result += area * sides;
  }

  return result;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto lines = read_input(argv[1]);
  cout << "Part 1:" << part1(lines) << '\n';
  cout << "Part 2:" << part2(lines) << '\n';
  return 0;
}

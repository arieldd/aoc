#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

inline bool is_valid_pos(int i, int j, int r, int c) {
  return i >= 0 && j >= 0 && i < r && j < c;
}

// Start from the right clockwise
const std::vector<int> /**/
    dy{0, 1, 1, 1, 0, -1, -1, -1}, dx{1, 1, 0, -1, -1, -1, 0, 1};

vector<string> read_input(const string &filename) {
  vector<string> lines{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    lines.push_back(line);
  return lines;
}

int part1(const vector<string> &lines) {
  const vector<char> xmas = {'X', 'M', 'A', 'S'};
  auto xmas_count = 0;

  auto n = lines.size();
  auto m = lines[0].size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (lines[i][j] == 'X') {
        for (int k = 0; k < 8; k++) {
          auto ni = i;
          auto nj = j;
          bool found = true;
          for (int l = 1; l < xmas.size(); l++) {
            ni += dy[k];
            nj += dx[k];
            if (!is_valid_pos(ni, nj, n, m) or lines[ni][nj] != xmas[l]) {
              found = false;
              break;
            }
          }
          if (found)
            xmas_count++;
        }
      }
    }
  }
  return xmas_count;
}

int part2(const vector<string> &lines) {
  const vector<char> ms = {'M', 'S'};
  auto xmas_count = 0;

  auto n = lines.size();
  auto m = lines[0].size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++) {
      if (lines[i][j] == 'A') {
        bool found = true;
        for (int k = 1; k < 4; k += 2) {
          int ni_1 = i + dy[k],
              ni_2 = i + dy[(k + 4) % 8], //
              nj_1 = j + dx[k],           //
              nj_2 = j + dx[(k + 4) % 8];
          if (!is_valid_pos(ni_1, nj_1, n, m) or
              !is_valid_pos(ni_2, nj_2, n, m) or
              find(ms.begin(), ms.end(), lines[ni_1][nj_1]) == ms.end() or
              find(ms.begin(), ms.end(), lines[ni_2][nj_2]) == ms.end() or
              (lines[ni_1][nj_1] == 'M' and lines[ni_2][nj_2] != 'S') or
              (lines[ni_1][nj_1] == 'S' and lines[ni_2][nj_2] != 'M')) {
            found = false;
            break;
          }
        }
        if (found) {
          xmas_count++;
        }
      }
    }
  }
  return xmas_count;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto lines = read_input(argv[1]);
  cout << "Part 1 :" << part1(lines) << '\n';
  cout << "Part 2 :" << part2(lines) << '\n';
  return 0;
}

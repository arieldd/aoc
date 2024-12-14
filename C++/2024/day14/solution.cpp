#include "utils.h"
#include <cassert>
using namespace std;
using namespace aoc_utils;

#define arr array

struct Robot {
  arr<int, 2> pos;
  arr<int, 2> vel;
};

vector<string> read_input(const string &filename) {
  vector<string> lines{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    lines.push_back(line);
  return lines;
}

void print_grid(const vector<Robot> robots, int n, int m) {
  vector<char> grid(m * n);
  for (auto &r : robots)
    grid[r.pos[1] * m + r.pos[0]] = 1;

  for (auto i = 0; i < n; i++) {
    for (auto j = 0; j < m; j++) {
      if (grid[i * m + j])
        cout << '#';
      else
        cout << ' ';
    }
    cout << '\n';
  }
}

int part1(vector<Robot> robots, int n, int m) {
  arr<int, 4> quads = {0, 0, 0, 0};
  for (auto &r : robots) {
    r.pos[0] = modulo(r.pos[0] + r.vel[0] * 100, m);
    r.pos[1] = modulo(r.pos[1] + r.vel[1] * 100, n);
    if (r.pos[0] < m / 2 and r.pos[1] < n / 2)
      quads[0]++;
    else if (r.pos[0] < m / 2 and r.pos[1] > n / 2)
      quads[1]++;
    else if (r.pos[0] > m / 2 and r.pos[1] < n / 2)
      quads[2]++;
    else if (r.pos[0] > m / 2 and r.pos[1] > n / 2)
      quads[3]++;
  }

  return accumulate(quads.begin(), quads.end(), 1,
                    [](const int acc, int e) { return acc * e; });
}

int part2(vector<Robot> robots, int n, int m) {
  int secs = 1;
  vector<char> overlap(n * m);
  for (; secs <= 9000; secs++) {
    overlap.assign(n * m, 0);
    for (auto &r : robots) {
      r.pos[0] = modulo(r.pos[0] + r.vel[0], m);
      r.pos[1] = modulo(r.pos[1] + r.vel[1], n);
      overlap[r.pos[1] * m + r.pos[0]] = 1;
    }
    // When the tree forms no robots are on top of each other
    if (accumulate(overlap.begin(), overlap.end(), 0) == robots.size()) {
      print_grid(robots, n, m);
      break;
    }
  }

  return secs;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto lines = read_input(argv[1]);

  int n = 103, m = 101;
  vector<Robot> robots;
  for (auto &l : lines) {
    Robot next;
    auto numbers = nums<int>(l);
    next.pos[0] = numbers[0];
    next.pos[1] = numbers[1];
    next.vel[0] = numbers[2];
    next.vel[1] = numbers[3];
    robots.push_back(next);
  }

  cout << "Part 1:" << part1(robots, n, m) << '\n';
  cout << "Part 2:" << part2(robots, n, m) << '\n';
  return 0;
}

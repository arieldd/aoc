#include "utils.h"
#include <cassert>
#include <numeric>
using namespace std;
using namespace aoc_utils;

#define ll long long
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

void print_grid(const vector<Robot> robots, int secs, int n, int m) {
  cout << '\n' << secs << '\n';
  for (auto i = 0; i < n; i++) {
    for (auto j = 0; j < m; j++) {
      bool found = false;
      for (auto &r : robots)
        if (r.pos[0] == j and r.pos[1] == i) {
          found = true;
          break;
        }
      if (found)
        cout << '#';
      else
        cout << '.';
    }
    cout << '\n';
  }
}

ll part1(vector<Robot> robots, int n, int m) {
  arr<ll, 4> quads;
  fill(quads.begin(), quads.end(), 0);
  for (auto &r : robots) {
    for (int secs = 0; secs < 100; secs++) {
      r.pos[0] = modulo(r.pos[0] + r.vel[0], m);
      r.pos[1] = modulo(r.pos[1] + r.vel[1], n);
    }
    if (r.pos[0] < m / 2 and r.pos[1] < n / 2)
      quads[0]++;
    else if (r.pos[0] < m / 2 and r.pos[1] > n / 2)
      quads[1]++;
    else if (r.pos[0] > m / 2 and r.pos[1] < n / 2)
      quads[2]++;
    else if (r.pos[0] > m / 2 and r.pos[1] > n / 2)
      quads[3]++;
  }

  return accumulate(quads.begin(), quads.end(), (ll)1,
                    [](const ll acc, ll e) { return acc * e; });
}

int part2(vector<Robot> robots, int n, int m, int changing_second) {
  for (int secs = 1; secs <= 9000; secs++) {
    for (auto &r : robots) {
      r.pos[0] = modulo(r.pos[0] + r.vel[0], m);
      r.pos[1] = modulo(r.pos[1] + r.vel[1], n);
    }
    if (!changing_second or
        secs == ((secs / 100) * 100 + changing_second + (secs / 100)))
      print_grid(robots, secs, n, m);
  }

  return 0;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto lines = read_input(argv[1]);

  int changing_second = 13; // My input
  if (argc > 2)
    changing_second = stoi(argv[2]);

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
  cout << "Part 2:" << part2(robots, n, m, changing_second) << '\n';
  return 0;
}

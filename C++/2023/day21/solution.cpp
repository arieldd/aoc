#include "utils.h"
#include <unordered_map>

using namespace std;

struct Point {
  int x;
  int y;
  char value;

  bool operator<(const Point p) const {
    return y < p.y || (!(p.y < y) && x < p.x);
  }

  bool operator==(const Point &p) const { return y == p.y && x == p.x; }
};

template <> struct hash<Point> {
  size_t operator()(const Point &p) const { return p.y ^ p.x; }
};

struct GardenMap {
  vector<vector<Point>> grid;
  Point start;
};

GardenMap read_map(const vector<string> &lines) {
  GardenMap res;

  auto n = lines.size();
  for (int i = 0; i < n; i++) {
    vector<Point> row;
    for (int j = 0; j < n; j++) {
      row.push_back({j, i, lines[i][j]});
      if (lines[i][j] == 'S')
        res.start = {j, i, 'S'};
    }
    res.grid.push_back(row);
  }

  return res;
}

void print_set(const set<Point> &reachable, int n) {
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      if (reachable.find(Point{j, i}) != reachable.end()) {
        print('O');
      } else
        print('.');
    }
    println();
  }
  println();
}

int part1(const GardenMap &garden, int steps) {

  unordered_map<Point, int> current_step{{garden.start, 1}};
  auto n = garden.grid.size();

  for (int i = 1; i <= steps; i++) {
    unordered_map<Point, int> next_step;

    for (auto &entry : current_step) {
      for (int d : {0, 2, 4, 6}) {
        int ny = entry.first.y + dy[d], //
            nx = entry.first.x + dx[d];

        Point next{nx, ny};
        if (garden.grid[ny % n][nx % n].value != '#')
          next_step[next]++;
      }
    }

    current_step = next_step;
  }

  //   print_set(current_step, n);
  return current_step.size();
}

vector<int> calculate_odd_pattern(const GardenMap &garden) {
  int n = garden.grid.size(), //
      half = n / 2;

  vector<int> res(n);
  for (int i = 1; i <= half; i++)
    for (auto row : {half - i, half + i}) {
      for (int j = i % 2; j < n; j += 2) {
        if (garden.grid[row][j].value == '.')
          res[row]++;
      }
    }

  for (int j = 0; j < n; j += 2) {
    if (garden.grid[half][j].value == '.')
      res[half]++;
  }
  return res;
}

int64_t part2(const GardenMap &garden, int steps) {

  int64_t n = garden.grid.size(), half = n / 2;
  int64_t steps_taken = half;

  int64_t base = part1(garden, half);
  println(half, " ", base);

  int64_t res = base;

  int64_t i;
  for (i = 2; steps_taken + (i * half) <= steps; i++) {
    steps_taken += i * half;
    res += base * ipow(2, i + 1);

    println(steps_taken, " ", res);
  }

  auto remainder = steps - steps_taken;
  int64_t missing = 0;
  // part1(garden, remainder) * ipow(2, i + 1);

  println(remainder, " ", i * half, " ", remainder - i * half);

  return res + missing;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto steps = stoi(argv[2]);
  auto large_steps = stoi(argv[3]);

  auto garden = read_map(lines);

  auto r1 = part1(garden, steps);
  println("Part 1: ", r1);

  auto r2 = part2(garden, large_steps);
  println("Part 2: ", r2);

  return 0;
}
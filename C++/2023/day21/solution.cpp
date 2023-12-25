#include "utils.h"
#include <cmath>
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
        if (garden.grid[modulo(ny, n)][modulo(nx, n)].value != '#')
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

int64_t rocks_on_step(const GardenMap &garden, int steps) {
  int n = garden.grid.size(), half = n / 2;

  int rocks = 0;
  for (int i = 1; i <= steps; i++)
    for (auto row : {half - i, half + i}) {
      for (int j = i % 2; j < n; j += 2) {
        if (garden.grid[row][j].value == '#')
          rocks++;
      }
    }

  for (int j = 0; j < n; j += 2) {
    if (garden.grid[half][j].value == '#')
      rocks++;
  }
  return rocks;
}

int64_t part2(const GardenMap &garden, int steps) {
  auto n = garden.grid.size();

  int64_t s64 = part1(garden, 64), s65 = part1(garden, 65),
           s196 = part1(garden, 196);

  int64_t double_pattern = (s196 - 4 * s65 - s64) / 2;

  // println(s64, " ", s65, " ", s196, " x + y = ", double_pattern);
  // println(9 * s65 + 4 * s64 + 6 * double_pattern);

  int64_t repeat = (steps - 65) / n;
  println(repeat, " expansions");

  int64_t total_diamonds = pow(2 * repeat + 1, 2),
           total_65 = pow(repeat + 1, 2), total_64 = pow(repeat, 2),
           total_double_pattern = (total_diamonds - total_65 - total_64) / 2;

  println(total_diamonds, " ", total_65, " ", total_64, " ",
          total_double_pattern);

  return total_65 * s65 + total_64 * s64 +
         total_double_pattern * double_pattern;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto steps = stoi(argv[2]);
  auto large_steps = stoi(argv[3]);

  auto garden = read_map(lines);
  println(garden.grid.size());

  auto r1 = part1(garden, steps);
  println("Part 1: ", r1);

  auto r2 = part2(garden, large_steps);
  println("Part 2: ", r2);

  return 0;
}
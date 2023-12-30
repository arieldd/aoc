#include "utils.h"

using namespace std;
using namespace aoc_utils;

using image_t = vector<vector<int>>;

struct galaxy_t {
  int r;
  int c;
};

struct universe_t {
  image_t reading;
  set<int> empty_rows;
  set<int> empty_columns;
};

int64_t distance(const galaxy_t &g1, const galaxy_t &g2,
                 const universe_t &universe, int64_t expansion) {

  auto max_row = max(g1.r, g2.r), max_column = max(g1.c, g2.c);

  auto min_row = min(g1.r, g2.r), min_column = min(g1.c, g2.c);

  int64_t dr = 0;
  for (auto r = min_row + 1; r < max_row; r++)
    if (universe.empty_rows.find(r) != universe.empty_rows.end())
      dr += expansion;

  int64_t dc = 0;
  for (auto c = min_column + 1; c < max_column; c++)
    if (universe.empty_columns.find(c) != universe.empty_columns.end())
      dc += expansion;

  return (max_row + dr - min_row) + (max_column + dc - min_column);
}

universe_t read_universe(const vector<string> &lines) {

  universe_t result;
  bool galaxy = false;

  int i = 0;
  for (auto &l : lines) {
    galaxy = false;
    vector<int> row;
    for (auto &c : l) {
      if (c == '#') {
        row.push_back(1);
        galaxy = true;
      } else
        row.push_back(0);
    }
    result.reading.push_back(row);
    if (!galaxy)
      result.empty_rows.insert(i);
    i++;
  }

  int rows = result.reading.size(), //
      columns = result.reading.front().size();
  for (int j = 0; j < columns; j++) {
    galaxy = false;
    for (int i = 0; i < rows; i++) {
      if (result.reading[i][j]) {
        galaxy = true;
        break;
      }
    }
    if (!galaxy) {
      result.empty_columns.insert(j);
    }
  }

  return result;
}

vector<galaxy_t> find_galaxies(const image_t &universe) {
  vector<galaxy_t> result;
  int n = universe.size(), m = universe.front().size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < m; j++)
      if (universe[i][j])
        result.push_back({i, j});
  }

  return result;
}

int64_t part2(const universe_t &universe, int64_t expansion) {
  auto galaxies = find_galaxies(universe.reading);

  vector<int64_t> distances;

  int count = galaxies.size();
  for (int i = 0; i < count - 1; i++) {
    for (int j = i + 1; j < count; j++) {
      distances.push_back(
          distance(galaxies[i], galaxies[j], universe, expansion));
    }
  }

  return accumulate(distances.begin(), distances.end(), (int64_t)0);

  return 0;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);

  auto universe = read_universe(lines);
  auto r1 = part2(universe, 1);
  println("Part 1: ", r1);

  auto r2 = part2(universe, 999999);
  println("Part 2: ", r2);

  return 0;
}
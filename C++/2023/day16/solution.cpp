#include "utils.h"

using namespace std;
using namespace aoc_utils;

struct Tile {
  int i;
  int j;
  char value;
  bool energized;

  bool operator<(const Tile &other) const {
    return (i < other.i) || (!(other.i < i) && j < other.j);
  }
};

using grid_t = vector<vector<Tile>>;

grid_t parse_tiles(const vector<string> &lines) {
  grid_t result;

  auto n = lines.size();
  for (int i = 0; i < n; i++) {
    vector<Tile> row;

    for (auto j = 0; j < n; j++) {
      row.push_back({i, j, lines[i][j]});
    }

    result.push_back(row);
  }

  return result;
}

void track_beam(grid_t &tiles, int r, int c, int dir, map<Tile, int> &visited) {

  int n = tiles.size(), i = r, j = c;

  while (is_valid_pos(i, j, n, n)) {
    auto current = tiles[i][j];
    if (visited.find(current) != visited.end() && visited.at(current) == dir) {
      break;
    }
    visited[current] = dir;

    tiles[i][j].energized = true;

    if (tiles[i][j].value == '/') {
      if (dir == 0)
        dir = 6;
      else if (dir == 2)
        dir = 4;
      else if (dir == 4)
        dir = 2;
      else
        dir = 0;

    } else if (tiles[i][j].value == '\\') {
      if (dir == 0)
        dir = 2;
      else if (dir == 2)
        dir = 0;
      else if (dir == 4)
        dir = 6;
      else
        dir = 4;

    } else if (tiles[i][j].value == '-') {
      if (dir == 2 || dir == 6) {
        dir = 0;
        auto new_dir = 4;
        track_beam(tiles, i + dy[new_dir], j + dx[new_dir], new_dir, visited);
      }

    } else if (tiles[i][j].value == '|') {
      if (dir == 0 || dir == 4) {
        dir = 2;
        auto new_dir = 6;
        track_beam(tiles, i + dy[new_dir], j + dx[new_dir], new_dir, visited);
      }
    }

    i += dy[dir];
    j += dx[dir];
  }
}

int total_energy(const grid_t &tiles) {
  int energized = 0;
  for (auto &row : tiles) {
    for (auto tile : row) {
      if (tile.energized)
        energized++;
    }
  }

  return energized;
}

int part1(const grid_t &tiles) {

  auto exploring = tiles;
  map<Tile, int> visited;
  track_beam(exploring, 0, 0, 0, visited);

  return total_energy(exploring);
}

int part2(const grid_t &tiles) {
  auto n = tiles.size();

  vector<int> energies;

  for (int i = 0; i < n; i++) {
    map<Tile, int> visited;
    auto exploring = tiles;
    track_beam(exploring, i, 0, 0, visited);
    energies.push_back(total_energy(exploring));

    visited = {};
    exploring = tiles;
    track_beam(exploring, i, n - 1, 4, visited);
    energies.push_back(total_energy(exploring));
  }

  for (int j = 0; j < n; j++) {
    map<Tile, int> visited;
    auto exploring = tiles;

    track_beam(exploring, 0, j, 2, visited);
    energies.push_back(total_energy(exploring));

    visited = {};
    exploring = tiles;

    track_beam(exploring, n - 1, j, 6, visited);
    energies.push_back(total_energy(exploring));
  }

  return *max_element(energies.begin(), energies.end());
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto tiles = parse_tiles(lines);

  auto r1 = part1(tiles);
  println("Part 1: ", r1);

  auto r2 = part2(tiles);
  println("Part 2: ", r2);

  return 0;
}

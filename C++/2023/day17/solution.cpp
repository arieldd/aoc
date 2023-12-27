#include "utils.h"
#include <queue>
#include <unordered_set>

using namespace std;

struct Block {
  int i;
  int j;
  int heat_loss;

  bool operator<(const Block &other) const {
    return (i < other.i) || (!(other.i < i) && j < other.j);
  }

  bool operator==(const Block &other) const {
    return i == other.i && j == other.j;
  }

  int distance(const int r, const int c) const {
    return abs(i - r) + abs(j - c);
  }
};

using grid_t = vector<vector<Block>>;
struct Vertex {
  int i;
  int j;
  int dir;
  int steps;
  int total_loss;

  bool operator==(const Vertex &v) const {
    return i == v.i && j == v.j && dir == v.dir && steps && v.steps;
  }

  bool operator<(const Vertex &v) const {
    return total_loss > v.total_loss; // For min heap
  }
};

ostream &operator<<(ostream &os, const Vertex &v) {
  return (os << "(" << v.i << "," << v.j << ") " << v.dir << " " << v.steps
             << " " << v.total_loss);
}

template <> struct hash<Vertex> {
  size_t operator()(const Vertex &v) const {
    size_t res = 0;
    hash_combine(res, v.i);
    hash_combine(res, v.j);
    hash_combine(res, v.dir);
    hash_combine(res, v.steps);
    return res;
  }
};

grid_t read_map(const vector<string> &lines) {
  grid_t result;

  auto n = lines.size();

  for (int i = 0; i < n; i++) {
    vector<Block> row;

    for (int j = 0; j < n; j++) {

      row.push_back({i, j, lines[i][j] - '0'});
    }
    result.push_back(row);
  }

  return result;
}

void print_map(const grid_t &city, const Block &current) {
  for (auto &row : city) {
    for (auto block : row) {
      if (block == current)
        print('$');
      else
        print(block.heat_loss);
    }
    println();
  }
  println();
}

void print_path(const grid_t &city, vector<Vertex> path) {
  for (auto &row : city) {
    for (auto block : row) {
      if (auto it = find(path.begin(), path.end(), Vertex{block.i, block.j});
          it != path.end()) {
        char dir = '>';
        if (it->dir == 2)
          dir = 'v';
        else if (it->dir == 4)
          dir = '<';
        else if (it->dir == 6)
          dir = '^';
        print(dir);
      } else
        print('.');
      // print(block.heat_loss);
    }
    println();
  }
  println();
}

void find_shortest_paths(const grid_t &city, const Vertex &src,
                         unordered_set<Vertex> &visited, int min_steps,
                         int max_steps) {

  auto n = city.size();
  priority_queue<Vertex> exploring;

  exploring.push(src);

  int ends_found = 0;
  while (!exploring.empty()) {
    auto current = exploring.top();
    exploring.pop();

    if (visited.find(current) != visited.end())
      continue; // If we have seen it before this one has higher value and is a
                // repeat, discard.

    visited.insert(current);

    if (current.steps < max_steps) {
      auto ni = current.i + dy[current.dir], nj = current.j + dx[current.dir];

      if (is_valid_pos(ni, nj, n, n)) {
        Vertex next{ni, nj, current.dir, current.steps + 1,
                    current.total_loss + city[ni][nj].heat_loss};

        exploring.push(next);
      }
    }
    for (auto dir : {(current.dir + 2) % 8, (current.dir + 6) % 8}) {
      auto ni = current.i + dy[dir] * min_steps,
           nj = current.j + dx[dir] * min_steps;

      if (is_valid_pos(ni, nj, n, n)) {
        int accumulated_loss = current.total_loss;
        for (auto k = 1; k < min_steps; k++)
          accumulated_loss +=
              city[current.i + dy[dir] * k][current.j + dx[dir] * k].heat_loss;

        Vertex next{ni, nj, dir, min_steps,
                    accumulated_loss + city[ni][nj].heat_loss};

        exploring.push(next);
      }
    }
  }
}

int part1(const grid_t &city, int min_steps, int max_steps) {
  int n = city.size();
  unordered_set<Vertex> visited;
  Vertex src{0, 0, 0, 0, 0};

  find_shortest_paths(city, src, visited, min_steps, max_steps);

  vector<int> end_path_lengths;
  for (auto &dir : {0, 2}) {
    for (auto steps = min_steps; steps <= max_steps; steps++) {
      Vertex end{n - 1, n - 1, dir, steps};
      if (auto it = visited.find(end); it != visited.end()) {
        end_path_lengths.push_back(it->total_loss);
      }
    }
  }

  return *min_element(end_path_lengths.begin(), end_path_lengths.end());
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto city = read_map(lines);

  auto r1 = part1(city, 1, 3);
  println("Part 1: ", r1);

  auto r2 = part1(city, 4, 10);
  println("Part 2: ", r2);

  return 0;
}
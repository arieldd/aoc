#include "utils.h"
#include <queue>

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

  // bool operator==(const Vertex &v) const {
  //   return i == v.i && j == v.j && dir == v.dir && steps && v.steps;
  // }

  bool operator<(const Vertex &v) const {
    return i < v.i || (!(v.i < i) && j < v.j);
  }
};

bool operator==(const Vertex &lhs, const Vertex &rhs) {
  return lhs.i == rhs.i && lhs.j == rhs.j && lhs.dir == rhs.dir && lhs.steps &&
         rhs.steps;
}

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

vector<vector<Vertex>> calculate_lower_bound(const grid_t &city,
                                             const Block &end) {
  auto n = city.size();

  vector<vector<Vertex>> heat_loss_map(n);
  for (int i = 0; i < n; i++) {
    vector<Vertex> row(n);
    heat_loss_map[i] = row;
    for (int j = 0; j < n; j++) {
      heat_loss_map[i][j] = {i, j, 0, 0, INT_MAX};
    }
  }
  heat_loss_map[end.i][end.j].total_loss = 0; // city[end.i][end.j].heat_loss;

  auto comp = [&](Vertex &left, Vertex &right) {
    return heat_loss_map[left.i][left.j].total_loss >
           heat_loss_map[right.i][right.j].total_loss;
  };

  priority_queue<Vertex, vector<Vertex>, decltype(comp)> exploring(comp);
  exploring.push(heat_loss_map[end.i][end.j]);

  while (!exploring.empty()) {
    auto current = exploring.top();
    exploring.pop();

    for (auto d : {0, 2, 4, 6}) {
      auto ni = current.i + dy[d], nj = current.j + dx[d];
      if (is_valid_pos(ni, nj, n, n)) {
        Vertex previous = {ni, nj, current.dir, current.steps,
                           current.total_loss +
                               city[current.i][current.j].heat_loss};

        if (heat_loss_map[ni][nj].total_loss > previous.total_loss) {
          heat_loss_map[ni][nj] = previous;
          exploring.push(previous);
        }
      }
    }
  }
  // for (auto &row : heat_loss_map) {
  //   for (auto &p : row) {
  //     if (p.total_loss == INT_MAX)
  //       print("?    ");
  //     else
  //       print(p.total_loss, "    ");
  //   }
  //   println();
  // }
  // println("-----");

  return heat_loss_map;
}

int shortest_path_a_star(const grid_t &city, const Vertex &start,
                         const Vertex &end,
                         const vector<vector<Vertex>> &lower_bound) {

  auto n = city.size();
  map<Vertex, int> gScore, fScore;
  vector<Vertex> openSet{start};

  for (auto &row : city)
    for (auto block : row) {
      Vertex v = {block.i, block.j};
      gScore[v] = INT_MAX;
      fScore[v] = INT_MAX;
    }

  gScore[start] = 0;
  fScore[start] = lower_bound[start.i][start.j].total_loss;

  while (!openSet.empty()) {
    int min_pos = 0, min_score = INT_MAX;

    for (int i = 0; i < openSet.size(); i++) {
      Vertex v = openSet[i];
      if (fScore[v] < min_score) {
        min_score = fScore[v];
        min_pos = i;
      }
    }

    Vertex current = openSet[min_pos];

    if (current.i == end.i && current.j == end.j && current.dir == end.dir)
      return current.total_loss;

    openSet.erase(openSet.begin() + min_pos);

    auto n = city.size();
    if (current.steps < 3) {
      auto ni = current.i + dy[current.dir], nj = current.j + dx[current.dir];
      if (is_valid_pos(ni, nj, n, n)) {
        Vertex neighbor = {ni, nj, current.dir, current.steps + 1,
                           current.total_loss + city[ni][nj].heat_loss};

        if (neighbor.total_loss < gScore[neighbor]) {
          gScore[neighbor] = neighbor.total_loss;
          fScore[neighbor] =
              neighbor.total_loss + lower_bound[ni][nj].total_loss;
          if (find(openSet.begin(), openSet.end(), neighbor) == openSet.end())
            openSet.push_back(neighbor);
        }
      }
    }

    for (auto next_dir : {(current.dir + 2) % 8, (current.dir + 6) % 8}) {
      auto ni = current.i + dy[next_dir], nj = current.j + dx[next_dir];
      if (is_valid_pos(ni, nj, n, n)) {
        Vertex neighbor = {ni, nj, next_dir, 1,
                           current.total_loss + city[ni][nj].heat_loss};

        if (neighbor.total_loss < gScore[neighbor]) {
          gScore[neighbor] = neighbor.total_loss;
          fScore[neighbor] =
              neighbor.total_loss + lower_bound[ni][nj].total_loss;
          if (find(openSet.begin(), openSet.end(), neighbor) == openSet.end())
            openSet.push_back(neighbor);
        }
      }
    }
  }

  return 0;
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

bool has_fat_head(const vector<Vertex> &path) {
  auto l = path.size();

  if (l < 4)
    return false;

  auto v1 = path.back(), v4 = path[l - 4];

  if ((v1.i == v4.i && abs(v1.j - v4.j) == 1) ||
      (v1.j == v4.j && abs(v1.i - v4.i) == 1))
    return true;

  return false;
}

void push_crucible(const grid_t &city, vector<Vertex> path, const Block &end,
                   vector<vector<Vertex>> &min_heat_map,
                   const vector<vector<Vertex>> &lower_bound) {

  Vertex current = path.back();

  if (Block{current.i, current.j} == end) {
    if (min_heat_map[end.i][end.j].total_loss > current.total_loss) {
      println("Updated min to ", current.total_loss);
      for (auto &p : path)
        min_heat_map[p.i][p.j] = p;

      print_path(city, path);
    }
    return;
  }

  if (current.total_loss + lower_bound[current.i][current.j].total_loss >
      min_heat_map[end.i][end.j].total_loss) { // Discard, won't make it
    return;
  }

  if (has_fat_head(path))
    return;

  auto n = city.size();
  if (current.steps < 3) {
    auto ni = current.i + dy[current.dir], nj = current.j + dx[current.dir];
    if (is_valid_pos(ni, nj, n, n)) {
      Vertex next = {ni, nj, current.dir, current.steps + 1,
                     current.total_loss + city[ni][nj].heat_loss};
      if (find(path.begin(), path.end(), next) == path.end()) {
        auto new_path = path;
        new_path.push_back(next);
        push_crucible(city, new_path, end, min_heat_map, lower_bound);
      }
    }
  }

  auto dir1 = (current.dir + 2) % 8, dir2 = (current.dir + 6) % 8;
  for (auto next_dir : {dir1, dir2}) { //{min(dir1, dir2), max(dir1, dir2)}) {
    auto ni = current.i + dy[next_dir], nj = current.j + dx[next_dir];
    if (is_valid_pos(ni, nj, n, n)) {
      Vertex next = {ni, nj, next_dir, 1,
                     current.total_loss + city[ni][nj].heat_loss};
      if (find(path.begin(), path.end(), next) == path.end()) {
        auto new_path = path;
        new_path.push_back(next);
        push_crucible(city, new_path, end, min_heat_map, lower_bound);
      }
    }
  }
}

int part1(const grid_t &city) {
  auto n = city.size();
  int min_loss = 1270;

  Block end = city[n - 1][n - 1];
  auto lower_bound = calculate_lower_bound(city, end);

  vector<vector<Vertex>> min_heat_map(n);
  for (int i = 0; i < n; i++) {
    vector<Vertex> row(n);
    min_heat_map[i] = row;
    for (int j = 0; j < n; j++) {
      min_heat_map[i][j] = {i, j, 0, 0, INT_MAX};
    }
  }
  min_heat_map[0][0].total_loss = 0;
  // min_heat_map[n - 1][n - 1].total_loss = 1263;

  Vertex start = {0, 0, 0, 0, 0};
  // push_crucible(city, {start}, end, min_heat_map, lower_bound);
  return shortest_path_a_star(city, start, Vertex{n - 1, n - 1, 0},
                              lower_bound);

  // return min_heat_map[n - 1][n - 1].total_loss;
}

int part2(const vector<string> &lines) { return 0; }

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto city = read_map(lines);

  vector<Vertex> test = {Vertex{0, 0, 1, 0}};

  println(find(test.begin(), test.end(), Vertex{0, 0, 1, 0}) == test.end());

  auto r1 = part1(city);
  println("Part 1: ", r1);

  auto r2 = part2(lines);
  println("Part 2: ", r2);

  return 0;
}
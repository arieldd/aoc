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

  bool operator==(const Vertex &other) const {
    return i == other.i && j == other.j;
  }
};

bool vertex_cmp(const Vertex &lhs, const Vertex &rhs) {
  return lhs.total_loss < rhs.total_loss;
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

// vector<vector<Vertex>> calculate_distance(const grid_t &city,
//                                           const Block &end) {
//   auto n = city.size();

//   vector<vector<Vertex>> heat_loss_map(n);
//   for (int i = 0; i < n; i++) {
//     vector<Vertex> row(n);
//     heat_loss_map[i] = row;
//     for (int j = 0; j < n; j++) {
//       Block current = city[i][j];
//       heat_loss_map[i][j] = {current.i, current.j, 0, 0, INT_MAX};
//     }
//   }
//   heat_loss_map[end.i][end.j].total_loss = 0;

//   priority_queue<Vertex> exploring;
//   exploring.push(heat_loss_map[end.i][end.j]);

//   while (!exploring.empty()) {
//     auto current = exploring.top();
//     exploring.pop();

//     for (auto d : {0, 2, 4, 6}) {
//       auto ni = current.i + dy[d], nj = current.j + dx[d];
//       if (is_valid_pos(ni, nj, n, n)) {
//         Vertex previous = {ni, nj, current.dir, current.steps,
//                            current.total_loss +
//                                city[current.i][current.j].heat_loss};

//         println(current.total_loss, " ",
//                 current.total_loss + city[current.i][current.j].heat_loss, "
//                 ", city[current.i][current.j].heat_loss);

//         // if (current.steps[d / 2] < 3) {
//         //   previous.steps[d / 2] += 1;
//         if (heat_loss_map[ni][nj].total_loss > previous.total_loss) {
//           heat_loss_map[ni][nj] = previous;
//           exploring.push(previous);
//         }
//         //}
//       }
//     }
//     print(current.i, " ", current.j);

//     println();
//     for (auto &row : heat_loss_map) {
//       for (auto &p : row) {
//         if (p.total_loss == INT_MAX)
//           print("\t?");
//         else
//           print("\t", p.total_loss);
//       }
//       println();
//     }
//     println("-----");
//   }

//   return heat_loss_map;
// }

// int upper_bound_loss(const grid_t &city, const Block &start, const Block
// &end) {
//   Block current = start;
//   current.j++;

//   int steps = 1, dir = 0, i, j;

//   int loss = 0;
//   while (!(current == end)) {
//     if (steps >= 3) {
//       dir = (dir == 0) ? 2 : 0;
//       steps = 1;
//     }

//     i = current.i + dy[dir];
//     j = current.j + dx[dir];

//     loss += city[i][j].heat_loss;

//     current = city[i][j];

//     steps++;
//   }

//   return loss;
// }

// void least_loss_path(const grid_t &city, vector<Vertex> path, const Block
// &end,
//                      int &min_loss) {
//   auto current = path.back();

//   if (city[current.i][current.j] == end) {
//     if (current.total_loss < min_loss) {
//       min_loss = current.total_loss;
//     }
//     return;
//   }

//   if (end.distance(current.i, current.j) + current.total_loss > min_loss)
//     return;

//   // for (auto p : path) {
//   //   print(p.i, " ", p.j, " ", p.dir, " ", p.steps, " ", p.total_loss, " |
//   ");
//   // }
//   // println();

//   int next_i, next_j, next_dir, n = city.size();

//   if (current.steps < 3) {
//     next_i = current.i + dy[current.dir];
//     next_j = current.j + dx[current.dir];

//     if (is_valid_pos(next_i, next_j, n, n)) {
//       Block next = city[next_i][next_j];
//       Vertex p{next_i, next_j, current.dir, current.steps + 1,
//                current.total_loss + next.heat_loss};

//       if (find(path.begin(), path.end(), p) == path.end()) {
//         auto new_path = path;
//         new_path.push_back(p);
//         least_loss_path(city, new_path, end, min_loss);
//       }
//     }
//   }

//   for (auto d : {2, 6}) {
//     next_dir = (current.dir + d) % 8;
//     next_i = current.i + dy[next_dir];
//     next_j = current.j + dx[next_dir];

//     if (is_valid_pos(next_i, next_j, n, n)) {
//       Block next = city[next_i][next_j];
//       Vertex p{next_i, next_j, next_dir, 1,
//                current.total_loss + next.heat_loss};

//       if (find(path.begin(), path.end(), p) == path.end()) {
//         auto new_path = path;
//         new_path.push_back(p);
//         least_loss_path(city, new_path, end, min_loss);
//       }
//     }
//   }
// }

int move_crucible_dijkstra(const grid_t &city, Block start, const Block &end) {

  int next_i, next_j, next_dir, n = city.size();

  Vertex first{start.i, start.j, 0, 0, 0};
  vector<Vertex> exploring;

  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      Vertex current{i, j, 0, 0, INT_MAX};
      if (i == start.i && j == start.j)
        current.total_loss = 0;
      exploring.push_back(current);
    }
  }

  Vertex end_vertex = exploring.back();
  while (!exploring.empty()) {
    auto it = min_element(exploring.begin(), exploring.end(), vertex_cmp);
    Vertex current = *it;

    exploring.erase(it);

    println(current.i, " ", current.j, " ", current.total_loss);

    if (Block{current.i, current.j} == end) {
      end_vertex = current;
    }

    if (current.steps < 3) {
      next_i = current.i + dy[current.dir];
      next_j = current.j + dx[current.dir];

      if (is_valid_pos(next_i, next_j, n, n)) {
        Block next = city[next_i][next_j];
        Vertex p{next_i, next_j, current.dir, current.steps + 1,
                 current.total_loss + next.heat_loss};
        auto neighbour = find(exploring.begin(), exploring.end(), p);

        if (neighbour != exploring.end() &&
            neighbour->total_loss > p.total_loss)
          *neighbour = p;
      }
    }
    for (auto d : {2, 6}) {
      next_dir = (current.dir + d) % 8;
      next_i = current.i + dy[next_dir];
      next_j = current.j + dx[next_dir];

      if (is_valid_pos(next_i, next_j, n, n)) {
        Block next = city[next_i][next_j];
        Vertex p{next_i, next_j, next_dir, 1,
                 current.total_loss + next.heat_loss};

        auto neighbour = find(exploring.begin(), exploring.end(), p);

        if (neighbour != exploring.end() &&
            neighbour->total_loss > p.total_loss)
          *neighbour = p;
      }
    }
  }
  return end_vertex.total_loss;
}

// int move_crucible(const grid_t &city, Block start, const Block &end,
//                   int min_loss) {

//   int next_i, next_j, next_dir, n = city.size();

//   vector<vector<Vertex>> heat_map, distances = calculate_distance(city, end);
//   for (auto row : city) {
//     vector<Vertex> heat_row;
//     for (auto e : row)
//       heat_row.push_back({e.i, e.j, 0, 0, INT_MAX});
//     heat_map.push_back(heat_row);
//   }

//   heat_map.front().front().total_loss = 0;
//   heat_map.back().back().total_loss = min_loss;

//   Vertex first{start.i, start.j, 0, 0, 0};
//   deque<Vertex> exploring;
//   exploring.push_back(first);

//   while (!exploring.empty()) {
//     Vertex current = exploring.back();
//     exploring.pop_back();

//     if (current.steps < 3) {
//       next_i = current.i + dy[current.dir];
//       next_j = current.j + dx[current.dir];

//       if (is_valid_pos(next_i, next_j, n, n)) {
//         Block next = city[next_i][next_j];
//         Vertex p{next_i, next_j, current.dir, current.steps + 1,
//                  current.total_loss + next.heat_loss};

//         if (next == end) {
//           if (p.total_loss < heat_map.back().back().total_loss) {
//             heat_map.back().back() = p;
//           }

//           continue;

//         } else if (p.total_loss + distances[p.i][p.j] <
//                    heat_map.back().back().total_loss) {

//           if ((p.total_loss < heat_map[p.i][p.j].total_loss))
//             heat_map[p.i][p.j] = p;
//           exploring.push_back(p);
//         }
//       }
//     }

//     for (auto d : {2, 6}) {
//       next_dir = (current.dir + d) % 8;
//       next_i = current.i + dy[next_dir];
//       next_j = current.j + dx[next_dir];

//       if (is_valid_pos(next_i, next_j, n, n)) {
//         Block next = city[next_i][next_j];
//         Vertex p{next_i, next_j, next_dir, 1,
//                  current.total_loss + next.heat_loss};

//         if (next == end) {
//           if (p.total_loss < heat_map.back().back().total_loss) {
//             heat_map.back().back() = p;
//           }

//           break;

//         } else if (p.total_loss + distances[p.i][p.j] <
//                    heat_map.back().back().total_loss) {
//           if ((p.total_loss < heat_map[p.i][p.j].total_loss))
//             heat_map[p.i][p.j] = p;
//           exploring.push_back(p);
//         }
//       }
//     }
//   }

//   return heat_map.back().back().total_loss;
// }

int part1(const grid_t &city) {
  auto n = city.size();
  int min_loss = 1270; // upper_bound_loss(city, city[0][0], city[n - 1][n -
                       // 1]);

  Block end = city[n - 1][n - 1];

  return move_crucible_dijkstra(city, city[0][0], end);

  // least_loss_path(city,
  //                 {
  //                     {0, 0, 0, 0, 0},
  //                 },
  //                 city.back().back(), min_loss);

  // return min_loss;
}

int part2(const vector<string> &lines) { return 0; }

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto city = read_map(lines);

  auto r1 = part1(city);
  println("Part 1: ", r1);

  auto r2 = part2(lines);
  println("Part 2: ", r2);

  return 0;
}
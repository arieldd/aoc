#include "utils.h"

using namespace std;

const map<char, int> downhil{{'>', 0}, {'v', 2}, {'<', 4}, {'^', 6}};

const vector<int> directions{0, 2, 4, 6};

struct Tile {
  int i;
  int j;
  char type;

  bool is_slope() const { return downhil.find(type) != downhil.end(); };

  bool operator<(const Tile &t) const {
    return i < t.i || (!(t.i < i) && j < t.j);
  }

  bool operator==(const Tile &t) const { return i == t.i && j == t.j; }
};

struct Junction {
  Tile tile;
  map<Junction, int> neightbours;

  bool operator<(const Junction &j) const { return tile < j.tile; }

  bool operator==(const Junction &j) const { return tile == j.tile; }
};

vector<vector<Tile>> read_trail(const vector<string> &lines) {
  vector<vector<Tile>> trail;

  auto n = lines.size(), m = lines[0].size();
  for (int i = 0; i < n; i++) {
    auto &l = lines[i];
    vector<Tile> row;
    for (int j = 0; j < m; j++) {
      row.push_back({i, j, l[j]});
    }
    trail.push_back(row);
  }

  return trail;
}

void print_path(const vector<vector<Tile>> trail, const vector<Tile> &path) {
  auto n = trail.size();

  for (auto &row : trail) {
    for (auto &t : row) {
      if (find(path.begin(), path.end(), t) != path.end())
        print('O');
      else
        print(t.type);
    }
    println();
  }
  println();
}

int find_longest_icy_path(const vector<vector<Tile>> trail, const Tile &end,
                          vector<Tile> path) {
  auto current = path.back();

  if (current == end) {
    return path.size();
  }

  auto n = trail.size();

  int max_steps = path.size();
  for (auto dir : directions) {
    if (current.is_slope() && dir != downhil.at(current.type))
      continue; // Have to keep direction on slope tiles

    auto ni = current.i + dy[dir], nj = current.j + dx[dir];
    if (is_valid_pos(ni, nj, n, n) && trail[ni][nj].type != '#') {
      Tile next = trail[ni][nj];
      if (next.is_slope() and downhil.at(next.type) == (dir + 4) % 8)
        continue; // Don't step on a forced turn back slope

      if (find(path.begin(), path.end(), next) != path.end())
        continue;

      auto next_path = path;
      next_path.push_back(next);
      auto steps = find_longest_icy_path(trail, end, next_path);

      if (steps > max_steps)
        max_steps = steps;
    }
  }

  return max_steps;
}

vector<Tile> find_exits(const vector<vector<Tile>> &trail,
                        const Tile &current) {
  auto n = trail.size();

  if (current.type == '#')
    return {};

  vector<Tile> result;
  for (auto dir : directions) {
    auto ni = current.i + dy[dir], nj = current.j + dx[dir];
    if (is_valid_pos(ni, nj, n, n) && trail[ni][nj].type != '#')
      result.push_back(trail[ni][nj]);
  }

  return result;
}

set<Tile> find_junction_tiles(const vector<vector<Tile>> &trail) {
  auto n = trail.size();

  set<Tile> junctions;

  for (int i = 1; i < n; i++) {
    for (int j = 1; j < n; j++) {
      auto exits = find_exits(trail, trail[i][j]);

      if (exits.size() > 2) {
        junctions.insert(trail[i][j]);
      }
    }
  }

  return junctions;
}

set<Junction> fill_junction_graph(const vector<vector<Tile>> &trail,
                                  const set<Tile> &junction_tiles,
                                  const Tile &start, const Tile &end) {

  Junction first{start}, last{end};
  set<Junction> result;

  for (auto junction : junction_tiles) {
    Junction exploring = {junction};
    auto j_exits = find_exits(trail, junction);

    for (auto &tile : j_exits) {
      auto current = tile;

      int steps = 1;
      Tile previous = junction;
      for (;;) {

        auto t_exits = find_exits(trail, current);

        if (t_exits.size() >= 3) {
          exploring.neightbours[{current}] = steps;
          break;
        }

        if (t_exits.size() == 1) // Only the 2 spcial ones
        {
          Junction &extreme = (current == start) ? first : last;

          extreme.neightbours[exploring] = steps;
          exploring.neightbours[extreme] = steps;
          break;
        }

        for (auto t : t_exits) {
          if (t == previous)
            continue;
          previous = current;
          current = t;
          break;
        }
        steps++;
      }
    }
    result.insert(exploring);
  }
  result.insert(first);
  result.insert(last);

  return result;
}

vector<Tile> find_longest_dry_path(const vector<vector<Tile>> &trail,
                                   const Tile &end, vector<Tile> path) {
  auto n = trail.size();
  auto new_trail = trail;
  for (;;) {
    auto current = path.back();
    // println("Entered ", current.i, ",", current.j);

    if (current == end) {
      return path;
    }

    auto max_path = path;
    auto exits = find_exits(new_trail, current);

    new_trail[current.i][current.j].type = '#';

    int exit_taken = 0;
    for (auto &next : exits) {

      if (exits.size() < 2) {
        path.push_back(next);
      } else {
        // println("Junction at ", current.i, ",", current.j);

        auto next_path = path;
        next_path.push_back(next);
        auto end_path = find_longest_dry_path(new_trail, end, next_path);

        if (end_path.back() == end && end_path.size() > max_path.size())
          max_path = end_path;
      }
    }
    if (exits.size() >= 2 || exits.size() == 0) {
      //   print_path(new_trail, max_path);
      return max_path;
    }
  }
}

int find_longest_dry_path_from_junctions(const set<Junction> graph,
                                         const Tile &end, vector<Junction> path,
                                         const Junction &last_before_end,
                                         int path_length) {
  if (path.back().tile == end) {
    return path_length;
  }

  auto current = *graph.find(path.back());
  // println("Checking ", current.tile.i, ",", current.tile.j);

  int max_steps = 0;
  for (auto &pair : current.neightbours) {
    // println("Checking neioghbor ", pair.first.tile.i, ",",
    // pair.first.tile.j);

    if (find(path.begin(), path.end(), pair.first) != path.end())
      continue;

    auto new_path = path;
    new_path.push_back(pair.first);
    int steps = 0;
    if (pair.first == last_before_end) {
      steps = path_length + pair.second +
              graph.find({end})->neightbours.begin()->second;
    } else
      steps = find_longest_dry_path_from_junctions(
          graph, end, new_path, last_before_end, path_length + pair.second);

    if (steps > max_steps)
      max_steps = steps;
  }

  return max_steps;
}

int part1(const vector<vector<Tile>> &trail) {
  auto n = trail.size();
  return find_longest_icy_path(trail, trail[n - 1][n - 2], {trail[0][1]}) - 1;
};

int part2(const vector<vector<Tile>> &trail) {
  int n = trail.size();

  Tile start = {0, 1}, end = {n - 1, n - 2};

  auto junctions = find_junction_tiles(trail);
  auto graph = fill_junction_graph(trail, junctions, start, end);

  for (auto &j : graph) {
    print("(", j.tile.i, ",", j.tile.j, ") : ");
    for (auto &pair : j.neightbours) {
      print("(", pair.first.tile.i, ",", pair.first.tile.j, ") ", pair.second,
            " ");
    }
    println();
  }

  auto last_before_end = graph.find({end})->neightbours.begin()->first;
  return find_longest_dry_path_from_junctions(graph, end, {{start}},
                                              last_before_end, 0);
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto trail = read_trail(lines);

  // auto r1 = part1(trail);
  // println("Part 1: ", r1);

  auto r2 = part2(trail);
  println("Part 2: ", r2);

  return 0;
}
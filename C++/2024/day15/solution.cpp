#include "utils.h"
#include <cassert>
using namespace std;
using namespace aoc_utils;

#define ll long long
#define arr array

struct Box {
  arr<int, 2> p1, p2;
};

struct Warehouse {
  vector<vector<char>> grid;
  vector<int> moves;
  int n, m;
  arr<int, 2> robot;
  map<arr<int, 2>, Box> boxes;

  bool can_move_box(const Box &box, int dir) {
    arr<int, 2> n1 = {box.p1[0] + adj4[dir].first,
                      box.p1[1] + adj4[dir].second};
    arr<int, 2> n2 = {box.p2[0] + adj4[dir].first,
                      box.p2[1] + adj4[dir].second};

    if (grid[n1[0]][n1[1]] != '#' and grid[n2[0]][n2[1]] != '#' and
        (dir == 0 or // n1 != n2
         (!boxes.contains(n1) or can_move_box(boxes.at(n1), dir))) and
        (dir == 2 or // n2 != n1
         (!boxes.contains(n2) or can_move_box(boxes.at(n2), dir)))) {
      return true;
    }
    return false;
  }

  void move_box(Box box, int dir) {

    arr<int, 2> n1 = {box.p1[0] + adj4[dir].first,
                      box.p1[1] + adj4[dir].second};
    arr<int, 2> n2 = {box.p2[0] + adj4[dir].first,
                      box.p2[1] + adj4[dir].second};
    if (dir != 0 and boxes.contains(n1))
      move_box(boxes.at(n1), dir);
    if (dir != 2 and boxes.contains(n2))
      move_box(boxes.at(n2), dir);
    boxes.erase(box.p1);
    boxes.erase(box.p2);

    Box new_box = {n1, n2};
    boxes.insert({n1, new_box});
    boxes.insert({n2, new_box});
  }

  bool move_robot(int dir) {
    int ni = robot[0] + adj4[dir].first, nj = robot[1] + adj4[dir].second;
    if (grid[ni][nj] != '#' and
        (!boxes.contains({ni, nj}) or can_move_box(boxes.at({ni, nj}), dir))) {
      if (boxes.contains({ni, nj}))
        move_box(boxes.at({ni, nj}), dir);
      robot = {ni, nj};
      return true;
    }

    return false;
  }
};

Warehouse read_input(const string &filename) {
  Warehouse result;
  ifstream fs(filename);

  string line;
  bool reading_grid = true;
  int i = 0;
  while (getline(fs, line)) {
    if (!line.size()) {
      reading_grid = false;
      continue;
    }
    if (reading_grid) {
      result.grid.push_back({});
      for (int j = 0; j < line.size(); j++) {
        result.grid.back().push_back(line[j]);
        if (line[j] == '@') {
          result.robot[0] = i;
          result.robot[1] = j;
        }
      }
      i++;
    } else {
      for (auto c : line) {
        switch (c) {
        case '>':
          result.moves.push_back(0);
          break;
        case 'v':
          result.moves.push_back(1);
          break;
        case '<':
          result.moves.push_back(2);
          break;
        case '^':
          result.moves.push_back(3);
          break;
        default:
          assert(0 && "Woops");
          break;
        }
      }
    }
  }
  result.n = result.grid.size();
  result.m = result.grid[0].size();
  return result;
}

bool move_small(vector<vector<char>> &grid, int i, int j, int n, int m,
                int dir) {
  int ni = i, nj = j;
  while (grid[ni][nj] != '#' and grid[ni][nj] != '.') {
    ni += adj4[dir].first, nj += adj4[dir].second;
  }
  if (grid[ni][nj] != '.')
    return false;

  int back = (dir + 2) % 4;
  while (ni != i or nj != j) {
    int ci = ni + adj4[back].first, cj = nj + adj4[back].second;
    grid[ni][nj] = grid[ci][cj];
    ni = ci;
    nj = cj;
  }
  grid[i][j] = '.';
  return true;
}

int part1(const Warehouse &input) {
  int gps_sum = 0;
  vector<vector<char>> grid = input.grid;
  arr<int, 2> robot = input.robot;

  for (auto dir : input.moves) {
    if (move_small(grid, robot[0], robot[1], input.n, input.m, dir)) {
      robot[0] += adj4[dir].first;
      robot[1] += adj4[dir].second;
    }
  }
  for (int i = 1; i < input.n; i++) {
    for (int j = 1; j < input.m; j++) {
      if (grid[i][j] == 'O') {
        gps_sum += i * 100 + j;
      }
    }
  }
  return gps_sum;
}

Warehouse enlarge_grid(const Warehouse &input) {
  Warehouse large;

  large.moves = input.moves;
  large.n = input.n;
  large.m = input.m * 2;
  large.robot = input.robot;
  large.robot[1] *= 2;

  for (int i = 0; i < input.n; i++) {
    large.grid.push_back({});
    int j = 0;
    for (auto elem : input.grid[i]) {
      switch (elem) {
      case '#': {
        large.grid.back().push_back(elem);
        large.grid.back().push_back(elem);
        break;
      }
      case '.': {
        large.grid.back().push_back(elem);
        large.grid.back().push_back(elem);
        break;
      }
      case '@': {
        large.grid.back().push_back(elem);
        large.grid.back().push_back('.');
        break;
      }
      case 'O': {
        large.grid.back().push_back('[');
        large.grid.back().push_back(']');
        Box box = {.p1 = {i, j}, .p2 = {i, j + 1}};
        large.boxes.insert({box.p1, box});
        large.boxes.insert({box.p2, box});
        break;
      }
      default:
        assert(0 && "Woops!");
      }
      j += 2;
    }
  }
  return large;
}

ll part2(const Warehouse &input) {
  auto large = enlarge_grid(input);
  for (auto dir : large.moves) {
    large.move_robot(dir);
  }

  // Faster than going through the map removing duplicates we simply
  // set the right values om the final grid and count like part 1.
  for (int i = 0; i < large.n; i++) {
    for (int j = 0; j < large.m; j++) {
      if (large.grid[i][j] != '#')
        large.grid[i][j] = '.';
    }
  }

  for (auto entry : large.boxes) {
    large.grid[entry.second.p1[0]][entry.second.p1[1]] = '[';
    large.grid[entry.second.p2[0]][entry.second.p2[1]] = ']';
  }
  large.grid[large.robot[0]][large.robot[1]] = '@';

  ll gps_sum = 0;
  for (int i = 1; i < large.n; i++) {
    for (int j = 1; j < large.m; j++) {
      if (large.grid[i][j] == '[') {
        gps_sum += i * 100 + j;
      }
    }
  }
  return gps_sum;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto wares = read_input(argv[1]);

  cout << "Part 1:" << part1(wares) << '\n';
  cout << "Part 2:" << part2(wares) << '\n';
  return 0;
}

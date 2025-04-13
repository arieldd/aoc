#include <algorithm>
#include <climits>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

#include <sstream>
#include <unordered_map>
#include <unordered_set>

struct coord_t {
  int i;
  int j;

  int steps;

  bool operator==(const coord_t &other) const {
    return i == other.i && j == other.j;
  }

  coord_t operator+(const coord_t &other) {
    coord_t ret{
        .i = i + other.i, .j = j + other.j, .steps = steps + other.steps};

    return ret;
  }
};

int manhattan_dist(const coord_t &c1, const coord_t &c2) {
  return abs(c1.i - c2.i) + abs(c1.j - c2.j);
}

namespace std {
template <> struct hash<coord_t> {
  std::size_t operator()(const coord_t &c) const noexcept {
    size_t hash = c.i + 10 * c.j;
    return hash;
  }
};
} // namespace std

enum dir { R, L, U, D };

struct move_t {
  dir to;
  int amount;
};

std::unordered_map<dir, coord_t, std::hash<int>> shifts = {
    {R, {0, 1}}, {L, {0, -1}}, {U, {1, 0}}, {D, {-1, 0}}};

using path_t = std::vector<move_t>;

std::vector<path_t> parse_input(const std::string &file_name) {
  std::vector<path_t> ret;

  std::ifstream fs(file_name);
  std::string line;
  while (std::getline(fs, line)) {
    path_t wire_path;
    std::string move;

    std::istringstream reader(line);
    while (std::getline(reader, move, ',')) {
      move_t next;

      next.amount = std::stoi(move.substr(1));
      switch (move[0]) {
      case 'R':
        next.to = R;
        break;
      case 'L':
        next.to = L;
        break;
      case 'U':
        next.to = U;
        break;
      case 'D':
        next.to = D;
        break;
      }

      wire_path.push_back(next);
    }

    ret.push_back(wire_path);
  }
  return ret;
}

std::unordered_set<coord_t> follow_path(const path_t &wire_path) {
  std::unordered_set<coord_t> ret;

  coord_t current{0, 0};

  int steps;
  for (auto move : wire_path) {
    steps = move.amount;
    for (; steps; steps--) {
      current = current + shifts[move.to];
      current.steps += 1;
      ret.insert(current);
    }
  }

  return ret;
}

std::vector<coord_t>
get_intersection(const std::unordered_set<coord_t> &wire1,
                 const std::unordered_set<coord_t> &wire2) {
  std::vector<coord_t> cross_points;

  for (auto c : wire2) {
    auto p1 = wire1.find(c);
    if (p1 != wire1.end()) {
      coord_t aggregate = c;
      aggregate.steps = c.steps + p1->steps;
      cross_points.push_back(aggregate);
    }
  }

  return cross_points;
}

void print_point(const coord_t &p) {
  std::cout << '(' << p.i << ',' << p.j << ')' << std::endl;
  ;
}

int part1(const std::vector<coord_t> &intersections) {

  int min_dist = INT_MAX;
  coord_t origin{.i = 0, .j = 0};
  for (auto point : intersections) {
    min_dist = std::min(min_dist, manhattan_dist(point, origin));
  }

  return min_dist;
}

int part2(const std::vector<coord_t> &intersections) {
  int min_sum = INT_MAX;
  coord_t origin{.i = 0, .j = 0};
  for (auto point : intersections) {
    min_sum = std::min(min_sum, point.steps);
  }

  return min_sum;
}

void print_path(std::unordered_set<coord_t> path) {
  for (auto c : path) {
    std::cout << "(" << c.i << "," << c.j << ")" << std::endl;
  }
}

int main() {

  auto moves = parse_input("input.txt");
  // auto moves = parse_input("test.txt");

  auto wire1 = follow_path(moves[0]), wire2 = follow_path(moves[1]);

  auto intersection = get_intersection(wire1, wire2);

  auto r1 = part1(intersection);
  std::cout << "Part 1: " << r1 << std::endl;

  auto r2 = part2(intersection);
  std::cout << "Part 2: " << r2 << std::endl;
  return 0;
}

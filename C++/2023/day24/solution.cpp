#include "utils.h"
#include <optional>

using namespace std;
using namespace aoc_utils;

struct Hailstone {

  vector<int64_t> pos;
  vector<int> velocity;
};

vector<Hailstone> read_hails(const vector<string> &lines) {
  vector<Hailstone> result;

  for (auto &l : lines) {
    Hailstone next;
    auto sides = split(l, '@');
    auto position = split(sides[0], ',');
    int i = 0;
    for (auto &p : position) {
      next.pos.push_back(stoll(p));
      i++;
    }

    auto velocity = split(sides[1], ',');
    i = 0;
    for (auto &v : velocity) {
      next.velocity.push_back(stoi(v));
      i++;
    }
    result.push_back(next);
  }

  return result;
}

optional<pair<double, double>>
intersect2d(const Hailstone &h1, const Hailstone &h2, int axis_1, int axis_2) {

  auto dx = h2.pos[axis_1] - h1.pos[axis_1],
       dy = h2.pos[axis_2] - h1.pos[axis_2];

  auto det = h2.velocity[axis_1] * h1.velocity[axis_2] -
             h2.velocity[axis_2] * h1.velocity[axis_1];

  if (det != 0) {
    double t1 = (dy * h2.velocity[axis_1] - dx * h2.velocity[axis_2]) /
                (double)det,
           t2 = (dy * h1.velocity[axis_1] - dx * h1.velocity[axis_2]) /
                (double)det;

    if (t1 >= 0 && t2 >= 0) {
      double x = h1.pos[axis_1] + t1 * h1.velocity[axis_1],
             y = h1.pos[axis_2] + t1 * h1.velocity[axis_2];
      return {{x, y}};
    }
  }

  return {};
}

int64_t part1(const vector<Hailstone> &hails, int64_t lower, int64_t upper) {
  int64_t count = 0;

  auto n = hails.size();
  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      auto point = intersect2d(hails[i], hails[j], 0, 1);
      if (point) {

        auto [x, y] = *point;

        if (x >= lower && x <= upper && y >= lower && y <= upper) {
          count++;
        }
      }
    }
  }

  return count;
}

int64_t part2_bf(const vector<Hailstone> &hails) {

  auto n = hails.size();
  auto bf = 1000; // Random good enough velocity range to brute force

  int good_enough = 10; // 3d lines very rarely intersect so if more than lets
                        // say 10 do we probably got it.

  bool found_xy = false;

  vector<double> result(3, DBL_MIN);

  int vx, vy;
  for (vx = -bf; vx <= bf && !found_xy; vx++) {
    println("Trying vx ", vx);
    for (vy = -bf; vy <= bf && !found_xy; vy++) {

      auto h1 = hails[0];
      h1.velocity[0] += vx;
      h1.velocity[1] += vy;

      result.clear();

      int i;
      for (i = 1; i <= good_enough; i++) {

        auto h2 = hails[i];
        h2.velocity[0] += vx;
        h2.velocity[1] += vy;

        auto point = intersect2d(h1, h2, 0, 1);

        if (!point) {
          break;
        }

        auto [x, y] = *point;

        if (i > 1 && (result[0] != x || result[1] != y))
          break;

        if (i == 1) {
          result[0] = x;
          result[1] = y;
        }
      }
      if (i == good_enough) {
        found_xy = true;
      }
    }
  }

  println("Found x y: ", result[0], " ", result[1]);
  vx--;
  int vz;
  for (vz = -bf; vz <= bf; vz++) {
    auto h1 = hails[0];
    h1.velocity[0] += vx;
    h1.velocity[2] += vz;

    int i;
    for (i = 1; i <= good_enough; i++) {

      auto h2 = hails[i];
      h2.velocity[0] += vx;
      h2.velocity[2] += vz;

      auto point = intersect2d(h1, h2, 0, 2);

      if (!point) {
        break;
      }

      auto [x, z] = *point;

      if (i > 1 && (result[0] != x || result[2] != z))
        break;

      if (i == 1) {
        result[1] = z;
      }
    }

    if (i == good_enough)
      break;
  }
  return result[0] + result[1] + result[2];
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto lower = stoll(argv[2]), upper = stoll(argv[3]);

  auto hails = read_hails(lines);

  auto r1 = part1(hails, lower, upper);
  println("Part 1: ", r1);

  auto r2 = part2_bf(hails);
  println("Part 2: ", r2);

  return 0;
}
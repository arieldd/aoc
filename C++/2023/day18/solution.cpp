#include "utils.h"

using namespace std;

struct Point {
  int64_t x;
  int64_t y;
  int dir;

  bool operator==(const Point &p) const { return x == p.x && y == p.y; }

  bool operator<(const Point &p) const {
    return x < p.x || (!(p.x < x) && y < p.y);
  }
};

struct Interval {
  Point first;
  Point last;
  int64_t length;
  int dir;

  bool is_vertical() const { return dir == 2 || dir == 6; };

  bool contains(const Point &p) const {
    if (first.x != p.x && first.y != p.y)
      return false;

    auto x0 = min(first.x, last.x), x1 = max(first.x, last.x),
         y0 = min(first.y, last.y), y1 = max(first.y, last.y);

    if (first.x == p.x)
      return p.y >= y0 && p.y <= y1;

    return p.x >= x0 && p.x <= x1;
  }

  bool intersects(const Point &p) const {

    if (p.x > first.x)
      return false;

    auto y0 = min(first.y, last.y), y1 = max(first.y, last.y);
    if (dir == 0 || dir == 4)
      return p.y == first.y;

    return p.y > y0 && p.y < y1;
  }
};

struct Dig {
  int dir;
  int length;
  string hex_color;
};

map<string, int> moves = {{"R", 0}, {"D", 2}, {"L", 4}, {"U", 6}};
map<char, int> hex_values{{'a', 10}, {'b', 11}, {'c', 12},
                          {'d', 13}, {'e', 14}, {'f', 15}};

int64_t hex_to_decimal(const string &hex) {
  int64_t result = 0;
  string code = hex;
  reverse(code.begin(), code.end());

  int i = 0;
  for (auto &c : code) {
    int value = (c - '0');
    if (!isdigit(c)) {
      value = hex_values.at(c);
    }
    result += value * ipow(16, i);
    i++;
  }

  return result;
}

vector<Dig> read_plan(const vector<string> &lines) {
  vector<Dig> result;

  for (auto &l : lines) {
    auto parts = split(l, ' ');
    Dig next{moves.at(parts[0]), stoi(parts[1]), parts[2]};

    result.push_back(next);
  }

  return result;
}

vector<Dig> get_new_plan(const vector<Dig> &plan) {
  vector<Dig> new_plan;

  for (auto &d : plan) {
    Dig next;
    next.length = hex_to_decimal(d.hex_color.substr(2, 5));
    next.dir = (d.hex_color[7] - '0') * 2;

    new_plan.push_back(next);
  }

  return new_plan;
}

void print_plan(const vector<Dig> &plan) {
  for (auto &d : plan) {
    println(d.dir, ",", d.length, ",", d.hex_color);
  }
}

void print_dig(const vector<Interval> &moves) {
  for (auto &m : moves) {
    println("(", m.first.y, ",", m.first.x, ")", "-(", m.last.y, ",", m.last.x,
            ") ", m.length, " ", m.dir);
  }
}

map<int, set<Point>> follow_plan(const vector<Dig> &plan) {
  map<int, set<Point>> result;

  int dir;
  Point current{0, 0};
  auto n = plan.size();
  for (auto i = 0; i < n; i++) {
    auto d = plan[i];
    dir = d.dir;
    for (int j = 0; j < d.length; j++) {
      Point next = {current.x + dx[dir], current.y + dy[dir], dir};
      if (j == d.length - 1) {
        if (dir == 2)
          next.dir = 7;
        else if (dir == 6)
          next.dir = 8;
        else {
          auto next_dir = (i < n - 1) ? plan[i + 1].dir : plan[0].dir;
          if (next_dir == 2)
            next.dir = 8;
          else
            next.dir = 7;
        }
      }
      result[next.y].insert(next);
      current = next;
    }
  }

  return result;
}

vector<Interval> follow_big_plan(const vector<Dig> &plan) {
  vector<Interval> result;
  int dir;

  Point first, last = {0, 0};
  auto n = plan.size();

  for (auto i = 0; i < n; i++) {
    auto d = plan[i];
    dir = d.dir;

    first = last;
    last = {first.x + dx[dir] * d.length, first.y - dy[dir] * d.length};

    result.push_back({first, last, d.length, dir});
  }
  return result;
}

int64_t part1__pipes(const vector<Dig> &plan) {

  auto points = follow_plan(plan);

  int64_t total = 0;

  int max_y = points.size();

  for (auto &pair : points) {
    auto max = pair.second.rbegin();
    auto min = pair.second.begin();

    auto y = pair.first, dir = 0;
    int up = 0, down = 0;

    for (int x = min->x; x <= max->x; x++) {
      if (auto it = pair.second.find({x, y}); it != pair.second.end()) {
        total++;
        dir = it->dir;

        if (dir == 2 || dir == 6) {
          up = (up) ? 0 : 1;
          down = (down) ? 0 : 1;
        } else if (dir == 7) {
          up = (up) ? 0 : 1;
        } else if (dir == 8) {
          down = (down) ? 0 : 1;
        }

      } else if (up || down) {
        total++;
      }
    }
  }

  return total;
}

int64_t part1(const vector<Dig> &plan) {

  auto sides = follow_big_plan(plan);

  int64_t area = 0, perimeter = 0;

  auto numPoints = sides.size();
  auto j = numPoints - 1;

  for (auto i = 0; i < numPoints; i++) {
    auto rect = sides[j].first.x * sides[i].first.y -
                sides[i].first.x * sides[j].first.y;

    perimeter += (sides[i].length + 1);
    area += rect;
    j = i;
  }

  perimeter -= numPoints;

  return abs(area / 2) + perimeter / 2 + 1;
}

int64_t part2(const vector<Dig> &plan) {
  auto new_plan = get_new_plan(plan);

  return part1(new_plan);
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto plan = read_plan(lines);

  auto r1 = part1(plan);
  println("Part 1: ", r1);

  auto r2 = part2(plan);
  println("Part 2: ", r2);

  return 0;
}
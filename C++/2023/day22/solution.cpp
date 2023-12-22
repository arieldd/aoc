#include "utils.h"
#include <unordered_map>

using namespace std;

struct Point {
  int x;
  int y;
  int z;

  bool operator==(const Point &p) const {
    return x == p.x && y == p.y && z == p.z;
  }
};

struct Brick {
  int id;
  vector<Point> ends;

  int orientation() const {
    if (ends[0].x != ends[1].x)
      return 0;
    if (ends[0].y != ends[1].y)
      return 1;

    return 2;
  }

  vector<Point> span() const {
    vector<Point> result;
    auto dir = orientation();
    if (dir == 0) {
      auto x_min = min_x(), x_max = max_x();
      for (auto x = x_min; x <= x_max; x++) {
        result.push_back({x, ends[0].y, ends[0].z});
      }
    } else if (dir == 1) {
      auto y_min = min_y(), y_max = max_y();
      for (auto y = y_min; y <= y_max; y++) {
        result.push_back({ends[0].x, y, ends[0].z});
      }
    } else {
      auto z_min = min_z(), z_max = max_z();
      for (auto z = z_min; z <= z_max; z++) {
        result.push_back({ends[0].x, ends[0].y, z});
      }
    }

    return result;
  }

  bool supports(const Brick &b) const {
    if (max_z() != b.min_z() - 1)
      return false;

    auto points = span(), b_points = b.span();
    for (auto &p1 : points) {
      for (auto &p2 : b_points) {
        if (p1.x == p2.x && p1.y == p2.y)
          return true;
      }
    }
    return false;
  }

  int min_z() const { return min(ends[0].z, ends[1].z); }

  int max_z() const { return max(ends[0].z, ends[1].z); }

  int min_x() const { return min(ends[0].x, ends[1].x); }

  int max_x() const { return max(ends[0].x, ends[1].x); }

  int min_y() const { return min(ends[0].y, ends[1].y); }

  int max_y() const { return max(ends[0].y, ends[1].y); }

  bool operator<(const Brick &b) const {

    auto mx = min_x(), mbx = b.min_x(), //
        my = min_y(), mby = b.min_y(),  //
        mz = min_z(), mbz = b.min_z();

    return mz < mbz ||
           (!(mbz < mz) && ((mx < mbx) || (!(mbx < mx) && my < mby)));
  }

  bool operator==(const Brick &b) const {
    return (ends[0] == b.ends[0] && ends[1] == b.ends[1]) ||
           (ends[0] == b.ends[1] && ends[1] == b.ends[0]);
  }

  bool operator==(int b_id) const { return id == b_id; }
};

template <> struct hash<Brick> {
  size_t operator()(const Brick &b) const { return std::hash<int>()(b.id); }
};

struct Report {
  unordered_map<int, vector<int>> on_top;
  unordered_map<int, vector<int>> below;

  unordered_map<int, vector<Brick>> elevation;
};

unordered_map<int, Brick> read_snapshot(const vector<string> &lines) {
  unordered_map<int, Brick> result;

  int id = 0;
  for (auto &l : lines) {
    auto ends = split(l, '~');
    Brick current;
    current.id = id;
    current.ends = vector<Point>(2);

    for (auto i : {0, 1}) {
      auto coords = split(ends[i], ',');
      current.ends[i] = {stoi(coords[0]), stoi(coords[1]), stoi(coords[2])};
    }
    result[id] = (current);
    id++;
  }
  return result;
}

Report move_to_ground(const unordered_map<int, Brick> snapshot) {
  Report result;

  vector<Brick> sorted;
  for (auto &pair : snapshot)
    sorted.push_back(pair.second);

  sort(sorted.begin(), sorted.end());

  auto n = sorted.size();

  for (int i = 0; i < n; i++) {
    bool can_drop = true;
    auto b = sorted[i];
    int z = b.min_z();

    result.on_top[b.id] = {};
    result.below[b.id] = {};

    while (z > 1) {
      if (result.elevation.find(z - 1) != result.elevation.end()) {
        for (auto &settled : result.elevation.at(z - 1)) {
          if (settled.supports(b)) {
            result.below[b.id].push_back(settled.id);
            result.on_top[settled.id].push_back(b.id);
            can_drop = false;
          }
        }
      }

      if (!can_drop)
        break;

      b.ends[0].z--;
      b.ends[1].z--;
      z = b.min_z();
    }

    result.elevation[b.max_z()].push_back(b);
  }

  return result;
}

set<int> part1(const Report &report) {

  set<int> can_remove;
  for (auto &pair : report.elevation) {
    for (auto &b : pair.second) {
      auto on_top = report.on_top.at(b.id);
      int bricks_on_top = on_top.size();

      if (bricks_on_top == 0 ||
          all_of(on_top.begin(), on_top.end(), [&](int supported) {
            return report.below.at(supported).size() > 1;
          }))
        can_remove.insert(b.id);
    }
  }

  return can_remove;
}

int count_falling(int id, const Report &report) {

  int total = 0;

  Report ledger = report;

  set<int> counted{id};
  deque<int> queue{id};
  while (!queue.empty()) {
    auto current = queue.front();
    queue.pop_front();
    auto on_top = ledger.on_top.at(current);

    for (auto i = 0; i < on_top.size(); i++) {
      int next_id = on_top[i];
      auto below = ledger.below.at(next_id);

      bool has_support = false;
      for (auto id : below) {
        if (counted.find(id) == counted.end()) {
          has_support = true;
          break;
        }
      }

      if (counted.find(next_id) == counted.end() && !has_support) {
        total++;
        queue.push_back(next_id);
        counted.insert(next_id);
      }
    }
  }
  return total;
}

int part2(const Report &report) {
  int total = 0;
  for (auto &pair : report.elevation) {
    for (auto &b : pair.second) {
      auto on_top = report.on_top.at(b.id);
      int bricks_on_top = on_top.size();

      if (bricks_on_top != 0) {
        int falling = count_falling(b.id, report);
        total += falling;
      }
    }
  }

  return total;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto snapshot = read_snapshot(lines);
  auto report = move_to_ground(snapshot);

  auto r1 = part1(report);
  println("Part 1: ", r1.size());

  auto r2 = part2(report);
  println("Part 2: ", r2);

  return 0;
}
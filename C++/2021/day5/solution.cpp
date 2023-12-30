#include "utils.h"

using namespace std;
using namespace aoc_utils;

struct Line {
  pair<int, int> p1;
  pair<int, int> p2;

  bool is_straight() const {
    return p1.first == p2.first || p1.second == p2.second;
  }

  int length() const {
    return (is_straight())
               ? abs(p1.first - p2.first) + abs(p1.second - p2.second)
               : abs(p1.second - p2.second);
  }

  vector<pair<int, int>> get_points() const {
    vector<pair<int, int>> result;

    auto top_left = min(p1, p2), bottom_right = max(p1, p2);
    result.push_back(top_left);
    if (is_straight()) {
      if (top_left.first == bottom_right.first) {
        for (int i = 1; i < bottom_right.second - top_left.second; i++)
          result.push_back({top_left.first, top_left.second + i});
      } else {
        for (int i = 1; i < bottom_right.first - top_left.first; i++)
          result.push_back({top_left.first + i, top_left.second});
      }
    } else {
      auto sign = sgn(bottom_right.second - top_left.second);
      for (int i = 1; i < bottom_right.first - top_left.first; i++) {

        result.push_back({top_left.first + i, top_left.second + i * sign});
      }
    }

    result.push_back(bottom_right);

    return result;
  }
};

vector<Line> read_lines(const vector<string> &input) {
  vector<Line> res;

  for (auto &entry : input) {
    auto arrow = entry.find("->");
    auto c1 = entry.substr(0, arrow - 1), c2 = entry.substr(arrow + 3);
    auto coords1 = split(c1, ','), coords2 = split(c2, ',');
    res.push_back(Line{{stoi(coords1[0]), stoi(coords1[1])},
                       {stoi(coords2[0]), stoi(coords2[1])}});
  }

  return res;
}

int solve(const vector<Line> &lines, bool part1) {

  vector<Line> vents;
  copy_if(lines.begin(), lines.end(), back_inserter(vents), [&](auto &line) {
    if (part1)
      return line.is_straight();
    return true;
  });

  map<pair<int, int>, int> repeats;
  int overlap = 0;
  for (auto &l : vents) {
    auto points = l.get_points();
    for (auto &p : points) {
      repeats[p]++;
      if (repeats.at(p) == 2)
        overlap++;
    }
  }
  return overlap;
}

int main(int argc, char *argv[]) {
  auto input = parse_input(argv[1]);
  auto lines = read_lines(input);

  auto r1 = solve(lines, true);
  println("Part 1: ", r1);

  auto r2 = solve(lines, false);
  println("Part 2: ", r2);

  return 0;
}
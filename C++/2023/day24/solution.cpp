#include "utils.h"
#include <Eigen/Dense>
#include <ranges>

using namespace std;
using namespace Eigen;

struct Hailstone {

  Vector<int64_t, 3> pos;
  Vector3i velocity;
};

vector<Hailstone> read_hails(const vector<string> &lines) {
  vector<Hailstone> result;

  for (auto &l : lines) {
    Hailstone next;
    auto sides = split(l, '@');
    auto position = split(sides[0], ',');
    int i = 0;
    for (auto &p : position) {
      next.pos(i) = stoll(p);
      i++;
    }

    auto velocity = split(sides[1], ',');
    i = 0;
    for (auto &v : velocity) {
      next.velocity(i) = stoi(v);
      i++;
    }
    result.push_back(next);
  }

  return result;
}

bool find_future_intersection(const Hailstone &h1, const Hailstone &h2,
                              double &outX, double &outY) {

  auto p1 = h1.pos, p2 = h2.pos;
  auto v1 = h1.velocity, v2 = h2.velocity;

  Matrix2<double> A;
  A << v1.head(2).cast<double>(), v2.head(2).cast<double>();
  A.transpose();
  Vector<double, 2> b(p2[0] - p1[0], p2[1] - p1[1]);

  auto solution = A.colPivHouseholderQr().solve(b);

  auto t = solution(0), s = solution(1);

  outX = p1(0) + v1(0) * t;
  outY = p1(1) + v1(1) * t;

  auto x1 = p1(0) + v1(0), x2 = p2(0) + v2(0);

  if (t == 0 || s == 0 // Parallal
      || abs(outX - x1) > abs(outX - p1(0)) ||
      abs(outX - x2) > abs(outX - p2(0))) // Past interssection
    return false;

  return true;
}

tuple<int64_t, int64_t, int64_t>
find_least_squares(const vector<Hailstone> &hails) {
  auto n = hails.size();

  Vector<double, Dynamic> b;

  vector<Vector3d> columns;
  for (int i = 1; i < n; i++) {
    columns.push_back(
        (hails[i].velocity - hails[i - 1].velocity).cast<double>());
    b << (hails[i].pos - hails[i - 1].pos).cast<double>();
  }

  // MatrixX<double> A(columns.data());

  // A << (1, 1, 1);
  // A.transpose();

  // println(A, b);

  // auto solution = A.colPivHouseholderQr().solve(b);
  // print(solution);

  return {};
}

int part1(const vector<Hailstone> &hails, int64_t lower, int64_t upper) {
  int count = 0;

  double outX, outY;
  auto n = hails.size();
  for (int i = 0; i < n - 1; i++) {
    for (int j = i + 1; j < n; j++) {
      if (find_future_intersection(hails[i], hails[j], outX, outY) &&
          lower <= outX && outX <= upper && lower <= outY && outY <= upper) {
        count++;
      }
    }
  }

  return count;
}

void move_to_time(vector<Hailstone> &hails, int time) {
  for (int i = 0; i < time; i++)
    for (auto &h : hails) {
      h.pos += h.velocity.cast<int64_t>();
    }
}

int64_t part2_bf(const vector<Hailstone> &hails) {

  auto n = hails.size();

  auto comp = [](const Hailstone &h1, const Hailstone &h2) {
    return h1.pos(2) > h2.pos(2); // order descending by Z coord
  };
  // for (auto &h : trajectories) {
  //   println(RowVector3<int64_t>(h.pos));
  // }
  for (int i = 1; i <= n; i++) {
    auto trajectories = hails;
    move_to_time(trajectories, i);
    sort(trajectories.begin(), trajectories.end(), comp);
    auto p1 = trajectories.back().pos;
    trajectories.pop_back();

    // println("Cutting ", RowVector3<int64_t>(p1), " at time ", i);
    for (int j = i + 1; j < 2 * n; j++) {
      move_to_time(trajectories, 1);
      sort(trajectories.begin(), trajectories.end(), comp);
      auto p2 = trajectories.back().pos;

      auto line = Hailstone{p1, (p2 - p1).cast<int>()};

      // println("Cutting ", RowVector3<int64_t>(p2), " at time ", j);

      // println("Tentative line is ", RowVector3<int64_t>(line.pos), "->",
      //         RowVector3<int>(line.velocity));

      bool intersects_all = true;
      for (int time = j + 1; time < 2 * n; time++) {
        println("cheking time ", time);
        vector<Hailstone> rest(trajectories.begin() + 1, trajectories.end());
        sort(rest.begin(), rest.end(), comp);
        move_to_time(rest, 1);
        auto next = rest.back();

        line.pos += line.velocity.cast<int64_t>();
        if (line.pos != next.pos && line.pos[2] > next.pos[2]) {
          intersects_all = false;
          break;
        }
      }
      if (intersects_all) {
        line = Hailstone{p1, (p2 - p1).cast<int>()};
        println(RowVector3<int64_t>(line.pos), RowVector3<int>(line.velocity));
        line.pos -= line.velocity.cast<int64_t>();
        println(line.pos);
        return line.pos.sum();
      }
    }
  }

  return 0;
}

int part2_ls(const vector<Hailstone> &hails) {
  auto solution = find_least_squares(hails);

  return 0;
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
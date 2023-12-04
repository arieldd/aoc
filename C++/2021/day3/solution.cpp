#include "utils.h"

using namespace std;

using report_t = vector<vector<int>>;

report_t read_report(const vector<string> &lines) {
  report_t result;

  for (auto &line : lines) {
    vector<int> row;
    for (auto &c : line) {
      row.push_back(c - '0');
    }
    result.push_back(row);
  }

  return result;
}

int to_decimal(vector<int> byte) {
  int result = 0, n = byte.size();
  for (auto i = 0; i < n; i++) {
    result += byte[i] * ipow(2, n - 1 - i);
  }

  return result;
}

int scan_numbers(const report_t &current, int column, int preferred) {
  if (current.size() == 1)
    return to_decimal(current.front());

  if (column >= current[0].size())
    return 0;

  int r = current.size();

  vector<int> ones, zeros;

  for (auto i = 0; i < r; i++) {
    if (current[i][column])
      ones.push_back(i);
    else
      zeros.push_back(i);
  }

  vector<int> selected;
  auto s1 = ones.size(), s0 = zeros.size();

  if (preferred) {
    selected = (s1 == s0 || s1 > s0) ? ones : zeros;
  } else {
    selected = (s1 == s0 || s1 > s0) ? zeros : ones;
  }

  report_t next;
  for (auto &index : selected) {
    next.push_back(current[index]);
  }

  return scan_numbers(next, column + 1, preferred);
}

vector<string> parse_input(const string &file_name) {
  vector<string> ret;

  ifstream fs(file_name);
  string line;
  while (getline(fs, line)) {
    ret.push_back(line);
  }
  return ret;
}

int part1(const report_t &report) {
  int r = report.size(),    //
      c = report[0].size(), //
      h = r / 2;

  vector<int> gamma, epsilon;
  for (auto j = 0; j < c; j++) {
    int sum = 0;
    for (auto i = 0; i < r; i++) {
      sum += report[i][j];
    }
    gamma.push_back(sum > h);
    epsilon.push_back(sum < h);
  }

  return to_decimal(gamma) * to_decimal(epsilon);
}

int part2(const report_t &report) {
  int o2 = scan_numbers(report, 0, 1), //
      co2 = scan_numbers(report, 0, 0);
  return o2 * co2;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto report = read_report(lines);

  auto r1 = part1(report);
  println("Part 1: ", r1);

  auto r2 = part2(report);
  println("Part 2: ", r2);

  return 0;
}
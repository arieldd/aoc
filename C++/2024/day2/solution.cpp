#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
using namespace std;

vector<vector<int>> read_input(const string &filename) {
  vector<vector<int>> reports{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line)) {
    vector<int> report;
    string number;
    stringstream ss(line);
    while (getline(ss, number, ' ')) {
      report.push_back(stoi(number));
    }
    reports.push_back(report);
  }
  return reports;
}

bool is_safe(const vector<int> &report) {
  auto n = report.size();
  int trend = 0;
  for (int i = 1; i < n; i++) {
    int distance = report[i] - report[i - 1], sign = abs(distance) / distance;
    if (abs(distance) < 1 or abs(distance) > 3 ||
        (trend != 0 and trend != sign)) {
      return false;
    }

    trend = sign;
  }
  return true;
}

int solve(const vector<vector<int>> &reports, bool tolerance) {
  int result = 0;
  for (auto &report : reports) {
    auto safe = is_safe(report);
    if (tolerance && !safe) {
      for (int i = 0; i < report.size(); i++) {
        vector<int> temp = report;
        temp.erase(temp.begin() + i);
        if (is_safe(temp)) {
          safe = true;
          break;
        }
      }
    }
    if (safe) {
      result++;
    }
  }
  return result;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto reports = read_input(argv[1]);
  cout << "Part 1: " << solve(reports, 0) << '\n';
  cout << "Part 2: " << solve(reports, 1) << '\n';
  return 0;
}

#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;
#define umap unordered_map

struct Locations {
  vector<int> l1;
  vector<int> l2;
};

Locations read_locations(const vector<string> &lines) {
  Locations result;
  int v1, v2;
  for (auto const &line : lines) {
    stringstream ss(line);
    ss >> v1 >> v2;
    result.l1.push_back(v1);
    result.l2.push_back(v2);
  }
  sort(result.l1.begin(), result.l1.end());
  sort(result.l2.begin(), result.l2.end());

  return result;
}

vector<string> read_input(const string &filename) {
  vector<string> lines{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    lines.push_back(line);
  return lines;
}

int part1(const Locations &lists) {

  auto n = lists.l1.size();
  auto sum = 0;
  for (int i = 0; i < n; i++) {
    sum += abs(lists.l1[i] - lists.l2[i]);
  }
  return sum;
}

int part2(const Locations &lists) {
  umap<int, int> freq;
  for (auto value : lists.l1) {
    freq.insert({value, 0});
  }
  for (auto value : lists.l2) {
    if (freq.count(value)) {
      freq.at(value) += 1;
    }
  }

  auto score = 0;
  for (auto &entry : freq) {
    score += entry.first * entry.second;
  }
  return score;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto lines = read_input(argv[1]);
  auto lists = read_locations(lines);
  cout << "Part 1: " << part1(lists) << '\n';
  cout << "Part 2: " << part2(lists) << '\n';
  return 0;
}

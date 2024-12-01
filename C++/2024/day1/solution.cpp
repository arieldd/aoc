#include <algorithm>
#include <cassert>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

#define ll long long
#define umap unordered_map

struct Locations {
  vector<int> l1;
  vector<int> l2;
};

Locations read_locations(const vector<string> &lines) {
  Locations result;
  for (auto const &line : lines) {
    auto sep = line.find("   ");
    result.l1.push_back(stoi(line.substr(0, sep)));
    result.l2.push_back(stoi(line.substr(sep + 3, line.size())));
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

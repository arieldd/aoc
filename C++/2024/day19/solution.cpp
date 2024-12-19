#include "utils.h"
#include <cassert>
using namespace std;
using namespace aoc_utils;

#define ll long long
#define arr array

vector<string> read_input(const string &filename) {
  vector<string> lines{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    lines.push_back(line);
  return lines;
}

ll possible(string pattern, const set<string> &towels, map<string, ll> &cache) {
  auto n = pattern.size();

  if (n == 0)
    return 1;

  if (cache.contains(pattern)) {
    return cache.at(pattern);
  }

  if (n == 1) // Would have been in cache otherwise
    return 0;

  ll count = towels.contains(pattern);
  for (auto index = 1; index < n; index++) {
    string head = pattern.substr(0, index);
    string tail = pattern.substr(index, n);
    if (towels.contains(head)) {
      count += possible(tail, towels, cache);
    }
  }

  cache[pattern] = count;
  return count;
}

pair<ll, ll> solve(const vector<string> &patterns, const set<string> &towels) {
  ll part1 = 0, part2 = 0;

  map<string, ll> cache;
  for (auto &t : towels) {
    if (t.size() == 1)
      cache[t] = 1;
  }

  for (auto &p : patterns) {
    ll counter = possible(p, towels, cache);
    if (counter)
      part1++;
    part2 += counter;
  }
  return {part1, part2};
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto lines = read_input(argv[1]);
  set<string> towels;
  vector<string> patterns;
  for (auto &towel : split(lines[0], ',')) {
    towels.insert(towel);
  }
  for (int i = 2; i < lines.size(); i++) {
    patterns.push_back(lines[i]);
  }

  auto solutions = solve(patterns, towels);
  cout << "Part 1:" << solutions.first << '\n';
  cout << "Part 2:" << solutions.second << '\n';
  return 0;
}

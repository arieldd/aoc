#include "utils.h"
#include <bitset>
#include <cassert>
#include <cstdio>
#include <unordered_map>
using namespace std;
using namespace aoc_utils;

#define ll long long
#define arr array

vector<ll> read_input(const string &filename) {
  vector<ll> numbers{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    numbers.push_back(stoll(line));
  return numbers;
}

ll mix(ll v1, ll v2) { return v1 ^ v2; }

ll prune(ll v) { return v % 16777216; }

pair<ll, ll> solve(const vector<ll> &numbers) {
  ll part1 = 0, part2 = 0;
  unordered_map<int, int> sequences;
  for (auto val : numbers) {
    unordered_map<int, int> first_seen;
    vector<int> changes{0};
    vector<int> prices{(int)(val % 10)};
    for (int cycle = 1; cycle <= 2000; cycle++) {
      val = prune(mix(val, val * 64));
      val = prune(mix(val, val / 32));
      val = prune(mix(val, val * 2048));

      int price = val % 10;
      changes.push_back(price - prices.back());
      prices.push_back(price);
      if (cycle >= 4) {
        int seq = 0;
        for (auto k = 0; k < 4; k++) {
          seq <<= 8;
          seq |= changes[cycle - 3 + k] & 0x000000ff;
        }
        if (!first_seen.contains(seq)) {
          first_seen.insert({seq, prices[cycle]});
        }
      }
    }

    for (auto entry : first_seen) {
      sequences[entry.first] += entry.second;
    }
    part1 += val;
  }

  for (auto entry : sequences)
    if (part2 < entry.second)
      part2 = entry.second;

  return {part1, part2};
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto numbers = read_input(argv[1]);
  auto answers = solve(numbers);
  cout << "Part 1:" << answers.first << '\n';
  cout << "Part 2:" << answers.second << '\n';
  return 0;
}

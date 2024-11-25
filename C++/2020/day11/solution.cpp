#include <bits/stdc++.h>
#include <cassert>
using namespace std;

#define ll long long
#define arr array
#define umap unordered_map

vector<ll> read_input(const string &filename) {
  vector<ll> lines{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    lines.push_back(atoll(line.c_str()));
  return lines;
}

ll part1(const vector<ll> &numbers) {
  vector<int> adapters(numbers.begin(), numbers.end());
  sort(adapters.begin(), adapters.end());

  ll ones = (adapters[0] == 1) ? 1 : 0,
     // there is always a 3 jolt diff with my device
      threes = (adapters[0] == 3) ? 2 : 1;

  auto n = adapters.size();
  for (auto i = 1; i < n; i++) {
    auto joltage = adapters[i - 1];
    if (adapters[i] - joltage == 1)
      ones++;
    if (adapters[i] - joltage == 3)
      threes++;
  }

  return ones * threes;
}

vector<ll> usable_adapters(const vector<ll> &adapters, ll joltage) {
  vector<ll> result{};
  for (auto const &value : adapters) {
    if (value <= joltage)
      continue;
    if (value - joltage <= 3)
      result.push_back(value);
    else
      break;
  }
  return result;
}

ll count_arrangements(const vector<ll> &adapters, ll joltage, ll max_joltage,
                      umap<ll, ll> &dp) {
  if (dp.find(joltage) != dp.end())
    return dp.at(joltage);

  ll result = 0;
  auto usable = usable_adapters(adapters, joltage);
  if (usable.size() == 0) {
    result = (max_joltage - joltage <= 3) ? 1 : 0;
  } else {
    for (auto value : usable) {
      result += count_arrangements(adapters, value, max_joltage, dp);
    }
  }

  dp.insert({joltage, result});
  return result;
}

ll part2(const vector<ll> &numbers) {
  vector<ll> adapters(numbers.begin(), numbers.end());
  sort(adapters.begin(), adapters.end());

  auto dp = umap<ll, ll>{};
  return count_arrangements(adapters, 0, adapters.back() + 3, dp);
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto numbers = read_input(argv[1]);

  cout << "Part 1: " << part1(numbers) << '\n';
  cout << "Part 2: " << part2(numbers) << '\n';
  return 0;
}

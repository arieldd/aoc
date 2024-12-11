#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
using namespace std;

#define ll long long
#define umap unordered_map

vector<ll> read_input(const string &filename) {
  vector<ll> stones{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line)) {
    string value;
    stringstream ss(line);
    while (getline(ss, value, ' '))
      stones.push_back(stoi(value));
  }
  return stones;
}

ll solve(vector<ll> stones, char blinks) {
  umap<ll, ll> current{};
  for (auto s : stones)
    current[s] = 1;

  while (blinks) {
    umap<ll, ll> next{};
    for (auto &pair : current) {
      if (pair.first == 0) {
        next[1] += pair.second;
        continue;
      }

      string value = to_string(pair.first);
      if (value.size() % 2) {
        next[stoll(value) * 2024] += pair.second;
      } else {
        ll left = stoll(value.substr(0, value.size() / 2)),
           right = stoll(value.substr(value.size() / 2, value.size()));

        next[left] += pair.second;
        next[right] += pair.second;
      }
    }
    current = next;
    blinks--;
  }

  return accumulate(
      current.begin(), current.end(), (ll)0,
      [](const auto sum, auto entry) { return sum + entry.second; });
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto stones = read_input(argv[1]);
  cout << "Part 1:" << solve(stones, 25) << '\n';
  cout << "Part 2:" << solve(stones, 75) << '\n';
  return 0;
}

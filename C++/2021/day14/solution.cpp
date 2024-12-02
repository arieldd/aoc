#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#define ll long long
#define pairc pair<char, char>

struct Instructions {
  map<char, ll> letter_count;
  map<pairc, ll> base_polymer;
  map<pairc, vector<pairc>> insertions;
};

Instructions read_input(const string &filename) {
  Instructions result;
  ifstream fs(filename);

  string base_polymer;
  getline(fs, base_polymer);
  auto n = base_polymer.size();
  for (int i = 0; i < n - 1; i++) {
    pairc key = {base_polymer[i], base_polymer[i + 1]};
    result.base_polymer.insert({key, 1});

    result.letter_count[base_polymer[i]] += 1;
  }
  result.letter_count[base_polymer[n - 1]] += 1;

  string line;
  while (getline(fs, line)) {
    if (line.empty())
      continue;
    stringstream ss(line);
    pairc key;
    ss >> key.first >> key.second;
    ss.ignore(4);
    char inserted;
    ss >> inserted;

    result.insertions.insert(
        {key, {{key.first, inserted}, {inserted, key.second}}});
  }
  return result;
}

ll solve(const Instructions &instr, int steps) {
  map<pairc, ll> polymer = instr.base_polymer;
  map<char, ll> letter_count = instr.letter_count;

  while (steps > 0) {
    map<pairc, ll> new_polymer;
    for (auto &entry : polymer) {
      if (instr.insertions.count(entry.first)) {
        auto new_pairs = instr.insertions.at(entry.first);
        auto new_letter =
            new_pairs[0].second; // Which new letter is being added

        letter_count[new_letter] += entry.second;

        for (auto &p : new_pairs) {
          new_polymer[p] += entry.second;
        }
      } else {
        new_polymer.insert(entry);
      }
    }
    polymer = new_polymer;
    steps--;
  }

  vector<ll> freq;
  for (auto entry : letter_count) {
    freq.push_back(entry.second);
  }

  return *max_element(freq.begin(), freq.end()) -
         *min_element(freq.begin(), freq.end());
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto instr = read_input(argv[1]);

  cout << "Part 1: " << solve(instr, 10) << '\n';
  cout << "Part 2: " << solve(instr, 40) << '\n';
  return 0;
}

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

struct Printer {
  map<int, vector<int>> rules;
  vector<vector<int>> updates;
};

vector<string> read_input(const string &filename) {
  vector<string> lines{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    lines.push_back(line);
  return lines;
}

Printer build_printer(const vector<string> &lines) {
  Printer result;
  bool rules = true;
  for (auto &line : lines) {
    if (line.empty()) {
      rules = false;
      continue;
    }
    if (rules) {
      auto sep = line.find('|');
      int key = stoi(line.substr(0, sep));
      if (!result.rules.count(key))
        result.rules[key] = vector<int>{};
      result.rules[key].push_back(stoi(line.substr(sep + 1, line.size())));
    } else {
      string value;
      stringstream ss(line);
      result.updates.push_back({});
      while (getline(ss, value, ',')) {
        result.updates.back().push_back(stoi(value));
      }
    }
  }

  return result;
}

pair<int, int> solve(Printer &printer) {
  auto part1 = 0;
  auto part2 = 0;

  // Custom comparison lambda based on the printer rules.
  auto cmp = [&](int page1, int page2) {
    if (!printer.rules.count(page2))
      return true;
    auto &pages_after = printer.rules.at(page2);
    return find(pages_after.begin(), pages_after.end(), page1) ==
           pages_after.end();
  };

  for (auto &update : printer.updates) {
    if (is_sorted(update.begin(), update.end(), cmp)) {
      part1 += update[update.size() / 2];
    } else {
      sort(update.begin(), update.end(), cmp);
      part2 += update[update.size() / 2];
    }
  }
  return {part1, part2};
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto lines = read_input(argv[1]);
  auto printer = build_printer(lines);
  auto answers = solve(printer);
  cout << "Part 1: " << answers.first << '\n';
  cout << "Part 2: " << answers.second << '\n';
  return 0;
}

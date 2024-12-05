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
  vector<int> invalid_updates;
};

vector<string> read_input(const string &filename) {
  vector<string> lines{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    lines.push_back(line);
  return lines;
}

Printer read_printer(const vector<string> &lines) {
  Printer result;
  bool rules = true;
  for (auto &line : lines) {
    if (line.empty()) {
      rules = false;
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
      if (result.updates.back().size() == 0)
        result.updates.pop_back();
    }
  }

  return result;
}

int part1(Printer &printer) {
  auto sum = 0;
  auto n = printer.updates.size();
  for (int i = 0; i < n; i++) {
    auto update = printer.updates[i];
    vector<int> printed_after = {update.back()};
    auto m = update.size();
    bool valid = true;
    for (int j = m - 2; j >= 0; j--) {
      auto page = update[j];
      for (auto after : printed_after) {
        if (printer.rules.contains(after)) {
          auto successors = printer.rules.at(after);
          if (find(successors.begin(), successors.end(), page) !=
              successors.end()) {
            valid = false;
            break;
          }
        }
      }
      if (!valid) {
        break;
      }
      printed_after.push_back(page);
    }
    if (valid) {
      sum += update[update.size() / 2];
    } else {
      printer.invalid_updates.push_back(i);
    }
  }
  return sum;
}

int part2(Printer &printer) {
  auto sum = 0;
  for (auto index : printer.invalid_updates) {
    auto update = printer.updates[index];
    sort(update.begin(), update.end(), [&](int p1, int p2) {
      if (!printer.rules.contains(p2))
        return true;
      auto &pages = printer.rules.at(p2);
      return find(pages.begin(), pages.end(), p1) == pages.end();
    });

    sum += update[update.size() / 2];
  }
  return sum;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto lines = read_input(argv[1]);
  auto printer = read_printer(lines);
  cout << "Part 1: " << part1(printer) << '\n';
  cout << "Part 2: " << part2(printer) << '\n';
  return 0;
}

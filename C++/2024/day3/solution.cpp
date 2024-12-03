#include <cassert>
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>
using namespace std;

vector<string> read_input(const string &filename) {
  vector<string> lines{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    lines.push_back(line);
  return lines;
}

pair<int, int> solve(const vector<string> &lines) {
  regex pattern("mul\\(([0-9]+),([0-9]+)\\)|do\\(\\)|don't\\(\\)");
  auto end = sregex_iterator();

  int part1 = 0;
  int part2 = 0;
  bool state = true;
  for (auto &line : lines) {
    auto matches = sregex_iterator(line.begin(), line.end(), pattern);
    for (auto it = matches; it != end; it++) {
      smatch match = *it;
      if (match.str() == "do()")
        state = true;
      else if (match.str() == "don't()")
        state = false;
      else {
        int v1 = stoi(match[1]), v2 = stoi(match[2]);
        part1 += v1 * v2;
        if (state) {
          part2 += v1 * v2;
        }
      }
    }
  }
  return {part1, part2};
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto lines = read_input(argv[1]);

  auto result = solve(lines);
  cout << "Part 1: " << result.first << '\n';
  cout << "Part 2: " << result.second << '\n';
  return 0;
}

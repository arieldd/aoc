#include "utils.h"

using namespace std;

vector<string> parse_input(const string &file_name) {
  vector<string> ret;

  ifstream fs(file_name);
  string line;
  while (getline(fs, line)) {
    ret.push_back(line);
  }
  return ret;
}

int part1(const vector<string> &lines) { return 0; }

int part2(const vector<string> &lines) { return 0; }

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);

  auto r1 = part1(lines);
  println("Part 1: ", r1);

  auto r2 = part2(lines);
  println("Part 2: ", r2);

  return 0;
}
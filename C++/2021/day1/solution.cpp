#include "utils.h"

using namespace std;

vector<int> parse_input(const string &file_name) {
  vector<int> ret;

  ifstream fs(file_name);
  string line;
  while (getline(fs, line)) {
    ret.push_back(stoi(line));
  }
  return ret;
}

int part1(const vector<int> &depths) {
  int last = depths[0], increased = 0;
  for (auto &d : depths) {
    if (d > last)
      increased++;
    last = d;
  }
  return increased;
}

int part2(const vector<int> &depths) {
  int last = depths[0] + depths[1] + depths[2], //
      increased = 0,                            //
      size = depths.size();

  for (int i = 1; i < size - 2; i++) {
    auto d = depths[i] + depths[i + 1] + depths[i + 2];
    if (d > last)
      increased++;
    last = d;
  }
  return increased;
}

int main(int argc, char *argv[]) {
  auto depths = parse_input(argv[1]);

  auto r1 = part1(depths);
  println("Part 1: ", r1);

  auto r2 = part2(depths);
  println("Part 2: ", r2);

  return 0;
}
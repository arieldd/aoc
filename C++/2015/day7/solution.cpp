#include "utils.h"

using namespace std;
using namespace aoc_utils;

struct Wire {
  string label;
  unsigned short signal;
  string op;
  vector<string> inputs;
};

map<string, Wire> read_booklet(const vector<string> &lines) {
  map<string, Wire> res;

  for (auto &l : lines) {
    Wire next;
    auto parts = split(l, ' ');

    next.signal = USHRT_MAX;
    next.label = parts.back();

    if (parts.size() == 3) { // direct input
      try {
        next.signal = stoi(parts[0]);
      } catch (...) {
        next.inputs.push_back(parts[0]);
      }
    } else if (parts.size() == 4) // NOT expression
    {
      next.op = parts[0];
      next.inputs.push_back(parts[1]);
    } else {
      next.op = parts[1];
      next.inputs.push_back(parts[0]);
      next.inputs.push_back(parts[2]);
    }

    res[next.label] = next;
  }

  //   for (auto &p : res) {
  //     print(p.second.label, ":", p.second.signal, " ", p.second.op);
  //     for (auto &i : p.second.inputs)
  //       print(" ", i);
  //     println();
  //   }

  return res;
}

int part1(const vector<string> &lines) { return 0; }

int part2(const vector<string> &lines) { return 0; }

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto wires = read_booklet(lines);

  auto r1 = part1(lines);
  println("Part 1: ", r1);

  auto r2 = part2(lines);
  println("Part 2: ", r2);

  return 0;
}
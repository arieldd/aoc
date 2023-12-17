#include "utils.h"

using namespace std;

struct Network {
  vector<int> directions;

  map<string, vector<string>> nodes;
};

Network read_instr(const vector<string> &lines) {
  Network result;

  for (auto &c : lines[0]) {
    auto move = (c == 'L') ? 0 : 1;
    result.directions.push_back(move);
  }

  for (int i = 2; i < lines.size(); i++) {
    auto entry = split(lines[i], '=');
    auto left = entry[1].substr(1, 3);
    auto right = entry[1].substr(6, 3);

    result.nodes[entry[0]] = {left, right};
  }

  return result;
}

void print_network(const Network &entries) {
  for (auto move : entries.directions) {
    print(move);
  }
  println();

  for (auto &pair : entries.nodes) {
    println(pair.first, " : ", pair.second[0], ",", pair.second[1]);
  }
}

vector<string> node_ends_with(const char &value, const Network &network) {
  vector<string> result;

  for (auto &pair : network.nodes)
    if (pair.first[2] == value)
      result.push_back(pair.first);

  return result;
}

vector<uint64_t> start_and_cycle(const string &node, const Network &network) {

  uint64_t steps = 0, index = 0, //
      start = 0, cycle = 0;

  string current = node, first;
  for (;;) {
    auto move = network.directions[index];
    auto next = network.nodes.at(current)[move];

    steps++;
    current = next;

    if (current == first) {
      break;
    }

    if (current[2] == 'Z') {
      start = steps;
      first = current;
    }

    index++;
    if (index >= network.directions.size())
      index = 0;
  }

  return {start, steps - start};
}

int part1(const Network &network, string start, string end) {
  int steps = 0, index = 0;

  string current = start;

  for (;;) {
    auto move = network.directions[index];
    auto next = network.nodes.at(current)[move];

    steps++;

    if (next == end)
      return steps;

    current = next;

    index++;
    if (index >= network.directions.size())
      index = 0;
  }

  return 0;
}

uint64_t part2(const Network &network) {
  uint64_t steps = 0, index = 0;

  auto current = node_ends_with('A', network);

  vector<uint64_t> starts, cycles;
  for (auto &node : current) {
    auto data = start_and_cycle(node, network);
    starts.push_back(data[0]);
    cycles.push_back(data[1]);
  }

  uint64_t mcm = 1;
  for (int i = 0; i < current.size(); i++) {
    mcm = lcm(mcm, cycles[i]);
  }

  return mcm;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto network = read_instr(lines);

  auto r1 = part1(network, "AAA", "ZZZ");
  println("Part 1: ", r1);

  auto r2 = part2(network);
  println("Part 2: ", r2);

  return 0;
}
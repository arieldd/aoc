#include "utils.h"

using namespace std;

struct Command {
  int move;
  int amount;
};

vector<Command> parse_commands(const vector<string> lines) {
  vector<Command> result;

  for (auto &l : lines) {
    auto parts = split(l, ' ');
    auto move = (parts[0] == "forward") ? 0 : (parts[0] == "up") ? -1 : 1;

    result.push_back({move, stoi(parts[1])});
  }

  return result;
}

vector<Command> parse_input(const string &file_name) {
  vector<string> ret;

  ifstream fs(file_name);
  string line;
  while (getline(fs, line)) {
    ret.push_back(line);
  }
  return parse_commands(ret);
}

int part1(const vector<Command> &commands) {
  int x = 0, d = 0;

  for (auto &c : commands) {
    if (c.move) {
      d += c.move * c.amount;
    } else
      x += c.amount;
  }
  return x * d;
}

int part2(const vector<Command> &commands) {
  int x = 0, d = 0, aim = 0;

  for (auto &c : commands) {
    if (c.move) {
      aim += c.move * c.amount;
    } else {
      x += c.amount;
      d += aim * c.amount;
    }
  }
  return x * d;
}

int main(int argc, char *argv[]) {
  auto commands = parse_input(argv[1]);

  auto r1 = part1(commands);
  println("Part 1: ", r1);

  auto r2 = part2(commands);
  println("Part 2: ", r2);

  return 0;
}
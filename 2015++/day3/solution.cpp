#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <set>
#include <string>
#include <vector>

#include "..\utils.h"

using namespace std;

struct move_t {
  int x;
  int y;
};

move_t operator+(const move_t &lhs, const move_t &rhs) {
  return {lhs.x + rhs.x, lhs.y + rhs.y};
}

bool operator<(const move_t &lhs, const move_t &rhs) {
  return (lhs.x < rhs.x) || ((!(rhs.x < lhs.x)) && (lhs.y < rhs.y));
}

const map<char, move_t> deltas{
    {'>', {1, 0}}, {'v', {0, 1}}, {'<', {-1, 0}}, {'^', {0, -1}}};

vector<move_t> parse_input(const string &file_name) {
  vector<std::string> ret;

  ifstream fs(file_name);
  string line;
  while (getline(fs, line)) {
    ret.push_back(line);
  }

  vector<move_t> result;
  for (auto &line : ret) {
    for (auto &c : line) {
      result.push_back(deltas.find(c)->second);
    }
  }
  return result;
}

int part1(const vector<move_t> &moves) {
  set<move_t> visited{{0, 0}};
  move_t current_pos = {0, 0};

  for (auto &move : moves) {
    current_pos = current_pos + move;
    if (visited.find(current_pos) == visited.end()) {
      visited.insert(current_pos);
    }
  }

  return visited.size();
}

int part2(const vector<move_t> &moves) {
  set<move_t> visited{{0, 0}};
  move_t santa_pos = {0, 0}, robot_pos = {0, 0};

  for (int i = 0; i < moves.size() - 1; i += 2) {
    santa_pos = santa_pos + moves[i];
    robot_pos = robot_pos + moves[i + 1];
    if (visited.find(santa_pos) == visited.end()) {
      visited.insert(santa_pos);
    }
    if (visited.find(robot_pos) == visited.end()) {
      visited.insert(robot_pos);
    }
  }

  return visited.size();
}

int main() {
  auto moves = parse_input("input.txt");
  auto r1 = part1(moves);
  println("Part 1: ", r1);
  auto r2 = part2(moves);
  println("Part 2: ", r2);
  return 0;
}
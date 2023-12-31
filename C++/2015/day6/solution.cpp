#include "utils.h"

using namespace std;
using namespace aoc_utils;

enum Action { On, Toggle, Off };

struct Instr_t {
  Action action;
  vector<pair<int, int>> range;
};

vector<Instr_t> read_instructions(const vector<string> &lines) {
  vector<Instr_t> res;

  for (auto &l : lines) {
    Instr_t next;

    auto parts = split(l, ' ');
    if (parts.size() == 4) { // toggle instruction{
      next.action = Toggle;
    } else {
      next.action = (parts[1] == "on") ? On : Off;
    }

    auto range_start = parts[parts.size() - 3], range_end = parts.back();

    for (auto str : {range_start, range_end}) {
      auto values = split(str, ',');
      next.range.push_back({stoi(values[0]), stoi(values[1])});
    }
    res.push_back(next);
  }

  //   for(auto & i : res){
  //     println(i.action, " ", i.range[0].first, ",", i.range[0].second, " to
  //     ", i.range[1].first, ",", i.range[1].second);
  //   }

  return res;
}

int part1(const vector<Instr_t> &instructions) {
  vector<vector<bool>> lights(1000, vector<bool>(1000));

  for (auto &instr : instructions) {
    for (int i = instr.range[0].first; i <= instr.range[1].first; i++)
      for (int j = instr.range[0].second; j <= instr.range[1].second; j++) {
        if (instr.action == On)
          lights[i][j] = true;
        else if (instr.action == Off)
          lights[i][j] = false;
        else
          lights[i][j] = !lights[i][j];
      }
  }

  int lights_on = 0;
  for (int i = 0; i < 1000; i++)
    for (int j = 0; j < 1000; j++) {
      if (lights[i][j])
        lights_on++;
    }

  return lights_on;
}

int64_t part2(const vector<Instr_t> &instructions) {
  vector<vector<int64_t>> lights(1000, vector<int64_t>(1000, 0));

  for (auto &instr : instructions) {
    for (int i = instr.range[0].first; i <= instr.range[1].first; i++)
      for (int j = instr.range[0].second; j <= instr.range[1].second; j++) {
        if (instr.action == On)
          lights[i][j]++;
        else if (instr.action == Off && lights[i][j] > 0)
          lights[i][j]--;
        else if (instr.action == Toggle)
          lights[i][j] += 2;
      }
  }

  int64_t brightnes = 0;
  for (int i = 0; i < 1000; i++)
    for (int j = 0; j < 1000; j++) {
      brightnes += lights[i][j];
    }

  return brightnes;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto instructions = read_instructions(lines);

  auto r1 = part1(instructions);
  println("Part 1: ", r1);

  auto r2 = part2(instructions);
  println("Part 2: ", r2);

  return 0;
}
#include "utils.h"
#include <__fwd/get.h>
#include <cassert>
using namespace std;
using namespace aoc_utils;

#define ll long long
#define arr array

enum InstrType {
  adv = 0,
  bxl = 1,
  bst = 2,
  jnz = 3,
  bxc = 4,
  out = 5,
  bdv = 6,
  cdv = 7,
};

struct Instr {
  InstrType op;
  char arg;
};

struct Computer {
  ll A, B, C;
  vector<Instr> program;
};

Computer read_input(const string &filename) {
  Computer result;
  ifstream fs(filename);

  string line;
  getline(fs, line);
  result.A = nums<ll>(line)[0];
  getline(fs, line);
  result.B = nums<ll>(line)[0];
  getline(fs, line);
  result.C = nums<ll>(line)[0];
  getline(fs, line);
  // Ignore new line
  getline(fs, line);
  auto values = nums<char>(line);
  for (ll i = 0; i < values.size(); i += 2)
    result.program.push_back(
        Instr{.op = static_cast<InstrType>(values[i]), .arg = values[i + 1]});

  return result;
}

ll combo(ll value, const Computer &pc) {
  switch (value) {
  case 0:
  case 1:
  case 2:
  case 3:
    return value;
  case 4:
    return pc.A;
  case 5:
    return pc.B;
  case 6:
    return pc.C;
  default:
    cout << "What's this " << value << '\n';
    assert(0 && "not a valid program");
    return -1;
  }
}

vector<char> run_program(Computer pc, bool once = false) {
  vector<char> output;
  auto n = pc.program.size();
  for (int ip = 0; ip < n; ip++) {
    auto instr = pc.program[ip];
    switch (instr.op) {
    case adv:
      pc.A /= 1 << combo(instr.arg, pc);
      break;
    case bxl:
      pc.B ^= instr.arg;
      break;
    case bst:
      pc.B = combo(instr.arg, pc) % 8;
      break;
    case jnz: {
      if (pc.A and !once) {
        ip = instr.arg - 1;
      }
      break;
    }
    case bxc:
      pc.B ^= pc.C;
      break;
    case out:
      output.push_back(combo(instr.arg, pc) % 8);
      break;
    case bdv:
      pc.B = pc.A / (1 << combo(instr.arg, pc));
      break;
    case cdv:
      pc.C = pc.A / (1 << combo(instr.arg, pc));
      break;
    }
  }
  return output;
}

string part1(const Computer &pc) {
  auto output = run_program(pc);

  string result;
  for (auto v : output)
    result += to_string(v) + ',';
  result.pop_back();
  return result;
}

ll find_A(Computer pc, const vector<char> &target, char shift, int index) {
  if (index == target.size()) {
    return pc.A;
  }

  vector<char> expected(target.end() - index - 1, target.end());

  ll start = pc.A << shift;
  for (ll rem = 0; rem <= 8; rem++) {
    pc.A = start + rem;
    auto output = run_program(pc);
    if (output == expected) {
      ll value = find_A(pc, target, shift, index + 1);
      if (value != -1) {
        return value;
      }
    }
  }
  return -1;
}

ll part2(Computer pc) {
  vector<char> target;
  for (const auto &instr : pc.program) {
    target.push_back(instr.op);
    target.push_back(instr.arg);
  }

  pc.A = 0;
  ll result = find_A(pc, target, 3, 0);
  if (result != -1) {
    pc.A = result;
    cout << part1(pc) << '\n';
  }
  return result;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto pc = read_input(argv[1]);
  cout << "Part 1:" << part1(pc) << '\n';
  cout << "Part 2:" << part2(pc) << '\n';
  return 0;
}

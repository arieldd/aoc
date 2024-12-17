#include "utils.h"
#include <algorithm>
#include <cassert>
#include <numeric>
#include <string>
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
  ll arg;
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
  auto values = nums<ll>(line);
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

vector<ll> run_program(Computer pc, bool once = false) {
  vector<ll> output;
  auto n = pc.program.size();
  for (int ip = 0; ip < n; ip++) {
    auto instr = pc.program[ip];
    switch (instr.op) {
    case adv:
      pc.A /= pow(2, combo(instr.arg, pc));
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
      pc.B = pc.A / pow(2, combo(instr.arg, pc));
      break;
    case cdv:
      pc.C = pc.A / pow(2, combo(instr.arg, pc));
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
  return result;
}

ll find_A(Computer pc, const vector<ll> &target, ll factor, int index) {
  if (index == target.size()) {
    return pc.A;
  }

  vector<ll> expected;
  for (int i = index; i >= 0; i--)
    expected.push_back(target[i]);

  ll start = pc.A * factor;
  for (ll rem = 0; rem <= 8; rem++) {
    pc.A = start + rem;
    auto output = run_program(pc);
    if (output == expected) {
      ll value = find_A(pc, target, factor, index + 1);
      if (value != -1) {
        return value;
      }
    }
  }
  return -1;
}

ll part2(Computer pc, ll factor) {
  vector<ll> target;
  for (const auto &instr : pc.program) {
    target.push_back(instr.op);
    target.push_back(instr.arg);
  }
  reverse(target.begin(), target.end());

  pc.A = 0;
  ll result = find_A(pc, target, factor, 0);
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
  cout << "Part 2:" << part2(pc, 8) << '\n';
  return 0;
}

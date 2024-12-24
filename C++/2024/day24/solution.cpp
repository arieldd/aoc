#include "utils.h"
#include <algorithm>
#include <cassert>
#include <ranges>
using namespace std;
using namespace aoc_utils;

#define ll long long
#define arr array

enum GateType { AND, OR, XOR };

ostream &operator<<(auto &os, GateType &gate) {
  switch (gate) {
  case AND:
    os << "AND";
    break;
  case OR:
    os << "OR";
    break;
  case XOR:
    os << "XOR";
    break;
  }
  return os;
}

struct Graph {
  map<string, char> wire_values;
  map<string, tuple<string, string, GateType>> outputs;
  map<tuple<string, string, GateType>, string> gates;
  set<string> bits;
  set<arr<string, 2>> inputs;
};

Graph read_input(const string &filename) {
  Graph conn;
  ifstream fs(filename);

  string line;
  bool input_values = true;
  vector<arr<string, 2>> inputs;
  int index;
  while (getline(fs, line)) {
    if (line.empty()) {
      input_values = false;
      continue;
    }
    if (input_values) {
      auto wire = split(line, ':');
      char value = wire[1][0] - '0';
      conn.wire_values[wire[0]] = value;
      if (wire[0] == "y00")
        index = 0;
      if (wire[0].starts_with('x'))
        inputs.push_back({wire[0], ""});
      else
        inputs[index][1] = wire[1];
      index++;
    } else {
      auto parts = split(line, ' ');
      GateType op = (parts[1] == "AND") ? AND : (parts[1] == "OR") ? OR : XOR;
      auto gate = tie(parts[0], parts[2], op);
      conn.outputs[parts.back()] = gate;
      if (parts.back().starts_with('z'))
        conn.bits.insert(parts.back());
      arr<string, 2> entry = {parts[0], parts[2]};
      sort(entry.begin(), entry.end());
      conn.gates.insert({tie(entry[0], entry[1], op), parts.back()});
    }
  }
  for (auto in : inputs)
    conn.inputs.insert(in);
  return conn;
}

ll to_decimal(const Graph &conn, char letter, int index = -1) {
  ll result = 0, pow = 0;
  for (auto [wire, value] : conn.wire_values) {
    if (wire.starts_with(letter)) {
      if (value)
        result += (ll)1 << pow;
      if (index >= 0 and nums<int>(wire)[0] == index)
        break;

      pow++;
    }
  }
  return result;
}

char run_gate(const Graph &conn, string wire) {
  if (conn.wire_values.contains(wire))
    return conn.wire_values.at(wire);

  assert(conn.outputs.contains(wire) && "Hmmm\n");

  char output = -1;
  auto [w1, w2, op] = conn.outputs.at(wire);

  auto op1 = run_gate(conn, w1);

  auto op2 = run_gate(conn, w2);

  switch (op) {
  case AND:
    output = op1 and op2;
    break;
  case OR:
    output = op1 or op2;
    break;
  case XOR:
    output = op1 xor op2;
    break;
  }

  return output;
}

ll part1(Graph conn, int index = -1) {
  auto n = (index >= 0) ? index : conn.bits.size();
  for (int i = 0; i < n; i++) {
    auto output = *next(conn.bits.begin(), i);
    conn.wire_values[output] = run_gate(conn, output);
  }

  return to_decimal(conn, 'z', index);
}

string part2(Graph conn, ll original) {
  cout << conn.gates.size() << '\n';
  ll target = to_decimal(conn, 'x') + to_decimal(conn, 'y');

  set<string> swaps;
  string cin = "";
  for (auto [x, y] : conn.inputs) {
    string c1, p_sum, c2, carry, sum;
    for (auto op : {XOR, AND}) {
      auto gate = tie(x, y, op);
      if (!conn.gates.contains(gate))
        continue;
      if (op == XOR) {
        p_sum = conn.gates.at(gate);
      } else if (op == AND) {
        c1 = conn.gates.at(gate);
      }
    }
    if (!cin.empty() and !p_sum.empty()) {
      for (auto op : {XOR, AND}) {
        auto gate = tie(cin, p_sum, op);
        if (!conn.gates.contains(gate)) {
        }
        if (op == XOR) {
          sum = conn.gates.at(gate);
          if (!sum.starts_with('z') or nums<int>(sum)[0] != nums<int>(x)[0])
            swaps.insert(sum);
        } else if (op == AND) {
          c2 = conn.gates.at(gate);
        }
      }
    }
    if (!c1.empty() and !c2.empty()) {
      GateType op = OR;
      auto gate = tie(c1, c2, op);
      if (conn.gates.contains(gate)) {
        carry = conn.gates.at(gate);
      }
    }
    cin = carry;
  }
  string result = "";
  for (auto key : swaps)
    result += key + ',';
  if (!result.empty())
    result.pop_back();
  return result;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto conn = read_input(argv[1]);
  auto original = part1(conn);
  cout << "Part 1:" << original << '\n';
  cout << "Part 2:" << part2(conn, original) << '\n';
  return 0;
}

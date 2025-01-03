#include "utils.h"
#include <cassert>
using namespace std;
using namespace aoc_utils;

#define ll long long
#define arr array

enum GateType { AND, OR, XOR };

struct Wires {
  map<string, char> values;
  map<string, tuple<string, string, GateType>> outputs;
  map<tuple<string, string, GateType>, string> gates;
  set<string> out_bits;
  set<arr<string, 2>> inputs;
};

Wires read_input(const string &filename) {
  Wires conn;
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
      conn.values[wire[0]] = value;
      if (wire[0] == "y00")
        index = 0;
      if (wire[0].starts_with('x'))
        inputs.push_back({wire[0], ""});
      else
        inputs[index][1] = wire[0];
      index++;
    } else {
      auto parts = split(line, ' ');
      GateType op = (parts[1] == "AND") ? AND : (parts[1] == "OR") ? OR : XOR;

      arr<string, 2> entry = {parts[0], parts[2]};
      sort(entry.begin(), entry.end());

      auto gate = tie(entry[0], entry[1], op);
      conn.outputs[parts.back()] = gate;

      if (parts.back().starts_with('z'))
        conn.out_bits.insert(parts.back());

      conn.gates.insert({gate, parts.back()});
    }
  }
  for (auto in : inputs)
    conn.inputs.insert(in);
  return conn;
}

ll to_decimal(const string &bits) {
  ll result = 0, pow = 0;
  for (auto c : bits) {
    if (c)
      result += (ll)1 << pow;

    pow++;
  }
  return result;
}

char gate_value(const Wires &conn, string wire) {
  if (conn.values.contains(wire))
    return conn.values.at(wire);

  assert(conn.outputs.contains(wire) && "Hmmm\n");

  char output = -1;
  auto [w1, w2, op] = conn.outputs.at(wire);

  auto op1 = gate_value(conn, w1);

  auto op2 = gate_value(conn, w2);

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

ll part1(const Wires &conn) {
  auto n = conn.out_bits.size();
  string bits{};
  for (auto &bit : conn.out_bits) {
    bits += gate_value(conn, bit);
  }

  return to_decimal(bits);
}

arr<string, 2> find_pair(const Wires &conn, string w1, string w2, GateType op) {
  for (auto [gate, out] : conn.gates) {
    auto [g1, g2, g_op] = gate;
    if (g_op == op) {
      if (g1 == w1)
        return {w2, g2};
      if (g1 == w2)
        return {w1, g2};
      if (g2 == w1)
        return {w2, g1};
      if (g2 == w2)
        return {w1, g1};
    }
  }
  return {};
}

// Build a full adder circuit and check when the expected inputs are wrong
string part2(Wires conn) {

  set<string> swaps;
  string cin = "";
  for (auto [x, y] : conn.inputs) {
    int bit_idx = nums<int>(x)[0];
    string psum, c1, c2, carry, sum;

    // First half adder with the inputs
    for (auto op : {XOR, AND}) {
      auto gate = tie(x, y, op);
      if (!conn.gates.contains(gate)) {
        continue;
      };
      if (op == XOR) {
        psum = conn.gates.at(gate);
      } else if (op == AND) {
        c1 = conn.gates.at(gate);
      }
    }

    // Second half adder
    if (!cin.empty() and !psum.empty()) {
      for (auto op : {XOR, AND}) {
        auto gate = tie(min(cin, psum), max(cin, psum), op);
        if (!conn.gates.contains(gate)) {
          auto [wrong, right] = find_pair(conn, cin, psum, op);
          if (!swaps.contains(wrong) and !swaps.contains(right)) {
            swaps.insert(wrong);
            swaps.insert(right);
          }
          auto g1 = conn.outputs.at(wrong);
          auto g2 = conn.outputs.at(right);

          swap(conn.gates.at(g1), conn.gates.at(g2));
          for (auto key : {&cin, &c1, &psum, &c2})
            if (*key == wrong) {
              *key = right;
              break;
            }

          continue;
        }

        if (op == XOR) {
          // This the output bit
          sum = conn.gates.at(gate);
          if (!sum.starts_with('z') or nums<int>(sum)[0] != nums<int>(x)[0]) {

            string correct_sum;
            for (auto z : conn.out_bits)
              if (nums<int>(z)[0] == bit_idx) {
                correct_sum = z;
                break;
              }

            auto correct_gate = conn.outputs.at(correct_sum);
            swap(conn.gates.at(gate), conn.gates.at(correct_gate));

            for (auto key : {&cin, &c1, &psum, &c2})
              if (*key == correct_sum) {
                *key = sum;
              }

            swaps.insert(sum);
            swaps.insert(correct_sum);
          }
        } else if (op == AND) {
          c2 = conn.gates.at(gate);
        }
      }
    }

    // Finally get the carry with an OR gate
    carry = c1;
    if (!c1.empty() and !c2.empty()) {
      GateType op = OR;

      auto gate = tie(min(c1, c2), max(c1, c2), op);
      if (conn.gates.contains(gate)) {
        carry = conn.gates.at(gate);
      } else {
        auto [wrong, right] = find_pair(conn, c1, c2, op);
        if (!swaps.contains(wrong) and !swaps.contains(right)) {
          swaps.insert(wrong);
          swaps.insert(right);
        }
        auto g1 = conn.outputs.at(wrong);
        auto g2 = conn.outputs.at(right);

        swap(conn.gates.at(g1), conn.gates.at(g2));

        for (auto key : {&cin, &c1, &psum, &c2, &carry})
          if (*key == wrong) {
            *key = right;
          }
      }
    }
    cin = carry;
  }

  string result = "\n";
  for (auto key : swaps)
    result += key + ',';
  if (!result.empty())
    result.pop_back();
  return result;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto conn = read_input(argv[1]);
  cout << "Part 1:" << part1(conn) << '\n';
  cout << "Part 2:" << part2(conn) << '\n';
  return 0;
}

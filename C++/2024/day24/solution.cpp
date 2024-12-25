#include "utils.h"
#include <algorithm>
#include <cassert>
#include <ranges>
#include <utility>
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

struct Data {
  map<string, char> wire_values;
  map<string, tuple<string, string, GateType>> outputs;
  map<tuple<string, string, GateType>, string> gates;
  set<string> bits;
  set<arr<string, 2>> inputs;
};

Data read_input(const string &filename) {
  Data conn;
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
        conn.bits.insert(parts.back());

      conn.gates.insert({gate, parts.back()});
    }
  }
  for (auto in : inputs)
    conn.inputs.insert(in);
  return conn;
}

ll to_decimal(const Data &conn, char letter, int index = -1) {
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

char run_gate(const Data &conn, string wire) {
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

ll part1(Data conn, int index = -1) {
  auto n = (index >= 0) ? index : conn.bits.size();
  for (int i = 0; i < n; i++) {
    auto output = *next(conn.bits.begin(), i);
    conn.wire_values[output] = run_gate(conn, output);
  }

  return to_decimal(conn, 'z', index);
}

arr<string, 2> find_pair(const Data &conn, string w1, string w2, GateType op) {
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

string part2(Data conn, ll original) {

  set<string> swaps;
  string cin = "";
  for (auto [x, y] : conn.inputs) {
    println(x, " ", y);
    int bit_idx = nums<int>(x)[0];
    string c1, p_sum, c2, carry, sum;

    for (auto op : {XOR, AND}) {
      auto gate = tie(x, y, op);
      if (!conn.gates.contains(gate)) {
        continue;
      };
      if (op == XOR) {
        p_sum = conn.gates.at(gate);
      } else if (op == AND) {
        c1 = conn.gates.at(gate);
      }
    }
    if (!cin.empty() and !p_sum.empty()) {
      for (auto op : {XOR, AND}) {
        arr<string, 2> keys{cin, p_sum};
        sort(keys.begin(), keys.end());

        auto gate = tie(keys[0], keys[1], op);
        if (!conn.gates.contains(gate)) {
          auto [wrong, right] = find_pair(conn, cin, p_sum, op);
          swaps.insert(wrong);
          swaps.insert(right);

          auto g1 = conn.outputs.at(wrong);
          auto g2 = conn.outputs.at(right);

          swap(conn.gates.at(g1), conn.gates.at(g2));
          cout << "Swapped " << wrong << " with " << right << '\n';
          for (auto key : {&cin, &c1, &p_sum, &c2, &carry})
            if (*key == wrong) {
              *key = right;
              break;
            }
          continue;
        }
        if (op == XOR) {
          sum = conn.gates.at(gate);
          if (!sum.starts_with('z') or nums<int>(sum)[0] != nums<int>(x)[0]) {

            string correct_sum;
            for (auto z : conn.bits)
              if (nums<int>(z)[0] == bit_idx) {
                correct_sum = z;
                break;
              }

            auto correct_gate = conn.outputs.at(correct_sum);
            swap(conn.gates.at(gate), conn.gates.at(correct_gate));

            for (auto key : {&cin, &c1, &p_sum, &c2, &carry})
              if (*key == correct_sum) {
                *key = sum;
                break;
              }

            swaps.insert(sum);
            swaps.insert(correct_sum);
            cout << "Swapped " << sum << " with " << correct_sum << '\n';
          }
        } else if (op == AND) {
          c2 = conn.gates.at(gate);
        }
      }
    }
    carry = c1;
    if (!c1.empty() and !c2.empty()) {
      GateType op = OR;
      arr<string, 2> keys{c1, c2};
      sort(keys.begin(), keys.end());

      auto gate = tie(keys[0], keys[1], op);
      if (conn.gates.contains(gate)) {
        carry = conn.gates.at(gate);
      } else {
        auto [wrong, right] = find_pair(conn, c1, c2, op);
        swaps.insert(wrong);
        swaps.insert(right);

        auto g1 = conn.outputs.at(wrong);
        auto g2 = conn.outputs.at(right);

        for (auto key : {&cin, &c1, &p_sum, &c2, &carry})
          if (*key == wrong) {
            *key = right;
            break;
          }
        continue;
        cout << "Swapped " << wrong << " with " << right << '\n';

        swap(conn.gates.at(g1), conn.gates.at(g2));
      }
    }
    cin = carry;
  }
  cout << part1(conn) << "=" << to_decimal(conn, 'x') + to_decimal(conn, 'y')
       << '\n';
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

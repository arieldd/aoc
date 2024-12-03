#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <regex>
#include <sstream>
#include <string>
#include <vector>
using namespace std;

#define pint pair<int, int>

vector<string> read_input(const string &filename) {
  vector<string> lines{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    lines.push_back(line);
  return lines;
}

int part1_regex(const vector<string> &lines) {
  regex pattern("mul\\(([0-9]+),([0-9]+)\\)");
  auto end = sregex_iterator();

  int result = 0;
  for (auto &line : lines) {
    auto iter = sregex_iterator(line.begin(), line.end(), pattern);
    for (auto it = iter; it != end; it++) {
      smatch match = *it;
      int v1 = stoi(match[1]), v2 = stoi(match[2]);
      result += v1 * v2;
    }
  }
  return result;
}

int part2_regex(const vector<string> &lines) {
  regex mult_pattern("mul\\(([0-9]+),([0-9]+)\\)");
  regex change_pattern("do\\(\\)|don't\\(\\)");
  auto end = sregex_iterator();

  int result = 0;
  bool state = true;
  for (auto &line : lines) {
    vector<pint> values;
    vector<int> changes;

    auto m_matches = sregex_iterator(line.begin(), line.end(), mult_pattern);
    for (auto it = m_matches; it != end; it++) {
      smatch match = *it;
      int v1 = stoi(match[1]), v2 = stoi(match[2]);
      values.push_back({v1 * v2, match.position()});
    }

    bool line_state = state;
    auto c_matches = sregex_iterator(line.begin(), line.end(), change_pattern);
    for (auto it = c_matches; it != end; it++) {
      smatch match = *it;
      if (match.str() == "do()" and line_state)
        continue;

      if (match.str() == "don't()" and !line_state)
        continue;

      line_state = !line_state;

      changes.push_back(match.position());
    }

    int mi = 0, ci = 0;
    for (; mi < values.size() and ci < changes.size();) {
      if (values[mi].second < changes[ci]) {
        if (state)
          result += values[mi].first;
        mi++;
      } else {
        state = !state;
        ci++;
      }
    }
    if (state) {
      for (; mi < values.size(); mi++) {

        result += values[mi].first;
      }
    }
  }
  return result;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto lines = read_input(argv[1]);

  auto p1_regex = part1_regex(lines);
  auto p2_regex = part2_regex(lines);
  cout << "Part 1: " << p1_regex << '\n';
  cout << "Part 2: " << p2_regex << '\n';
  return 0;
}

/* Original solution with manual parsing
struct Mul {
  int value;
  unsigned long start_pos;
  unsigned long end_pos;
};

int to_decimal(vector<char> number) {

  auto value = 0;
  auto i = 0;
  for (auto it = number.rbegin(); it != number.rend(); it++) {
    value += *it * pow(10, i);
    i++;
  }
  return (int)value;
}

vector<vector<Mul>> part1(const vector<string> &lines) {
  vector<vector<Mul>> valid_mults;
  unsigned long line_ctr = 0;
  for (auto &line : lines) {
    auto n = line.size();
    valid_mults.push_back({});
    stringstream ss(line);
    auto last_pos = 0;
    auto open = line.find("mul(");
    while (open != string::npos) {

      auto comma = line.find(',', open + 4);
      if (comma == string::npos) {
        last_pos = open + 4;
        open = line.find("mul(", last_pos);
        continue;
      }

      vector<char> number;
      bool nan = false;
      for (auto i = open + 4; i < comma; i++) {
        if (!isdigit(line[i])) {
          nan = true;
          break;
        }
        number.push_back(line[i] - '0');
      }
      if (nan) {
        last_pos = open + 4;
        open = line.find("mul(", last_pos);
        continue;
      }

      auto v1 = to_decimal(number);
      number.clear();

      auto close = line.find(')', comma + 1);
      if (close == string::npos) {
        last_pos = open + 4;
        open = line.find("mul(", last_pos);
        continue;
      }

      for (auto i = comma + 1; i < close; i++) {
        if (!isdigit(line[i])) {
          nan = true;
          break;
        }
        number.push_back(line[i] - '0');
      }
      if (nan) {
        last_pos = open + 4;
        open = line.find("mul(", last_pos);
        continue;
      }
      auto v2 = to_decimal(number);
      valid_mults[line_ctr].push_back({
          .value = v1 * v2,
          .start_pos = open,
          .end_pos = close,
      });

      last_pos = close + 1;
      open = line.find("mul(", last_pos);
    }
    line_ctr++;
  }

  return valid_mults;
}

int part2(const vector<string> &lines, const vector<vector<Mul>> mults) {
  bool do_state = true;
  auto result = 0;
  auto line_ctr = 0;
  vector<vector<unsigned long>> changes;
  for (const auto &line : lines) {
    auto n = line.size();
    changes.push_back({});
    auto idx = line.find((do_state) ? "don't()" : "do()");
    while (idx != string::npos) {
      changes[line_ctr].push_back(idx);
      do_state = !do_state;
      if (do_state)
        idx = line.find("don't()", idx + 4);
      else {
        idx = line.find("do()", idx + 7);
      }
    }
    line_ctr++;
  }

  do_state = true;
  for (int i = 0; i < changes.size(); i++) {
    auto mi = 0;
    auto ci = 0;
    while (ci < changes[i].size() and mi < mults[i].size()) {
      auto change = changes[i][ci];
      auto mult = mults[i][mi];
      if (mult.start_pos < change) {
        if (do_state)
          result += mult.value;
        mi++;
      } else {
        do_state = !do_state;
        ci++;
      }
    }
    if (do_state) {
      for (; mi < mults[i].size(); mi++)
        result += mults[i][mi].value;
    }
  }
  return result;
} */

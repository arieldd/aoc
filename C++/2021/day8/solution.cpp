#include "utils.h"

using namespace std;
using namespace aoc_utils;

int char_diff(const string &s1, const string &s2) {
  int diff = abs(int(s1.size() - s2.size()));

  for (auto &c : s1) {
    if (s2.find(c) == string::npos)
      diff++;
  }
  return diff;
}

struct Pattern {
  vector<string> signals;
  vector<string> output;

  map<int, string> encoding;
  void process_encoding() {

    vector<string> unknowns;
    // Preprocess known values
    for (auto &s : signals) {
      string code = s;
      sort(code.begin(), code.end());
      auto length = code.size();
      if (length == 2)
        encoding[1] = code;
      else if (length == 3)
        encoding[7] = code;
      else if (length == 4)
        encoding[4] = code;
      else if (length == 7)
        encoding[8] = code;
      else
        unknowns.push_back(code);
    }

    // Deduce remaining
    for (auto &s : unknowns) {
      auto length = s.size();
      if (length == 5) { // 2 3 or 5
        if (char_diff(encoding[7], s) == 2)
          encoding[3] = s;
        else if (char_diff(encoding[4], s) == 2)
          encoding[5] = s;
        else
          encoding[2] = s;
      } else { // 0 6 or 9
        if (char_diff(encoding[4], s) == 2)
          encoding[9] = s;
        else if (char_diff(encoding[7], s) == 3)
          encoding[0] = s;
        else
          encoding[6] = s;
      }
    }
  }

  int decode() const {
    int result = 0;
    for (auto &o : output) {
      result *= 10;

      string code = o;
      sort(code.begin(), code.end());
      for (auto &pair : encoding) {
        if (pair.second == code)
          result += pair.first;
      }
    }

    return result;
  }
};

vector<Pattern> read_patterns(const vector<string> &lines) {
  vector<Pattern> res;
  for (auto &l : lines) {
    Pattern next;
    auto sides = split(l, '|');
    auto signals = split(sides[0], ' ');
    for (auto &s : signals)
      next.signals.push_back(s);

    auto outputs = split(sides[1], ' ');
    for (auto &o : outputs)
      next.output.push_back(o);

    res.push_back(next);
  }

  return res;
}

int part1(const vector<Pattern> &patterns) {
  int count = 0;

  for (auto &p : patterns) {
    for (auto &o : p.output) {
      auto length = o.size();
      if (length == 2 || length == 3 || length == 4 || length == 7)
        count++;
    }
  }

  return count;
}

int part2(vector<Pattern> &patterns) {

  int sum = 0;
  for (auto &p : patterns) {
    p.process_encoding();

    auto value = p.decode();
    sum += value;
  }
  return sum;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto patterns = read_patterns(lines);

  auto r1 = part1(patterns);
  println("Part 1: ", r1);

  auto r2 = part2(patterns);
  println("Part 2: ", r2);

  return 0;
}
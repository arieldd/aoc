#include "utils.h"

using namespace std;
using namespace aoc_utils;

int part1(const vector<string> &lines) {
  const set<string> not_allowed{"ab", "cd", "pq", "xy"};
  const set<char> vowels{'a', 'e', 'i', 'o', 'u'};

  int vowel_count;
  bool double_letter, naughty;

  int count_nice = 0;
  for (auto &l : lines) {
    auto length = l.size();

    vowel_count = 0;
    double_letter = false;
    naughty = false;

    if (vowels.find(l[0]) != vowels.end())
      vowel_count++;

    char previous = l[0];
    for (int i = 1; i < length; i++) {
      if (not_allowed.find(string() + previous + l[i]) != not_allowed.end()) {
        naughty = true;
        break;
      }

      if (l[i] == previous)
        double_letter = true;

      if (vowels.find(l[i]) != vowels.end())
        vowel_count++;

      previous = l[i];
    }

    if (!naughty && vowel_count >= 3 && double_letter)
      count_nice++;
  }

  return count_nice;
}

int part2(const vector<string> &lines) {
  int count_nice = 0;

  bool rule1, rule2;
  for (auto &l : lines) {
    auto length = l.size();
    map<string, vector<int>> pairs;

    rule1 = false;
    for (int i = 0; i < length - 1; i++) {
      auto pair = string() + l[i] + l[i + 1];
      pairs[pair].push_back(i);

      auto pos = pairs.at(pair);
      auto repeats = pos.size();
      if (repeats > 1) {
        for (int k = 0; k < repeats - 1; k++)
          for (auto h = k + 1; h < repeats; h++) {
            if (pos[h] - pos[k] > 1) {
              rule1 = true;
              break;
            }
          }
        if (rule1)
          break;
      }
    }

    rule2 = false;
    for (int i = 0; i < length - 2; i++) {
      if (l[i] == l[i + 2]) {
        rule2 = true;
        break;
      }
    }

    if (rule1 && rule2) {
      count_nice++;
    }
  }

  return count_nice;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);

  auto r1 = part1(lines);
  println("Part 1: ", r1);

  auto r2 = part2(lines);
  println("Part 2: ", r2);

  return 0;
}
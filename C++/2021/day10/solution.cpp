#include "utils.h"

using namespace std;
using namespace aoc_utils;

map<char, int> error_scoring{
    {')', 3},
    {']', 57},
    {'}', 1197},
    {'>', 25137},
};

map<char, int> incomplete_scoring{
    {')', 1},
    {']', 2},
    {'}', 3},
    {'>', 4},
};

map<char, char> closing_pairs{
    {')', '('},
    {']', '['},
    {'}', '{'},
    {'>', '<'},
};

map<char, char> opening_pairs{
    {'(', ')'},
    {'[', ']'},
    {'{', '}'},
    {'<', '>'},
};

int64_t find_incomplete_score(vector<char> &open) {
  int64_t score = 0;
  for (auto it = open.rbegin(); it != open.rend(); it++) {
    score *= 5;
    char closing_char = opening_pairs.at(*it);
    score += incomplete_scoring.at(closing_char);
  }

  return score;
}

pair<int, int64_t> solve(const vector<string> &lines) {

  int error_score = 0;
  vector<int64_t> incomplete_scores;
  for (auto &l : lines) {
    vector<char> open{l[0]};

    bool corrupted = false;
    for (int i = 1; i < l.size(); i++) {
      char current = l[i];
      if (closing_pairs.find(current) == closing_pairs.end()) {
        open.push_back(current);
      } else {
        char expected = closing_pairs.at(current);
        if (expected != open.back()) // Corrupted line
        {
          error_score += error_scoring.at(current);
          corrupted = true;
          break;
        } else
          open.pop_back();
      }
    }

    if (!corrupted) {
      incomplete_scores.push_back(find_incomplete_score(open));
    }
  }

  ranges::sort(incomplete_scores);
  auto mid_score = incomplete_scores[incomplete_scores.size() / 2];

  return {error_score, mid_score};
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);

  auto parts = solve(lines);
  println("Part 1: ", parts.first);
  println("Part 2: ", parts.second);

  return 0;
}
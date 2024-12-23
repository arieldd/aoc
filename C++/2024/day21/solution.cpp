#include "utils.h"
#include <cassert>
#include <climits>
#include <ranges>
using namespace std;
using namespace aoc_utils;

#define ll size_t
#define arr array

const map<char, arr<ll, 2>> NumPad = {
    {'7', {0, 0}}, {'8', {0, 1}}, {'9', {0, 2}}, //
    {'4', {1, 0}}, {'5', {1, 1}}, {'6', {1, 2}}, //
    {'1', {2, 0}}, {'2', {2, 1}}, {'3', {2, 2}},
    {' ', {3, 0}}, {'0', {3, 1}}, {'A', {3, 2}},
};

const map<char, arr<ll, 2>> DirPad = {
    {' ', {0, 0}}, {'^', {0, 1}}, {'A', {0, 2}},
    {'<', {1, 0}}, {'v', {1, 1}}, {'>', {1, 2}},
};

vector<string> read_input(const string &filename) {
  vector<string> lines{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    lines.push_back(line);
  return lines;
}

void find_paths(const map<char, arr<ll, 2>> &keypad, vector<string> &all_paths,
                arr<ll, 2> current, arr<ll, 2> end, string path) {

  if (!is_valid_pos(current[0], current[1], keypad.size() / 3, 3))
    return;

  if (current == end) {
    all_paths.push_back(path + 'A');
    return;
  }

  auto gap = keypad.at(' ');
  arr<ll, 2>                             //
      left{current[0], current[1] - 1},  //
      right{current[0], current[1] + 1}, //
      up{current[0] - 1, current[1]},    //
      down{current[0] + 1, current[1]};

  if (current[1] < end[1]) {
    find_paths(keypad, all_paths, right, end, path + '>');
  }
  if (current[1] > end[1] and left != gap) {
    find_paths(keypad, all_paths, left, end, path + '<');
  }
  if (current[0] > end[0] and up != gap) {
    find_paths(keypad, all_paths, up, end, path + '^');
  }
  if (current[0] < end[0] and down != gap) {
    find_paths(keypad, all_paths, down, end, path + 'v');
  }
}

ll best_path(char from, char to, int level,
             map<tuple<char, char, int>, ll> &cache) {

  auto entry = tie(from, to, level);
  if (cache.contains(entry))
    return cache.at(entry);

  ll best = LONG_MAX;
  vector<string> all_paths;
  find_paths(DirPad, all_paths, DirPad.at(from), DirPad.at(to), {'A'});
  for (auto &path : all_paths) {
    ll length = 0;
    for (auto [a, b] : path | views::pairwise) {
      length += (level == 1) ? 1 : best_path(a, b, level - 1, cache);
    }
    if (length < best)
      best = length;
  }

  cache.insert({entry, best});
  return best;
}

ll solve(const vector<string> &codes, int robots) {
  ll result = 0;

  map<tuple<char, char, int>, ll> cache;
  for (auto code : codes) {
    ll command_size = 0;
    code = 'A' + code;
    for (auto [num_from, num_to] : code | views::pairwise) {
      vector<string> all_paths;
      find_paths(NumPad, all_paths, NumPad.at(num_from), NumPad.at(num_to), {});
      ll best = 0;
      for (auto path : all_paths) {
        path = 'A' + path;
        ll path_length = 0;
        for (auto [from, to] : path | views::pairwise) {
          path_length += best_path(from, to, robots, cache);
        }
        if (!best or best > path_length)
          best = path_length;
      }
      command_size += best;
    }

    result += nums<ll>(code)[0] * command_size;
  }
  return result;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto codes = read_input(argv[1]);
  cout << "Part 1:" << solve(codes, 2) << '\n';
  cout << "Part 2:" << solve(codes, 25) << '\n';
  return 0;
}

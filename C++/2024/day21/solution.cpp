#include "utils.h"
#include <cassert>
#include <climits>
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

string dir_path(char start, char end) {
  string path;

  assert(start != ' ' and end != ' ' && "Panic mode!");

  auto gap = DirPad.at(' ');
  auto p0 = arr<ll, 2>(DirPad.at(start)); // Copy for modyfing
  auto p1 = DirPad.at(end);

  while (p0[1] < p1[1]) {
    p0[1]++;
    path.push_back('>');
  }
  while (p0[0] > p1[0]) {
    p0[0]--;
    path.push_back('^');
  }
  while (p0[0] < p1[0]) {
    p0[0]++;
    path.push_back('v');
  }
  while (p0[1] > p1[1]) {
    p0[1]--;
    path.push_back('<');
  }

  assert(p0 != gap && "Panic mode!");

  path.push_back('A');
  return path;
}

map<arr<char, 2>, ll> get_freq(const string &path) {
  map<arr<char, 2>, ll> freq;
  auto current = 'A';
  for (auto key : path) {
    freq[{current, key}]++;
    current = key;
  }
  return freq;
}

ll solve(const vector<string> &codes, ll robots) {
  ll result = 0;
  for (auto &code : codes) {
    map<char, vector<string>> paths;
    char current = 'A';
    for (auto key : code) {
      vector<string> key_paths;
      find_paths(NumPad, key_paths, NumPad.at(current), NumPad.at(key), {});
      paths[key] = key_paths;
      current = key;
    }

    ll command_size = 0;
    for (auto entry : paths) {
      ll best_size = 0;
      for (auto possible : entry.second) {
        auto pair_freq = get_freq(possible);
        ll path_size = 0;
        for (char i = 0; i < robots; i++) {
          map<arr<char, 2>, ll> next;
          path_size = 0;
          for (auto [pair, repeat] : pair_freq) {
            string expanded = dir_path(pair[0], pair[1]);
            path_size += (expanded.size() * repeat);
            for (auto [new_pair, _ /* Always 1 */] : get_freq(expanded))
              next[new_pair] += repeat;
          }
          pair_freq = next;
        }
        if (!best_size or path_size < best_size) {
          best_size = path_size;
        }
      }

      command_size += best_size;
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

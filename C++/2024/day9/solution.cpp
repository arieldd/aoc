#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#define ll long long

vector<string> read_input(const string &filename) {
  vector<string> lines{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    lines.push_back(line);
  return lines;
}

int part1(const vector<string> &lines) { return 0; }

int part2(const vector<string> &lines) { return 0; }

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");

  vector<int> blocks{};
  ifstream fs(argv[1]);

  string line;
  getline(fs, line);

  int file_id = 0;
  vector<vector<int>> full{};
  vector<vector<int>> empty{};
  for (int i = 0; i < line.size(); i++) {
    auto value = line[i] - '0';
    for (auto j = 0; j < value; j++) {
      if (i % 2)
        blocks.push_back(-1);
      else {
        blocks.push_back(file_id);
      }
    }

    if (i % 2) {
      empty.push_back({value, file_id, (int)blocks.size() - value});
    } else {
      full.push_back({value, file_id, (int)blocks.size() - value});
      file_id++;
    }
  }

  auto defrag = [](vector<int> &blocks) {
    for (int r = 0, l = blocks.size() - 1; r <= l; r++) {
      while (blocks[l] == -1 and r <= l)
        l--;
      if (r > l)
        break;
      if (blocks[r] == -1) {
        blocks[r] = blocks[l];
        blocks[l] = -1;
      }
    }
  };

  auto print_blocks = [](vector<int> blocks) {
    for (int i = 0; i < blocks.size(); i++) {
      if (blocks[i] >= 0)
        cout << blocks[i] << ' ';
      else
        cout << '.';
    }
    cout << '\n';
  };

  auto first = blocks;

  defrag(first);

  ll part1 = 0;
  for (int i = 0; i < first.size(); i++) {
    if (first[i] == -1)
      break;
    part1 += first[i] * i;
  }
  cout << "Part 1: " << part1 << '\n';

  auto defrag2 = [&](vector<int> &blocks) {
    int n = full.size(), m = empty.size();
    for (int e = 0; e < m;) {
      int f = full.size() - 1;
      while (f > 0 and empty[e][0] < full[f][0])
        f--;

      if (f <= 0 or empty[e][2] > full[f][2]) {
        e++;
        continue;
      }

      for (auto index = 0; index < full[f][0]; index++) {
        blocks[empty[e][2] + index] = full[f][1];
        blocks[full[f][2] + index] = -1;
      }

      empty[e][2] += full[f][0];
      empty[e][0] -= full[f][0];
      full.erase(full.begin() + f);
    }
  };

  auto second = blocks;
  defrag2(second);

  // print_blocks(second);

  ll part2 = 0;
  for (int i = 0; i < second.size(); i++) {
    if (second[i] == -1)
      continue;
    part2 += second[i] * i;
  }
  cout << "Part 2: " << part2 << '\n';
  return 0;
}

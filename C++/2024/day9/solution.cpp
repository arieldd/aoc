#include <cassert>
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

#define ll long long

struct Block {
  int file_id = -1, space, start;
};

ll checksum(const vector<int> &filesystem) {
  ll checksum = 0;
  for (int i = 0; i < filesystem.size(); i++) {
    if (filesystem[i] == -1)
      continue; // compatible with part 2, 'break' for part 1 only

    checksum += filesystem[i] * i;
  }
  return checksum;
}

ll part1(vector<int> filesystem) {
  for (int r = 0, l = filesystem.size() - 1; r <= l; r++) {
    while (filesystem[l] == -1 and r <= l)
      l--;
    if (r > l)
      break;
    if (filesystem[r] == -1) {
      filesystem[r] = filesystem[l];
      filesystem[l] = -1;
    }
  }

  return checksum(filesystem);
}

ll part2(vector<int> filesystem, vector<Block> full, vector<Block> empty) {
  for (int e = 0; e < empty.size();) {
    int f = full.size() - 1;
    while (f > 0 and empty[e].space < full[f].space)
      f--;

    if (f <= 0 or empty[e].start > full[f].start) {
      e++;
      continue;
    }

    for (auto index = 0; index < full[f].space; index++) {
      filesystem[empty[e].start + index] = full[f].file_id;
      filesystem[full[f].start + index] = -1;
    }

    empty[e].start += full[f].space;
    empty[e].space -= full[f].space;
    full.erase(full.begin() + f);
  }

  return checksum(filesystem);
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");

  ifstream fs(argv[1]);
  string input;
  getline(fs, input);

  int file_id = 0;
  vector<int> filesystem{};
  vector<Block> full{}, empty{};

  for (int i = 0; i < input.size(); i++) {
    auto value = input[i] - '0';
    for (auto j = 0; j < value; j++) {
      if (i % 2)
        filesystem.push_back(-1);
      else {
        filesystem.push_back(file_id);
      }
    }

    int block_start = (int)filesystem.size() - value;
    if (i % 2) {
      empty.push_back({.space = value, .start = block_start});
    } else {
      full.push_back(
          {.file_id = file_id, .space = value, .start = block_start});
      file_id++;
    }
  }

  cout << "Part 1: " << part1(filesystem) << '\n';
  cout << "Part 2: " << part2(filesystem, full, empty) << '\n';
  return 0;
}

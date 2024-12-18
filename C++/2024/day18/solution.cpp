#include "utils.h"
#include <algorithm>
#include <cassert>
#include <queue>
using namespace std;
using namespace aoc_utils;

#define ll long long
#define arr array

vector<arr<int, 2>> read_input(const string &filename) {
  vector<arr<int, 2>> bytes;
  ifstream fs(filename);

  string line;
  while (getline(fs, line)) {
    auto point = nums<int>(line);
    bytes.push_back({point[1], point[0]});
  }
  return bytes;
}

bool cmp(const arr<int, 3> &lhs, const arr<int, 3> rhs) {
  // Compare on distance
  return lhs[2] < rhs[2];
}

int part1(const vector<arr<int, 2>> &bytes, int limit, int n) {
  int result = 0;

  vector<int> distances(n * n);
  fill(distances.begin(), distances.end(), INT_MAX);
  distances[0] = 0;

  for (int i = 0; i <= limit; i++)
    distances[bytes[i][0] * n + bytes[i][1]] = -1;

  auto cmp = [](const arr<int, 3> &lhs, const arr<int, 3> rhs) -> bool {
    // Compare on distance
    return lhs[2] < rhs[2];
  };

  vector<arr<int, 3>> pq{{0, 0, 0}};

  while (!pq.empty()) {
    pop_heap(pq.begin(), pq.end(), cmp);
    auto current = pq.back();
    pq.pop_back();

    for (auto [di, dj] : adj4) {
      arr<int, 3> entry = {current[0] + di, current[1] + dj, current[2] + 1};
      bool can_move = is_valid_pos(entry[0], entry[1], n, n) &&
                      distances[entry[0] * n + entry[1]] >= 0;
      if (can_move && distances[entry[0] * n + entry[1]] > entry[2]) {
        distances[entry[0] * n + entry[1]] = entry[2];
        pq.push_back(entry);
        push_heap(pq.begin(), pq.end(), cmp);
      }
    }
  }
  return distances[(n - 1) * n + n - 1];
}

arr<int, 2> part2(const vector<arr<int, 2>> &bytes, int limit, int n) {
  int index = bytes.size() - 1;
  for (; index >= limit; index--) {
    auto path = part1(bytes, index, n);
    if (path != INT_MAX)
      break;
  }
  return bytes[index + 1];
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto bytes = read_input(argv[1]);

  int n = 71, limit = 1024;

  cout << "Part 1:" << part1(bytes, limit, n) << '\n';
  auto blocking_byte = part2(bytes, limit, n);
  cout << "Part 2:" << blocking_byte[1] << ',' << blocking_byte[0] << '\n';
  return 0;
}

#include "utils.h"
#include <cassert>
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

int part1(const vector<arr<int, 2>> &bytes, int limit, int n) {
  int result = 0;

  vector<int> distances(n * n);
  fill(distances.begin(), distances.end(), INT_MAX);
  distances[0] = 0;

  for (int i = 0; i <= limit; i++)
    distances[bytes[i][0] * n + bytes[i][1]] = -1;

  vector<arr<int, 2>> pq{{0, 0}};

  for (int i = 0; i < pq.size(); i++) {
    auto current = pq[i];

    if (current == arr<int, 2>{n - 1, n - 1})
      break;

    for (auto [di, dj] : adj4) {
      arr<int, 2> pos = {current[0] + di, current[1] + dj};

      bool can_move = is_valid_pos(pos[0], pos[1], n, n) &&
                      distances[pos[0] * n + pos[1]] > 0;

      if (can_move && distances[pos[0] * n + pos[1]] >
                          distances[current[0] * n + current[1]] + 1) {
        distances[pos[0] * n + pos[1]] =
            distances[current[0] * n + current[1]] + 1;
        pq.push_back(pos);
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

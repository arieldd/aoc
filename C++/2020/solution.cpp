#include <bits/stdc++.h>
#include <cassert>
using namespace std;

#define ll long long
#define umap unordered_map
#define PREAMBLE 25

vector<int64_t> read_input(const string &filename) {
  vector<int64_t> numbers{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    numbers.push_back(atoll(line.c_str()));
  return numbers;
}

bool is_sum_of_two(const int64_t *numbers, size_t n, int64_t target) {
  umap<int64_t, size_t> hash_map{};
  for (auto i = 0; i < n; i++) {
    auto value = target - numbers[i];
    if (value < 0)
      continue;
    if (hash_map.find(value) != hash_map.end())
      return true;

    hash_map.insert({numbers[i], i});
  }

  return false;
}

int64_t part1(const vector<int64_t> &numbers) {
  auto n = numbers.size();
  for (auto i = PREAMBLE; i < n; i++) {
    if (!is_sum_of_two(numbers.data() + (i - PREAMBLE), PREAMBLE, numbers[i]))
      return numbers[i];
  }
  return 0;
}

int64_t weakness(const vector<int64_t> &numbers, size_t left, size_t right) {
  auto max = numbers[left], min = numbers[left];
  for (auto i = left + 1; i < right; i++) {
    if (numbers[i] > max)
      max = numbers[i];
    if (numbers[i] < min)
      min = numbers[i];
  }
  return min + max;
}

int64_t part2(const vector<int64_t> &numbers, int64_t target) {
  auto n = numbers.size();
  vector<int64_t> prefix_sum{0};
  for (const auto &value : numbers) {
    prefix_sum.push_back(prefix_sum.back() + value);
  }

  for (size_t left = 0, right = 2; right <= n;) {
    auto contiguous_sum = prefix_sum[right] - prefix_sum[left];
    if (contiguous_sum == target)
      return weakness(numbers, left, right);
    if (contiguous_sum > target)
      left++;
    else
      right++;
  }
  return 0;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto numbers = read_input(argv[1]);

  auto invalid_number = part1(numbers);
  cout << "Part 1: " << invalid_number << '\n';
  cout << "Part 2: " << part2(numbers, invalid_number) << '\n';
  return 0;
}

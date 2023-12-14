#include "utils.h"
#include <execution>

using namespace std;

struct Record {
  vector<char> springs;
  vector<int> diagnostic;
  int64_t ways;

  bool operator<(const Record &other) const {
    return springs < other.springs &&
           (!(other.springs < springs) && diagnostic < other.diagnostic);
  }
};

vector<Record> read_springs(const vector<string> &lines) {
  vector<Record> result;

  for (auto &l : lines) {
    Record current;
    auto sides = split(l, ' ');
    auto numbers = split(sides[1], ',');

    for (auto &c : sides[0]) {
      current.springs.push_back(c);
    }

    for (auto &n : numbers)
      current.diagnostic.push_back(stoi(n));

    result.push_back(current);
  }

  return result;
}

void print_record(const Record row) {

  for (auto s : row.springs)
    print(s);
  print(':');
  for (auto &d : row.diagnostic)
    print(d, " ");
  println();
}

void find_ways(const Record &row, int index, int group_size, vector<int> groups,
               int64_t &total) {

  if (index >= row.springs.size()) {
    if (group_size)
      groups.push_back(group_size);

    if (groups == row.diagnostic) {
      total += 1;
    }
    return;
  }

  if (!groups.empty() && !equal(groups.begin(), groups.end(),
                                row.diagnostic.begin())) { // Wrong path
    return;
  }

  if (row.springs[index] == '?') {
    auto current = row;

    current.springs[index] = '#';
    find_ways(current, index + 1, group_size + 1, groups, total);

    current.springs[index] = '.';
    if (group_size)
      groups.push_back(group_size);
    find_ways(current, index + 1, 0, groups, total);

  } else if (row.springs[index] == '#') {
    find_ways(row, index + 1, group_size + 1, groups, total);

  } else {

    if (group_size)
      groups.push_back(group_size);

    find_ways(row, index + 1, 0, groups, total);
  }
}

void find_ways_memoise(Record &row, int sindex, int gindex, int group_size,
                       set<Record> cache) {

  print_record(row);
  println(sindex, " ", gindex, " ", group_size);
  row.ways = 0;

  if (auto it = cache.find(row); it != cache.end()) {
    row.ways = it->ways;
    return;
  }

  if (sindex >= row.springs.size()) {
    row.ways = 1;
    cache.insert(row);
    return;
  }

  if (gindex >= row.diagnostic.size() ||
      group_size > row.diagnostic[gindex]) { // Wrong path
    row.ways = 0;
    cache.insert(row);
    return;
  }

  auto next = row;
  if (row.springs[sindex] == '?') {

    next.springs[sindex] = '#';
    find_ways_memoise(next, sindex + 1, gindex, group_size + 1, cache);
    row.ways = next.ways;

    next.springs[0] = '.';
    next.springs =
        vector<char>(row.springs.begin() + sindex + 1, row.springs.end());
    find_ways_memoise(next, 0, gindex + 1, 0, cache);
    row.ways += next.ways;

  } else if (row.springs[sindex] == '#') {
    find_ways_memoise(row, sindex + 1, gindex, group_size + 1, cache);

  } else {
    next.springs =
        vector<char>(row.springs.begin() + sindex + 1, row.springs.end());
    find_ways_memoise(next, 0, gindex + 1, 0, cache);
    row.ways += next.ways;
  }

  cache.insert(row);
  println(row.ways);
  return;
}

void find_ways_for_extended(Record &r, int extensions, char value) {

  auto expanded = r;

  auto s = r.springs;
  auto d = r.diagnostic;

  for (int i = 0; i < extensions; i++) {
    expanded.springs.push_back(value);
    expanded.springs.insert(expanded.springs.end(), s.begin(), s.end());

    expanded.diagnostic.insert(expanded.diagnostic.end(), d.begin(), d.end());
  }

  int64_t ways = 0;
  find_ways_memoise(expanded, 0, 0, 0, {});
}

vector<string> parse_input(const string &file_name) {
  vector<string> ret;

  ifstream fs(file_name);
  string line;
  while (getline(fs, line)) {
    ret.push_back(line);
  }
  return ret;
}

int64_t part1(vector<Record> &rows) {
  int64_t total_ways = 0;
  for (auto &row : rows) {
    find_ways_memoise(row, 0, 0, 0, {});
    total_ways += row.ways;
  }
  return total_ways;
}

int64_t part2(vector<Record> &rows) {
  int64_t total_ways = 0;
  for_each(execution::par, rows.begin(), rows.end(), [](auto &&row) {
    find_ways_for_extended(row, 4, '?');

    println(row.ways);
  });

  for (auto &row : rows)
    total_ways += row.ways;

  return total_ways;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);

  auto rows = read_springs(lines);

  auto r1 = part1(rows);
  println("Part 1: ", r1);

  // auto r2 = part2(rows);
  // println("Part 2: ", r2);

  return 0;
}
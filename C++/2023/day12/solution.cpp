#include "utils.h"
#include <unordered_map>

using namespace std;
using namespace aoc_utils;

struct Record {
  vector<char> springs;
  vector<int> groups;

  bool operator==(const Record &other) const {
    return springs == other.springs && groups == other.groups;
  }
};

template <> struct std::hash<Record> {
  size_t operator()(const Record &r) const {
    string ss;
    for (auto &c : r.springs) {
      ss += c;
    }

    return hash<string>{}(ss);
  }
};

using map_t = unordered_map<Record, int64_t>;

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
      current.groups.push_back(stoi(n));

    result.push_back(current);
  }

  return result;
}

void print_record(const Record row, tuple<int, int, int> mark) {

  for (auto s : row.springs)
    print(s);
  print(" [");
  for (auto &d : row.groups)
    print(d, " ");

  print("] s:", get<0>(mark), " g:", get<1>(mark), " l:", get<2>(mark));

  println();
}

void find_ways(const Record &row, int index, int group_size, vector<int> groups,
               int64_t &total) {

  if (index >= row.springs.size()) {
    if (group_size)
      groups.push_back(group_size);

    if (groups == row.groups) {
      total += 1;
    }
    return;
  }

  if (!groups.empty() && !equal(groups.begin(), groups.end(),
                                row.groups.begin())) { // Wrong path
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

int64_t find_ways_memoise(const Record &row, int sindex, int gindex,
                          int group_size, map_t &cache) {

  if (cache.find(row) != cache.end()) {
    return cache.at(row);
  }

  int64_t result = 0;

  if (row.springs.empty() || sindex >= row.springs.size()) {
    result = 1;

    if (group_size) {
      if (row.groups.empty() || row.groups.size() > 1 ||
          row.groups[gindex] != group_size) {
        result = 0;
      }
    } else if (!row.groups.empty())
      result = 0;

    cache[row] = result;

    return result;
  }

  if (group_size &&
      (gindex >= row.groups.size() || row.groups[gindex] < group_size)) {
    cache[row] = 0;
    return 0;
  }

  if (row.springs[sindex] == '?') {
    auto next = row;

    next.springs[sindex] = '#';
    result += find_ways_memoise(next, sindex, gindex, group_size, cache);

    next.springs[sindex] = '.';
    result += find_ways_memoise(next, sindex, gindex, group_size, cache);

  } else if (row.springs[sindex] == '#') {
    result = find_ways_memoise(row, sindex + 1, gindex, group_size + 1, cache);

  } else {
    if (group_size) {
      if (group_size != row.groups[gindex]) {
        cache[row] = 0;
        return 0;
      }
      gindex++;
    }

    Record next;
    next.springs =
        vector<char>(row.springs.begin() + sindex + 1, row.springs.end());

    next.groups = vector<int>(row.groups.begin() + gindex, row.groups.end());

    result = find_ways_memoise(next, 0, 0, 0, cache);
  }

  cache[row] = result;
  return result;
}

int64_t find_ways_for_extended(Record &r, int extensions, char value) {

  auto expanded = r;

  auto s = r.springs;
  auto d = r.groups;

  for (int i = 0; i < extensions; i++) {
    expanded.springs.push_back(value);
    expanded.springs.insert(expanded.springs.end(), s.begin(), s.end());

    expanded.groups.insert(expanded.groups.end(), d.begin(), d.end());
  }

  map_t cache;
  return find_ways_memoise(expanded, 0, 0, 0, cache);
}

int64_t part1(vector<Record> &rows) {
  vector<int64_t> total_ways;

  for (auto &row : rows) {
    try {
      map_t cache;
      auto ways = find_ways_memoise(row, 0, 0, 0, cache);
      total_ways.push_back(ways);
    } catch (exception e) {
      println(e.what());
    }
  }

  return accumulate(total_ways.begin(), total_ways.end(), (int64_t)0);
}

int64_t part2(vector<Record> &rows) {
  vector<int64_t> total_ways(rows.size());

  for (auto &row : rows) {
    int64_t ways = find_ways_for_extended(row, 4, '?');
    total_ways.push_back(ways);
  }

  return accumulate(total_ways.begin(), total_ways.end(), (int64_t)0);
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);

  auto rows = read_springs(lines);

  auto r1 = part1(rows);
  println("Part 1: ", r1);

  auto r2 = part2(rows);
  println("Part 2: ", r2);

  return 0;
}

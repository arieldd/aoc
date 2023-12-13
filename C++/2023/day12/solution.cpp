#include "utils.h"
#include <execution>

using namespace std;

struct Row {
  vector<char> springs;
  vector<int> diagnostic;
  int64_t ways;
};

vector<Row> read_springs(const vector<string> &lines) {
  vector<Row> result;

  for (auto &l : lines) {
    Row current;
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

void print_row(const Row row) {

  for (auto s : row.springs)
    print(s);
  print(':');
  for (auto &d : row.diagnostic)
    print(d, " ");
  println();
}

void print_rows(const vector<Row> rows) {

  for (auto &r : rows) {
    print_row(r);
  }
}

bool check_row(const Row &row) {
  auto n = row.springs.size();
  auto s_i = 0;

  for (auto &group : row.diagnostic) {
    while (s_i < n && row.springs[s_i] != '#')
      s_i++;

    if (s_i + group - 1 >= n)
      return false;

    vector<char> block(row.springs.begin() + s_i,
                       row.springs.begin() + s_i + group);
    if (!all_of(block.begin(), block.end(),
                [](const char &elem) { return elem == '#'; }))
      return false;

    s_i += group;
    if (s_i < n && row.springs[s_i] != '.')
      return false;
  }

  for (auto j = s_i; j < n; j++)
    if (row.springs[j] == '#')
      return false;

  return true;
}

void find_ways(const Row &row, int index, int group_size, vector<int> groups,
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

int64_t ways_for_extended(const Row &r, int extensions, char value) {

  auto expanded = r;

  auto s = r.springs;
  auto d = r.diagnostic;

  for (int i = 0; i < extensions; i++) {
    expanded.springs.push_back(value);
    expanded.springs.insert(expanded.springs.end(), s.begin(), s.end());

    expanded.diagnostic.insert(expanded.diagnostic.end(), d.begin(), d.end());
  }

  int64_t ways = 0;
  find_ways(expanded, 0, 0, {}, ways);

  return ways;
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

int64_t part1(vector<Row> &rows) {
  int64_t total_ways = 0;
  for (auto &row : rows) {
    int64_t ways = 0;
    find_ways(row, 0, 0, {}, ways);
    row.ways = ways;
    total_ways += row.ways;
  }
  return total_ways;
}

int64_t part2(vector<Row> &rows) {
  int64_t total_ways = 0;
  for_each(execution::par, rows.begin(), rows.end(), [](auto &&row) {
    auto w0 = row.ways, w1 = ways_for_extended(row, 1, '#'),
         w2 = ways_for_extended(row, 2, '#'),
         w3 = ways_for_extended(row, 3, '#'),
         w4 = ways_for_extended(row, 4, '#');

    row.ways = ipow(w0, 5) + 4 * (w1 * ipow(w0, 3)) + 3 * (w2 * w0 * w0) +
               3 * w1 * w1 * w0 + 2 * w3 * w0 + 2 * w2 * w1 + w4;
    println("done");
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

  auto r2 = part2(rows);
  println("Part 2: ", r2);

  return 0;
}
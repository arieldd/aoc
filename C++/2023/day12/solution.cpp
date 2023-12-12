#include "utils.h"

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

int64_t ways_with_break(const Row &r, bool front) {

  auto expanded = r;

  if (front) {
    expanded.springs.insert(expanded.springs.begin(), '#');
  } else {
    expanded.springs.push_back('#');
  }

  int64_t ways = 0;
  find_ways(expanded, 0, 0, {}, ways);
  return ways;
}

int64_t ways_for_breaks(const Row &r, int breaks) {

  auto expanded = r;

  auto s = r.springs;
  auto d = r.diagnostic;

  for (int i = 0; i < breaks; i++) {
    expanded.springs.push_back('#');
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
  for (auto &row : rows) {

    auto w0 = row.ways,                   //
        wf = ways_with_break(row, true),  //
        wb = ways_with_break(row, false), //
        p2 = w0 * w0, p3 = p2 * w0, p4 = p3 * w0, p5 = p4 * w0;

    auto new_ways = p5;

    int64_t f_ways = 4 * p4 * wf + 6 * ipow(wf, 2) * p3 + 4 * p2 * ipow(wf, 3) +
                     w0 * ipow(wf, 4);

    int64_t b_ways = b_ways = 4 * p4 * wb + 6 * ipow(wb, 2) * p3 +
                              4 * p2 * ipow(wb, 3) + w0 * ipow(wb, 4);

    int64_t c_ways =
        6 * wb * wf * p3 + 4 * p2 * (wb * wb * wf + wf * wf * wb) +
        w0 * (ipow(wb, 3) * wf + ipow(wb, 2) * ipow(wf, 2) + wb * ipow(wf, 3));

    new_ways += f_ways + b_ways + c_ways;

    // println(f_ways, " ", b_ways, " : ", new_ways);

    total_ways += new_ways;
  }
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
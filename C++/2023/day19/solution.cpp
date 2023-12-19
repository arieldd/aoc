#include "utils.h"

using namespace std;

map<char, int> part_mapping{{'x', 0}, {'m', 1}, {'a', 2}, {'s', 3}};

constexpr int64_t MAX_VALUE = 4000, MIN_VALUE = 1;

struct Range {
  int64_t lower;
  int64_t upper;

  int64_t length() const { return (upper - lower) + 1; }

  bool operator==(const Range &r) const {
    return lower == r.lower && upper == r.upper;
  }

  bool operator<(const Range &r) const {
    return lower < r.lower || (!(r.lower < lower) && upper < r.upper);
  }

  bool disjoint(const Range r) const {
    return (lower < r.lower && upper < r.upper) ||
           (r.lower < lower && r.upper < upper);
  }

  Range intersect(const Range &r) const {
    return {max(lower, r.lower), min(upper, r.upper)};
  }

  Range union_range(const Range &r) const {
    return {min(lower, r.lower), max(upper, r.upper)};
  }

  Range diff(const Range &r) const {
    if (lower == r.lower)
      return {r.upper + 1, upper};
    return {lower, r.lower - 1};
  }
};

constexpr Range MAX_RANGE{MIN_VALUE, MAX_VALUE};

struct Node {
  int pos;
  vector<Range> values;
  Node *left, *right;
};

struct Part {
  vector<int64_t> values; // {x m a s}

  int64_t rating() const {
    return accumulate(values.begin(), values.end(), (int64_t)0);
  }
};

struct Rule {
  int pos;
  int value;
  char op;
  string dest;

  bool accept(const Part &p) const {
    if (!op)
      return true;

    if (op == '>') {
      return p.values[pos] > value;
    }

    return p.values[pos] < value;
  }

  Range accepted() const {
    if (!op)
      return MAX_RANGE;

    if (op == '>') {
      return {value + 1, MAX_VALUE};
    }

    return {MIN_VALUE, value - 1};
  }

  Range rejected() const {
    if (!op)
      return MAX_RANGE;

    if (op == '>') {
      return {1, value};
    }

    return {value, MAX_VALUE};
  }
};

struct Workflow {
  string label;
  vector<Rule> rules;

  string process_part(const Part p) const {
    for (auto r : rules) {
      if (r.accept(p))
        return r.dest;
    }
  }
};

map<string, Workflow> parse_workflows(const vector<string> &lines,
                                      int &mid_line) {

  map<string, Workflow> result;
  for (int i = 0; i < lines.size(); i++) {
    if (lines[i] == "") {
      mid_line = i;
      break;
    }

    auto l = lines[i];
    Workflow current;
    auto sides = split(l, '{');
    current.label = sides[0];
    sides[1].pop_back(); // Remove back }

    auto rules = split(sides[1], ',');
    for (auto r : rules) {
      Rule next{-1, -1, 0};
      auto parts = split(r, ':');

      if (parts.size() == 1) {
        next.dest = parts[0];
      } else {
        next.dest = parts[1];
        next.pos = part_mapping.at(parts[0][0]);
        next.op = parts[0][1];
        next.value = stoi(parts[0].substr(2));
      }
      current.rules.push_back(next);
    }
    result[current.label] = current;
  }

  return result;
}

vector<Part> read_parts(const vector<string> &lines, int mid_line) {
  vector<Part> result;
  for (int i = mid_line + 1; i < lines.size(); i++) {
    auto l = lines[i];
    auto strip = l.substr(1, l.size() - 2);

    Part current;
    auto values = split(strip, ',');
    for (auto v : values) {
      auto sides = split(v, '=');
      current.values.push_back(stoll(sides[1]));
    }

    result.push_back(current);
  }

  return result;
}

int64_t explore_flows(const map<string, Workflow> workflows,
                      const string &label, vector<Range> available,
                      set<string> visited) {
  visited.insert(label);

  auto current = workflows.at(label);

  int64_t total = 0;

  for (auto r : current.rules) {
    if (r.dest == "R") {
      if (r.op) {
        available[r.pos] = r.accepted();
      }
      continue;
    }
    if (visited.find(r.dest) == visited.end()) {
      auto new_ranges = available;

      if (r.op) {
        Range accepted = r.accepted(), rejected = r.rejected();
        available[r.pos] = rejected;

        if (new_ranges[r.pos].disjoint(accepted))
          continue; // Not compatible

        new_ranges[r.pos] = accepted;
      }

      if (r.dest == "A") {

        int64_t mult = 1;
        for (int i = 0; i < 4; i++) {
          auto range = available[i];
          mult *= range.length();
        }
        total += mult;

      } else {
        total += explore_flows(workflows, r.dest, new_ranges, visited);
      }
    }
  }

  return total;
}

void build_tree(const map<string, Workflow> workflows, const string &label,
                int rule_index, vector<Range> remaining,
                vector<vector<Range>> &combinations) {

  auto current = workflows.at(label);

  if (rule_index >= current.rules.size())
    return;

  vector<Range> available = remaining;

  Rule r = current.rules[rule_index];
  if (r.dest == "R") {
    if (r.op) {
      available[r.pos] = r.rejected().intersect(available[r.pos]);
    }
  }

  else if (r.dest == "A") {

    vector<Range> next_combination;
    for (int i = 0; i < 4; i++) {
      auto range = (i == r.pos) ? r.accepted() : available[i];
      print("[", range.lower, ",", range.upper, "] ");
      next_combination.push_back(range);
    }
    combinations.push_back(next_combination);
    if (r.op) {
      available[r.pos] = r.rejected().intersect(available[r.pos]);
    }

    println(label, " accepted ");
  } else {
    auto down = available;

    if (r.op) {
      down[r.pos] = r.accepted();

      available[r.pos] = r.rejected().intersect(available[r.pos]);
    }
    build_tree(workflows, r.dest, 0, down, combinations);
  }

  build_tree(workflows, label, rule_index + 1, available, combinations);
}

int64_t part1(const map<string, Workflow> workflows,
              const vector<Part> &parts) {

  map<string, vector<Part>> results{{"A", {}}, {"R", {}}};

  for (auto p : parts) {
    Workflow current = workflows.at("in");
    string dest;
    for (;;) {
      dest = current.process_part(p);
      if (dest == "A" || dest == "R") {
        break;
      }

      current = workflows.at(dest);
    }

    results[dest].push_back(p);
  }

  auto accepted = results.at("A");

  int64_t total_rating = 0;
  for (auto a : accepted) {
    total_rating += a.rating();
  }
  return total_rating;
}

int64_t part2(const map<string, Workflow> workflows) {

  vector<Range> initial = {MAX_RANGE, MAX_RANGE, MAX_RANGE, MAX_RANGE};
  vector<vector<Range>> combinations;

  int64_t question = explore_flows(workflows, "in", initial, {});

  build_tree(workflows, "in", 0, initial, combinations);

  int64_t total = 0;
  for (auto &c1 : combinations) {
    vector<int> disjoints(4);
    for (auto &c2 : combinations) {
      if (c1 != c2) {
        for (int i = 0; i < 4; i++) {
          if (c1[i].disjoint(c2[i]))
            disjoints[i]++;
        }
      }
    }

    int64_t mult = 1;
    for (int i = 0; i < 4; i++) {
      mult *= c1[i].length();
    }
    total += mult;
  }

  println(question);
  return total;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);

  int mid_line = 0;
  auto workflows = parse_workflows(lines, mid_line);
  auto parts = read_parts(lines, mid_line);

  auto r1 = part1(workflows, parts);
  println("Part 1: ", r1);
  auto r2 = part2(workflows);
  println();

  println("Part 2: ", r2);

  return 0;
}
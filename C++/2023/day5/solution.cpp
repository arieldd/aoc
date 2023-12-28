#include "utils.h"

using namespace std;
using namespace aoc_utils;

using number = uint64_t;

struct Range {
  number dest;
  number source;
  number length;

  number end() const { return source + length - 1; }

  vector<Range> split_and_transform(const Range &other) const {
    vector<Range> result;
    if (source < other.source) {
      Range first = {0, source, other.source - source};

      Range main = {0, other.source, min(other.length, length - first.length)};
      main.transform(other);

      result.push_back(main);
      result.push_back(first);

      auto used = first.length + main.length;
      if (used < length) {
        Range third = {0, other.source + other.length, length - used};
        result.push_back(third);
      }
    } else {
      Range main = {0, source, min(length, other.end() - source + 1)};
      main.transform(other);
      result.push_back(main);

      if (main.length < length) {
        Range second = {0, other.end() + 1, length - main.length};
        result.push_back(second);
      };
    }
    return result;
  }

  bool contains(number src) const {
    return src >= source && src <= source + length;
  }

  bool disjoint(const Range &other) const {
    auto x1 = source, x2 = other.source, y1 = source + length - 1,
         y2 = other.source + other.length - 1;
    return (x1 < x2 && y1 < x2) || (x2 < x1 && y2 < x1);
  }

  void transform(const Range &other) { source += other.dest - other.source; }

  number map_to_dest(number src) const {
    auto delta = src - source;
    return dest + delta;
  }

  bool operator<(const Range &other) const {
    return (source < other.source) ||
           ((!(other.source < source)) && (length < other.length));
  }
};

struct Almanac {
  vector<number> seeds;

  vector<vector<Range>> maps;

  vector<Range> get_seed_ranges() const {
    vector<Range> ret;

    for (int i = 0; i < seeds.size() - 1; i += 2) {
      ret.push_back({0, seeds[i], seeds[i + 1]});
    }

    return ret;
  }
};

Almanac read_almanac(const vector<string> &lines) {
  Almanac result;

  auto seed_list = split(split(lines[0], ':')[1], ' ');
  for (auto &s : seed_list) {
    result.seeds.push_back(stoull(s));
  }

  vector<Range> map;
  // read maps:
  for (int i = 3; i < lines.size(); i++) {
    if (lines[i] == "") {
      result.maps.push_back(map);

      map.clear();
      i += 1;
      continue;
    }

    auto values = split(lines[i], ' ');
    map.push_back({stoull(values[0]), stoull(values[1]), stoull(values[2])});
  }
  result.maps.push_back(map);

  return result;
}

void print_almanac(const Almanac &instr) {
  for (auto &s : instr.seeds) {
    print(s, " ");
  }
  println();
  println();

  for (auto &m : instr.maps) {
    for (auto &r : m) {
      print(r.dest, " ", r.source, " ", r.length);
      println();
    }
    println();
  }
}

number part1(const Almanac &instr) {
  vector<number> values = instr.seeds;

  for (auto &map : instr.maps) {
    for (auto i = 0; i < values.size(); i++) {
      auto value = values[i];
      for (auto &range : map) {
        if (range.contains(value)) {
          values[i] = range.map_to_dest(value);
          break;
        }
      }
    }
  }

  return *min_element(values.begin(), values.end());
}

number part2(const Almanac &instr) {
  vector<Range> src_ranges = instr.get_seed_ranges();

  int index = 0;
  for (auto &map : instr.maps) {
    vector<Range> new_ranges;
    while (src_ranges.size()) {
      auto src = src_ranges.back();
      src_ranges.pop_back();

      bool match = false;

      for (auto &entry : map) {
        if (src.disjoint(entry))
          continue;

        auto split = src.split_and_transform(entry);
        new_ranges.push_back(split[0]);

        for (auto i = 1; i < split.size(); i++) {
          src_ranges.push_back(split[i]);
        }

        match = true;
      }
      if (!match)
        new_ranges.push_back(src);
    }

    src_ranges = new_ranges;
  }

  number min = src_ranges[0].source;
  for (auto &src : src_ranges) {
    if (min > src.source)
      min = src.source;
  }

  return min;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto instr = read_almanac(lines);

  // print_almanac(instr);

  auto r1 = part1(instr);
  println("Part 1: ", r1);

  auto r2 = part2(instr);
  println("Part 2: ", r2);

  return 0;
}
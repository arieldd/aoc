#include "utils.h"

using namespace std;

struct Race {
  uint64_t duration;
  uint64_t record;
};

uint64_t beat_race(const Race &r) {
  uint64_t ways_to_win = 0;

  for (auto holding = 1; holding < r.duration; holding++) {
    auto speed = holding;
    auto distance = speed * (r.duration - holding);

    if (distance > r.record)
      ways_to_win++;
  }

  return ways_to_win;
}

uint64_t beat_race_elegantly(const Race &r) {
  double b = r.duration, c = r.record;
  auto D = sqrt(b * b - 4 * c);

  auto s1 = (-b + D) / 2, //
      s2 = (-b - D) / 2;

  return s1 - s2 + 1;
}

vector<Race> read_races(const vector<string> &lines) {
  vector<Race> result;

  auto times = split(split(lines[0], ':')[1], ' ');
  auto records = split(split(lines[1], ':')[1], ' ');

  for (int i = 0; i < times.size(); i++)
    result.push_back({stoull(times[i]), stoull(records[i])});

  return result;
}

Race read_big_race(const vector<string> &lines) {

  auto times = split(split(lines[0], ':')[1], ' ');
  auto records = split(split(lines[1], ':')[1], ' ');

  uint64_t time = 0, record = 0;
  for (int i = 0; i < times.size(); i++) {
    for (auto &c : times[i]) {
      time *= 10;
      time += c - '0';
    }
    for (auto &c : records[i]) {
      record *= 10;
      record += c - '0';
    }
  };

  return Race{time, record};
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

int part1(const vector<Race> &races) {
  int total = 1;
  for (auto &r : races) {
    auto ways = beat_race(r);
    total *= ways;
  }
  return total;
}

int part2(const Race &big_race) { return beat_race_elegantly(big_race); }

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto races = read_races(lines);
  auto big_race = read_big_race(lines);

  auto r1 = part1(races);
  println("Part 1: ", r1);

  auto r2 = part2(big_race);
  println("Part 2: ", r2);

  return 0;
}
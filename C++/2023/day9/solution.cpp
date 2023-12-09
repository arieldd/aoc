#include "utils.h"

using namespace std;

using readings_t = vector<vector<int64_t>>;

readings_t read_sequences(const vector<string> &lines) {
  readings_t result;
  for (auto &l : lines) {
    auto seq_str = split(l, ' ');
    vector<int64_t> seq;
    for (auto &entry : seq_str) {
      seq.push_back(stoll(entry));
    }
    result.push_back(seq);
  }
  return result;
}

int64_t predict_next(const vector<int64_t> &seq, bool front = false) {
  vector<int64_t> current = seq,         //
      front_entries = {current.front()}, //
      back_entries = {current.back()};

  for (;;) {
    vector<int64_t> next;

    for (int i = 0; i < current.size() - 1; i++) {
      next.push_back(current[i + 1] - current[i]);
    }

    if (all_of(next.begin(), next.end(), [](int val) { return val == 0; })) {
      if (front) {
        return accumulate(front_entries.rbegin(), front_entries.rend(), 0,
                          [](int sum, int elem) { return elem - sum; });
      }

      return accumulate(back_entries.begin(), back_entries.end(), 0);
    }

    front_entries.push_back(next.front());
    back_entries.push_back(next.back());
    current = next;
  }

  return 0;
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

int part1(const readings_t &seqs) {
  int total = 0;

  for (auto &seq : seqs) {

    total += predict_next(seq);
  }
  return total;
}

int part2(const readings_t &seqs) {
  int total = 0;

  for (auto &seq : seqs) {

    total += predict_next(seq, true);
  }
  return total;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto seqs = read_sequences(lines);

  auto r1 = part1(seqs);
  println("Part 1: ", r1);

  auto r2 = part2(seqs);
  println("Part 2: ", r2);

  return 0;
}
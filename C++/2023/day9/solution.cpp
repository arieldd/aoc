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

tuple<int64_t, int64_t> seq_predictions(const vector<int64_t> &seq) {
  vector<int64_t> current = seq,         //
      front_entries = {current.front()}, //
      back_entries = {current.back()};

  for (;;) {
    vector<int64_t> next;

    for (int i = 0; i < current.size() - 1; i++) {
      next.push_back(current[i + 1] - current[i]);
    }

    if (all_of(next.begin(), next.end(), [](int val) { return val == 0; })) {
      int64_t front =
          accumulate(front_entries.rbegin(), front_entries.rend(), 0,
                     [](int sum, int elem) { return elem - sum; });
      int64_t back = accumulate(back_entries.begin(), back_entries.end(), 0);

      return {front, back};
    }

    front_entries.push_back(next.front());
    back_entries.push_back(next.back());
    current = next;
  }
}

tuple<int64_t, int64_t> predict_history(const readings_t &seqs) {
  int64_t front_sum = 0, back_sum = 0;

  for (auto &seq : seqs) {
    auto predictions = seq_predictions(seq);
    front_sum += get<0>(predictions);
    back_sum += get<1>(predictions);
  }

  return {front_sum, back_sum};
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto seqs = read_sequences(lines);

  auto results = predict_history(seqs);

  auto r1 = get<1>(results);
  println("Part 1: ", r1);

  auto r2 = get<0>(results);
  println("Part 2: ", r2);

  return 0;
}
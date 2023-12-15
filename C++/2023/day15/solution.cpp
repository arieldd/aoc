#include "utils.h"

using namespace std;

struct Lens {
  string label;
  int focal_length;

  Lens(const string &label, int focal_length) {
    this->label = label;

    this->focal_length = focal_length;
  }

  Lens(const string &label) : Lens(label, 0) {}

  bool operator()(const Lens &other) const { return label == other.label; }

  bool operator==(const Lens &other) const { return label == other.label; }
};

vector<string> read_sequence(const vector<string> &lines) {

  vector<string> result;
  for (auto &l : lines) {
    auto entries = split(l, ',');
    for (auto &str : entries)
      result.push_back(str);
  }

  return result;
}

int64_t hash_step(const string &step) {

  int64_t current_value = 0;
  for (auto &c : step) {
    current_value += c;
    current_value *= 17;
    current_value %= 256;
  }

  return current_value;
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

int part1(const vector<string> &seq) {
  int total = 0;

  for (auto &str : seq) {
    total += hash_step(str);
  }
  return total;
}

int part2(const vector<string> &seq) {
  map<int, vector<Lens>> boxes;

  for (int i = 0; i < 256; i++) {
    boxes[i] = {};
  }

  for (auto &str : seq) {
    string label;
    if (str.ends_with('-')) {
      label = str.substr(0, str.size() - 1);

      auto box_number = hash_step(label);

      if (auto it = find_if(
              boxes[box_number].begin(), boxes[box_number].end(),
              [label](const Lens &lens) { return lens.label == label; });
          it != boxes[box_number].end()) {
        boxes[box_number].erase(it);
      }

    } else {
      label = str.substr(0, str.size() - 2);
      int focal_length = str.back() - '0';

      Lens current(label, focal_length);

      auto box_number = hash_step(label);

      if (auto it = find_if(
              boxes[box_number].begin(), boxes[box_number].end(),
              [label](const Lens &lens) { return lens.label == label; });
          it != boxes[box_number].end()) {
        *it = current;
      } else {
        boxes[box_number].push_back(current);
      }
    }
  }

  auto total_power = 0;
  for (int i = 0; i < 256; i++) {
    auto box = boxes[i];
    for (int j = 0; j < box.size(); j++) {
      auto lens_power = (i + 1) * (j + 1) * box[j].focal_length;
      total_power += lens_power;
    }
  }

  return total_power;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto seq = read_sequence(lines);

  auto r1 = part1(seq);
  println("Part 1: ", r1);

  auto r2 = part2(seq);
  println("Part 2: ", r2);

  return 0;
}
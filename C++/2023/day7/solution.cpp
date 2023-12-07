#include "utils.h"

using namespace std;

struct Hand {
  vector<int> cards;
  int bid;

  int strength = 0;

  Hand(vector<int> cards, int bid) {
    this->cards = cards;
    this->bid = bid;
    _find_strength_with_joker();
  }

  void _find_strength_with_joker() {

    map<int, int> suits;
    for (auto &c : cards) {
      suits[c]++;
    }

    if (suits.find(1) != suits.end()) {
      auto max = 0, most_repeated = 0;
      for (auto &pair : suits)
        if (pair.first != 1 && pair.second > max) {
          max = pair.second;
          most_repeated = pair.first;
        }

      suits[most_repeated] += suits[1];
    }

    for (auto &pair : suits) {
      if (pair.first == 1)
        continue;
      auto temp = ipow(5, pair.second);
      strength += temp;
    }
  }

  bool operator<(const Hand &other) const {
    if (strength > other.strength)
      return 0;
    if (strength < other.strength)
      return 1;

    for (int i = 0; i < 5; i++) {
      if (cards[i] > other.cards[i])
        return 0;
      if (cards[i] < other.cards[i])
        return 1;
    }

    return 0;
  }
};

vector<Hand> read_hands(const vector<string> &lines, bool joker = false) {
  vector<Hand> result;

  for (auto &line : lines) {
    auto hand_str = split(line, ' ');
    vector<int> cards;
    for (auto &c : hand_str[0]) {
      if (isdigit(c))
        cards.push_back(c - '0');
      else {
        if (c == 'A') {
          cards.push_back(14);
        } else if (c == 'K') {
          cards.push_back(13);
        } else if (c == 'Q') {
          cards.push_back(12);
        } else if (c == 'J') {
          cards.push_back((joker) ? 1 : 11);
        } else if (c == 'T') {
          cards.push_back(10);
        } else {
          println("Something missing");
        }
      }
    }

    result.push_back(Hand(cards, stoi(hand_str[1])));
  }

  return result;
}

void print_hands(const vector<Hand> &hands) {
  for (auto &h : hands) {
    for (auto &c : h.cards) {
      print(c, " ");
    }
    println(", ", h.strength, " : ", h.bid);
  }
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

int part1(const vector<Hand> &hands) {
  auto sorted = hands;
  sort(sorted.begin(), sorted.end());

  auto total = 0;
  for (auto i = 0; i < sorted.size(); i++) {
    total += (i + 1) * sorted[i].bid;
  }

  return total;
}

int part2(const vector<Hand> &hands) { return part1(hands); }

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto hands = read_hands(lines);

  auto r1 = part1(hands);
  println("Part 1: ", r1);

  hands = read_hands(lines, true);

  auto r2 = part2(hands);
  println("Part 2: ", r2);

  return 0;
}
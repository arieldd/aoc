#include "utils.h"

using namespace std;

struct Card {
  int id;
  set<int> winning;
  set<int> yours;
};

vector<Card> parse_cards(const vector<string> &lines) {
  vector<Card> result;

  for (auto &line : lines) {
    Card current;
    auto sides = split(line, ':');
    auto id_side = sides[0];
    auto number_side = sides[1];

    current.id = stoi(split(id_side, ' ')[1]);

    auto numbers = split(number_side, '|');

    auto winning = split(numbers[0], ' ');
    auto yours = split(numbers[1], ' ');

    for (auto &w_str : winning) {
      current.winning.insert(stoi(w_str));
    }
    for (auto &y_str : yours) {
      current.yours.insert(stoi(y_str));
    }

    result.push_back(current);
  }

  return result;
}

vector<Card> parse_input(const string &file_name) {
  vector<string> ret;

  ifstream fs(file_name);
  string line;
  while (getline(fs, line)) {
    ret.push_back(line);
  }
  return parse_cards(ret);
}

int part1(const vector<Card> &cards) {
  int total_points = 0;
  for (auto &card : cards) {
    int point_value = 0;
    for (auto number : card.yours) {
      if (card.winning.find(number) != card.winning.end()) {
        point_value = (point_value) ? point_value * 2 : 1;
      }
    }
    total_points += point_value;
  }
  return total_points;
}

int part2(const vector<Card> &cards) {

  map<int, int> total_cards;

  for (auto &card : cards) {
    total_cards[card.id]++;
    int cards_won = 0;
    for (auto number : card.yours) {
      if (card.winning.find(number) != card.winning.end()) {
        cards_won++;
      }
    }

    for (auto i = 1; i <= cards_won; i++) {
      total_cards[card.id + i] += total_cards[card.id];
    }
  }

  return accumulate(total_cards.begin(), total_cards.end(), 0,
                    [](auto sum, auto &pair) { return sum + pair.second; });
}

int main(int argc, char *argv[]) {
  auto cards = parse_input(argv[1]);

  auto r1 = part1(cards);
  println("Part 1: ", r1);
  auto r2 = part2(cards);
  println("Part 2: ", r2);
  return 0;
}
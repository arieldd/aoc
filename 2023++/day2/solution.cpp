#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

#include "..\utils.h"

using namespace std;

struct Set {
  int blue;
  int green;
  int red;
};

struct Game {
  int id;
  vector<Set> plays;
};

vector<Set> parse_sets(const string &plays) {
  vector<Set> sets;

  auto rounds = split(plays, ';');

  for (auto &round : rounds) {
    Set current{};
    auto cubes = split(round, ',');

    for (auto &cube : cubes) {
      auto parts = split(cube, ' ');
      auto amount = stoi(parts[0]);
      auto color = parts[1];

      if (color == "green") {
        current.green = amount;
      } else if (color == "blue") {
        current.blue = amount;
      } else {
        current.red = amount;
      }
    }

    sets.push_back(current);
  }

  return sets;
}

vector<Game> parse_games(const vector<string> game_lines) {
  vector<Game> result;

  for (auto &game_line : game_lines) {
    Game current{};
    auto parts = split(game_line, ':');
    auto id_side = parts[0];
    auto play_side = parts[1];

    current.id = stoi(split(id_side, ' ')[1]);
    current.plays = parse_sets(play_side);
    result.push_back(current);
  }

  return result;
}

vector<Game> parse_input(const string &file_name) {
  vector<string> ret;

  ifstream fs(file_name);
  string line;
  while (getline(fs, line)) {
    ret.push_back(line);
  }

  return parse_games(ret);
}

void print_games(const vector<Game> &games) {
  for (auto &game : games) {
    print("id: ", game.id, " ");
    for (auto &set : game.plays) {
      print("(", set.blue, set.green, set.red, ");");
    }
    println();
  }
}

int part1(const vector<Game> &games) {
  Set limits{14, 13, 12};

  vector<int> possible_games;

  for (auto &game : games) {
    bool possible = true;
    for (auto &set : game.plays) {
      if (set.blue > limits.blue || set.green > limits.green ||
          set.red > limits.red) {
        possible = false;
        break;
      }
    }
    if (possible)
      possible_games.push_back(game.id);
  }

  return accumulate(possible_games.begin(), possible_games.end(), 0);
}

int part2(const vector<Game> &games) {

  vector<int> set_powers;

  for (auto &game : games) {
    Set minimum{0, 0, 0};

    for (auto &set : game.plays) {
      if (set.blue > minimum.blue) {
        minimum.blue = set.blue;
      }
      if (set.green > minimum.green) {
        minimum.green = set.green;
      }
      if (set.red > minimum.red) {
        minimum.red = set.red;
      }
    }
    set_powers.push_back(minimum.blue * minimum.green * minimum.red);
  }

  return accumulate(set_powers.begin(), set_powers.end(), 0);
}

int main() {
  auto games = parse_input("input.txt");
  // auto games = parse_input("test.txt");
  auto r1 = part1(games);
  println("Part 1: ", r1);
  auto r2 = part2(games);
  println("Part 2: ", r2);
  return 0;
}
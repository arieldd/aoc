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

  int semicolon, comma, space;
  string play_str = plays;
  while (play_str != "") {
    Set current{};

    semicolon = play_str.find(';');
    auto round = play_str.substr(0, semicolon);

    while (round != "") {
      comma = round.find(',');

      auto next_cube = round.substr(0, comma);

      space = next_cube.find(' ');
      auto color = next_cube.substr(space + 1);
      auto amount = stoi(next_cube.substr(0, space));

      if (color == "green") {
        current.green = amount;
      } else if (color == "blue") {
        current.blue = amount;
      } else {
        current.red = amount;
      }

      if (comma == string::npos)
        break;

      round = round.substr(comma + 2, string::npos);
    }

    sets.push_back(current);

    if (semicolon == string::npos)
      break;

    play_str = play_str.substr(semicolon + 2, string::npos);
  }

  return sets;
}

vector<Game> parse_games(const vector<string> game_lines) {
  vector<Game> result;

  for (auto &game_line : game_lines) {
    Game current{};
    auto split = game_line.find(':');
    auto id_side = game_line.substr(0, split);
    auto play_side = game_line.substr(split + 2, string::npos);

    split = id_side.find(' ');
    current.id = stoi(id_side.substr(split + 1));
    current.plays = parse_sets(play_side);
    result.push_back(current);
  }

  return result;
}

vector<Game> parse_input(const string &file_name) {
  vector<std::string> ret;

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
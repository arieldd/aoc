#include "utils.h"

using namespace std;

struct Number {
  int value;
  bool marked;
};

struct Board {
  vector<vector<Number>> numbers;

  int score(int value) {
    int sum = 0;

    for (auto &row : numbers) {
      for (auto &entry : row) {
        if (!entry.marked)
          sum += entry.value;
      }
    }
    return sum * value;
  }

  bool mark(int value) {

    int i = 0;
    for (auto &row : numbers) {
      int j = 0;
      for (auto &entry : row) {
        if (entry.value == value) {
          entry.marked = true;
          return bingo(i, j);
        }
        j++;
      }
      i++;
    }

    return false;
  }

  bool bingo(int i, int j) {
    bool bingo = true;
    for (int k = 0; k < numbers.size(); i++) {
      if (!numbers[k][j].marked) {
        bingo = false;
        break;
      }
    }
    if (bingo)
      return bingo;

    bingo = true;

    for (int k = 0; k < numbers.size(); i++) {
      if (!numbers[i][k].marked) {
        bingo = false;
        break;
      }
    }
    return bingo;
  }
};

struct Game {
  vector<int> called;
  vector<Board> bingos;
};

Game parse_game(const vector<string> &lines) {
  Game result;

  auto values = split(lines[0], ',');

  for (auto &v : values) {
    result.called.push_back(stoi(v));
  }

  Board current;
  for (auto i = 2; i < lines.size(); i++) {
    auto l = lines[i];

    if (l.empty()) {
      result.bingos.push_back(current);
      current = {};
      continue;
    }

    auto entries = split(l, ' ');

    vector<Number> new_row;
    for (int j = 0; j < entries.size(); j++) {
      new_row.push_back({stoi(entries[j]), false});
    }
    current.numbers.push_back(new_row);
  }
  result.bingos.push_back(current);

  return result;
}

void print_game(const Game &game) {
  for (auto &n : game.called) {
    print(n, " ");
  }
  println();
  println();

  for (auto &board : game.bingos) {
    for (auto &row : board.numbers) {
      for (auto &entry : row) {
        print(entry.value, " ");
      }
      println();
    }
    println();
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

int part1(const Game &game) {

  for (auto n : game.called) {
    for (auto board : game.bingos) {
      if (board.mark(n)) {
        return board.score(n);
      }
    }
  }

  return 0;
}

int part2(const Game &game) { return 0; }

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto game = parse_game(lines);

  // print_game(game);

  auto r1 = part1(game);
  println("Part 1: ", r1);

  auto r2 = part2(game);
  println("Part 2: ", r2);

  return 0;
}
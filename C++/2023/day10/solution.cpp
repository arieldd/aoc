#include "utils.h"

using namespace std;

struct Node {
  int r;
  int c;
  char symbol;

  string to_str() const {
    return '(' + to_string(r) + ',' + to_string(c) + ") " + symbol;
  }
};

const bool operator<(const Node &lhs, const Node &rhs) {
  return (lhs.r < rhs.r) || (!(rhs.r < lhs.r) && lhs.c < rhs.c);
}

struct Graph {
  vector<Node> nodes;
  map<Node, set<Node>> pipes;
  Node start;
};

Graph read_graph(const vector<string> &lines) {
  Graph result;

  auto n = lines.size();
  for (int i = 0; i < n; i++) {
    for (int j = 0; j < n; j++) {
      auto symbol = lines[i][j];

      Node current{i, j, symbol};
      set<Node> neighbours;

      if (symbol == '.')
        continue;

      else if (symbol == '|') {
        if (is_valid_pos(i - 1, j, n, n)) {
          neighbours.insert({i - 1, j, lines[i - 1][j]});
        }
        if (is_valid_pos(i + 1, j, n, n)) {
          neighbours.insert({i + 1, j, lines[i + 1][j]});
        }

      } else if (symbol == '-') {
        if (is_valid_pos(i, j - 1, n, n)) {
          neighbours.insert({i, j - 1, lines[i][j - 1]});
        }
        if (is_valid_pos(i, j + 1, n, n)) {
          neighbours.insert({i, j + 1, lines[i][j + 1]});
        }

      } else if (symbol == 'L') {
        if (is_valid_pos(i - 1, j, n, n)) {
          neighbours.insert({i - 1, j, lines[i - 1][j]});
        }
        if (is_valid_pos(i, j + 1, n, n)) {
          neighbours.insert({i, j + 1, lines[i][j + 1]});
        }

      } else if (symbol == 'J') {
        if (is_valid_pos(i - 1, j, n, n)) {
          neighbours.insert({i - 1, j, lines[i - 1][j]});
        }
        if (is_valid_pos(i, j - 1, n, n)) {
          neighbours.insert({i, j - 1, lines[i][j - 1]});
        }

      } else if (symbol == '7') {
        if (is_valid_pos(i, j - 1, n, n)) {
          neighbours.insert({i, j - 1, lines[i][j - 1]});
        }
        if (is_valid_pos(i + 1, j, n, n)) {
          neighbours.insert({i + 1, j, lines[i + 1][j]});
        }

      } else if (symbol == 'F') {
        if (is_valid_pos(i, j + 1, n, n)) {
          neighbours.insert({i, j + 1, lines[i][j + 1]});
        }
        if (is_valid_pos(i + 1, j, n, n)) {
          neighbours.insert({i + 1, j, lines[i + 1][j]});
        }

      } else { // S
        result.start = current;
      }

      result.nodes.push_back(current);
      result.pipes[current] = neighbours;
    }
  }

  for (auto &pair : result.pipes) {
    if (pair.second.find(result.start) != pair.second.end()) {
      result.pipes.at(result.start).insert(pair.first);
    }
  }

  return result;
}

void print_graph(const Graph &graph) {

  println("Start: ", graph.start.to_str());

  for (auto &node : graph.pipes) {
    print(node.first.to_str());
    for (auto &adj : node.second) {
      print(" ", adj.to_str());
    }
    println();
  }
}

void print_loop(const vector<Node> &loop, int n) {
  vector<vector<char>> map(n);

  for (auto i = 0; i < n; i++) {
    auto row = vector<char>(n);
    for (int j = 0; j < n; j++)
      row[j] = '.';
    map[i] = row;
  }

  for (auto node : loop) {
    map[node.r][node.c] = node.symbol;
  }

  for (auto &row : map) {
    for (auto &elem : row)
      print(elem, " ");
    println();
  }
}

vector<vector<Node>> clean_junk(int n, const vector<Node> &loop) {
  vector<vector<Node>> result;

  for (auto i = 0; i < n; i++) {
    vector<Node> row;
    for (int j = 0; j < n; j++)
      row.push_back({i, j, '.'});
    result.push_back(row);
  }

  for (auto &node : loop) {
    result[node.r][node.c].symbol = node.symbol;
  }

  return result;
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

int part1(Graph &graph, vector<Node> &loop) {

  map<Node, vector<int>> visited;
  for (auto &node : graph.nodes) {
    visited[node] = {0, INT_MAX};
  }

  visited[graph.start][1] = 0;

  deque<Node> queue{graph.start};

  while (!queue.empty()) {
    auto current = queue.front();
    queue.pop_front();

    visited.at(current)[0] = 1; // MArk as visited

    auto neighbours = graph.pipes.at(current);

    for (auto &adj : neighbours) {
      auto prev_distance = visited.at(current)[1];
      auto distance = visited.at(adj)[1];

      visited.at(adj)[1] = min(distance, prev_distance + 1);
      if (visited.at(adj)[0] == 0) {
        queue.push_back(adj);
      }
    }
  }

  int max = 0;
  for (auto &pair : visited) {
    auto visited = pair.second[0];
    auto distance = pair.second[1];
    if (visited) {
      loop.push_back(pair.first);
      if (distance > max)
        max = distance;
    }
  }
  return max;
}

int part2(vector<vector<Node>> &pipes) {

  int total_in = 0;

  map<char, char> closing{{'|', '|'}, {'F', '7'}, {'L', 'J'}, {'S', 'J'}};

  for (auto &row : pipes) {
    int up = 0, down = 0;

    for (auto &elem : row) {
      if (elem.symbol == '-')
        continue;

      if (elem.symbol == '|') {
        if (up)
          up--;
        else
          up++;

        if (down)
          down--;
        else
          down++;
      }
      if (elem.symbol == 'F' || elem.symbol == '7') {
        if (down)
          down--;
        else
          down++;
      }

      if (elem.symbol == 'L' || elem.symbol == 'J' || elem.symbol == 'S') {
        if (up)
          up--;
        else
          up++;
      }

      if (elem.symbol == '.' && (up || down))
        total_in++;
    }
  }

  return total_in;
}

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto graph = read_graph(lines);

  vector<Node> loop;
  auto r1 = part1(graph, loop);
  println("Part 1: ", r1);

  auto pipes = clean_junk(lines.size(), loop);

  auto r2 = part2(pipes);
  println("Part 2: ", r2);

  return 0;
}
#include "utils.h"
#include <cassert>
using namespace std;
using namespace aoc_utils;

#define ll long long
#define arr array

map<string, set<string>> read_input(const string &filename) {
  map<string, set<string>> graph{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line)) {
    auto nodes = split(line, '-');
    graph[nodes[0]].insert(nodes[1]);
    graph[nodes[1]].insert(nodes[0]);
  }
  return graph;
}

ll part1(const map<string, set<string>> &graph) {
  ll result = 0;
  set<arr<string, 3>> connected;
  for (auto node : graph) {
    auto first = node.first;
    for (auto second : node.second) {
      for (auto third : graph.at(second)) {
        if (graph.at(third).contains(first)) {
          auto group = arr<string, 3>{first, second, third};
          sort(group.begin(), group.end());
          connected.insert(group);
        }
      }
    }
  }

  for (auto group : connected) {
    for (auto key : group)
      if (key.starts_with('t')) {
        result++;
        break;
      }
  }
  return result;
}

set<string> get_connected(const map<string, set<string>> &graph,
                          set<string> connected, string current,
                          set<string> &visited) {

  visited.insert(current);

  auto adj = graph.at(current);
  for (auto key : connected)
    if (!adj.contains(key))
      return connected;

  connected.insert(current);

  set<string> best = connected;
  for (auto next : adj) {
    if (visited.contains(next) or connected.contains(next))
      continue;
    auto possible = get_connected(graph, connected, next, visited);
    if (possible.size() > best.size())
      best = possible;
  }
  return best;
}

string part2(const map<string, set<string>> &graph) {
  set<string> best;
  for (auto node : graph) {
    set<string> visited;
    set<string> connected;
    auto conn = get_connected(graph, connected, node.first, visited);
    if (conn.size() > best.size())
      best = conn;
  }
  string password;
  for (auto key : best)
    password += key + ',';
  password.pop_back();
  return password;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto graph = read_input(argv[1]);
  cout << "Part 1:" << part1(graph) << '\n';
  cout << "Part 2:\n" << part2(graph) << '\n';
  return 0;
}

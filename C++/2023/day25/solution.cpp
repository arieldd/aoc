#include "utils.h"
#include <unordered_map>

using namespace std;

struct Component {
  string label;
  vector<string> edges;
};

template <> struct hash<Component> {
  size_t operator()(const Component &c) const {
    return hash<string>()(c.label);
  }
};

unordered_map<string, Component>
read_component_list(const vector<string> &lines) {
  unordered_map<string, Component> result;

  for (auto &l : lines) {
    Component next;
    auto sides = split(l, ':');
    next.label = sides[0];
    auto connections = split(sides[1], ' ');
    for (auto &c : connections) {
      next.edges.push_back(c);
      result[c].edges.push_back(next.label);
    }
    result[next.label] = next;
  }

  return result;
}

vector<vector<int>>
build_adj_list(const unordered_map<string, Component> &graph) {
  map<string, int> nodes;
  auto n = graph.size();
  vector<vector<int>> adj(n, vector<int>(n));

  int pos = 0;
  for (auto &pair : graph) {
    auto label = pair.first;
    nodes[label] = pos;
    pos++;
  }
  for (auto &pair : graph) {
    auto label = pair.first;
    for (auto &c : pair.second.edges) {
      int i = nodes.at(label), j = nodes.at(c);
      adj[i][j] = 1;
      adj[j][i] = 1;
    }
  }

  return adj;
}

//From https://en.wikipedia.org/wiki/Stoer%E2%80%93Wagner_algorithm#Example_code
pair<int, vector<int>> global_min_cut(vector<vector<int>> adj) {
  pair<int, vector<int>> cut = {INT_MAX, {}};
  
  auto n = adj.size();
  vector<vector<int>> contractions(n);

  for (int i = 0; i < n; i++)
    contractions[i] = {i};

  for (int ph = 1; ph < n; ph++) {
    vector<int> w = adj[0];
    size_t s = 0, t = 0;
    for (int it = 0; it < n - ph;
         it++) { // O(V^2) -> O(E log V) with prio. queue
      w[t] = INT_MIN;
      s = t, t = max_element(w.begin(), w.end()) - w.begin();
      for (int i = 0; i < n; i++)
        w[i] += adj[t][i];
    }
    cut = min(cut, {w[t] - adj[t][t], contractions[t]});
    contractions[s].insert(contractions[s].end(), contractions[t].begin(), contractions[t].end());
    for (int i = 0; i < n; i++)
      adj[s][i] += adj[t][i];
    for (int i = 0; i < n; i++)
      adj[i][s] = adj[s][i];
    adj[0][t] = INT_MIN;
  }

  return cut;
}

int part1(const unordered_map<string, Component> &graph) { return 0; }

int main(int argc, char *argv[]) {
  auto lines = parse_input(argv[1]);
  auto graph = read_component_list(lines);
  auto adj = build_adj_list(graph);

  auto min_cut = global_min_cut(adj);
  auto p1 = min_cut.second.size(), p2 = graph.size() - p1, result = p1 * p2;

  println(p1, ",", p2, " : ", result);

  return 0;
}
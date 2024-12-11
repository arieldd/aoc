#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <unordered_map>
#include <vector>
using namespace std;

#define ll long long
#define vi vector<int>
#define umap unordered_map

struct Range {
  int lower, upper;

  bool contains(int value) const { return lower <= value && value <= upper; }
};

struct Field {
  string name;
  pair<Range, Range> allowed;

  bool is_valid_value(int value) const {
    return allowed.first.contains(value) || allowed.second.contains(value);
  }
};

struct Document {
  vector<Field> fields;
  vi my_ticket;
  vector<vi> nearby_tickets;
};

Document read_document(vector<string> lines) {
  auto n = lines.size();

  Document result{};
  bool reading_tickets = false;
  for (auto i = 0; i < n; i++) {
    auto &line = lines[i];
    if (line.empty()) {
      i++;
      reading_tickets = true;
      continue;
    }

    stringstream ss(line);
    string chunk;
    if (reading_tickets) {
      vi ticket;
      while (getline(ss, chunk, ',')) {
        ticket.push_back(stoi(chunk));
      }

      if (result.my_ticket.empty())
        result.my_ticket = ticket;
      else
        result.nearby_tickets.push_back(ticket);
    } else {
      Field next;
      auto name_end = line.find(": ");
      next.name = line.substr(0, name_end);

      auto range_end = line.find(" or ");
      auto range = line.substr(name_end + 2, range_end);
      auto range_sep = range.find('-');
      next.allowed.first = {
          .lower = stoi(range.substr(0, range_sep)),
          .upper = stoi(range.substr(range_sep + 1, range.size()))};

      range = line.substr(range_end + 4, line.size());
      range_sep = range.find('-');
      next.allowed.second = {
          .lower = stoi(range.substr(0, range_sep)),
          .upper = stoi(range.substr(range_sep + 1, range.size()))};

      result.fields.push_back(next);
    }
  }
  return result;
}

vector<string> read_input(const string &filename) {
  vector<string> lines{};
  ifstream fs(filename);

  string line;
  while (getline(fs, line))
    lines.push_back(line);
  return lines;
}

int part1(Document &doc) {
  vi invalid_values;
  for (int i = 0; i < doc.nearby_tickets.size(); i++) {
    bool valid_ticket = true;
    vi ticket = doc.nearby_tickets[i];
    for (auto value : ticket) {
      bool valid = false;
      for (auto const &field : doc.fields) {
        if (field.is_valid_value(value)) {
          valid = true;
          break;
        }
      }
      if (!valid) {
        invalid_values.push_back(value);
        valid_ticket = false;
      }
    }
    if (!valid_ticket) {
      doc.nearby_tickets.erase(doc.nearby_tickets.begin() + i);
      i--;
    }
  }
  return accumulate(invalid_values.begin(), invalid_values.end(), 0);
}

ll part2(const Document &doc) {
  umap<int, vi> possible;

  int n = doc.nearby_tickets.size(), m = doc.my_ticket.size();
  for (int findex = 0; findex < doc.fields.size(); findex++) {
    auto field = doc.fields[findex];
    possible[findex] = vi{};
    auto &valid_pos = possible.at(findex);
    for (int j = 0; j < m; j++) {
      bool valid = true;
      for (int i = 0; i < n; i++) {
        if (!field.is_valid_value(doc.nearby_tickets[i][j])) {
          valid = false;
          break;
        }
      }
      if (valid)
        valid_pos.push_back(j);
    }
  }

  vector<pair<int, vi>> list(possible.begin(), possible.end());
  sort(list.begin(), list.end(), [](const auto e1, const auto e2) {
    return e1.second.size() < e2.second.size();
  });

  for (int i = 0; i < list.size(); i++) {
    auto entry = list[i];
    assert(entry.second.size() == 1 && "Oops too many valid!");
    auto pos = entry.second[0];
    for (int j = i + 1; j < list.size(); j++) {
      if (auto it = find(list[j].second.begin(), list[j].second.end(), pos);
          it != list[j].second.end()) {
        list[j].second.erase(it);
      }
    }
  }

  possible.clear();
  possible = {list.begin(), list.end()};
  ll result = 1;
  for (int i = 0; i < 6; i++) {
    auto pos = possible.at(i)[0];
    result *= doc.my_ticket[pos];
  }
  return result;
}

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto lines = read_input(argv[1]);
  auto doc = read_document(lines);
  cout << "Part 1:" << part1(doc) << '\n';
  cout << "Part 2:" << part2(doc) << '\n';
  return 0;
}

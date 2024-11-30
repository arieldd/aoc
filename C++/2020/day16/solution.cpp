#include <bits/stdc++.h>
#include <numeric>
using namespace std;

#define ll long long
#define vi vector<int>

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

ostream &operator<<(ostream &os, const Document &arg) {
  for (auto field : arg.fields) {
    os << field.name << ": (" << field.allowed.first.lower << ','
       << field.allowed.first.upper << ")-(" << field.allowed.second.lower
       << ',' << field.allowed.second.upper << ")\n";
  }
  os << "\nMine: ";
  for (auto value : arg.my_ticket) {
    os << value << ',';
  }
  os << '\n';

  os << "\nNearby:\n";
  for (auto &ticket : arg.nearby_tickets) {
    for (auto value : ticket) {
      os << value << ',';
    }
    os << '\n';
  }
  return os;
}

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

int part1(const Document &doc) {
  vector<int> invalid;
  for (auto &ticket : doc.nearby_tickets) {
    for (auto value : ticket) {
      bool valid = false;
      for (auto const &field : doc.fields) {
        if (field.is_valid_value(value)) {
          valid = true;
          break;
        }
      }
      if (!valid)
        invalid.push_back(value);
    }
  }
  return accumulate(invalid.begin(), invalid.end(), 0);
}

int part2(const vector<string> &lines) { return 0; }

int main(int argc, char *argv[]) {
  assert(argc > 1 && "Need some input brotha\n");
  auto lines = read_input(argv[1]);
  auto doc = read_document(lines);
  cout << "Part 1:" << part1(doc) << '\n';
  return 0;
}

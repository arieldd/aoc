#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

struct FsNode {
  string name;
  int size;
  FsNode *parent;

  unordered_map<string, int> children;

  FsNode(const string &name, int size, FsNode *parent)
      : name(name), size(size), parent(parent) {}

  FsNode() {}
};

int calculate_size(FsNode &node, vector<FsNode> &file_system) {
  if (!node.size && node.children.size()) {
    for (const auto &child : node.children) {
      node.size += calculate_size(file_system[child.second], file_system);
    }
  }

  return node.size;
}

void print(FsNode &node, vector<FsNode> &file_system, int depth) {
  auto tabs = "";

  for (int i = 0; i < depth; i++)
    tabs += '\t';

  cout << tabs + node.name << endl;

  for (const auto &child : node.children) {
    print(file_system[child.second], file_system, depth + 1);
  }
}

std::vector<string> parse_input(const std::string &file_name) {
  std::vector<string> ret;

  std::ifstream fs(file_name);
  std::string line;
  while (std::getline(fs, line)) {
    ret.push_back(line);
  }
  return ret;
}

int parse_command(const string &line, string &dir_name) {
  if (line[0] != '$') // not a command
    return 0;

  if (line.find("cd") != 2) // ls command
    return 1;

  dir_name = line.substr(5);
  return 2;
}

int parse_file_data(const string &line, string &dir_name) {
  auto middle = line.find(" ");

  auto half = line.substr(0, middle);
  dir_name = line.substr(middle + 1);

  if (half == "dir") // dir listing
    return 0;

  return stoi(half);
}

void read_fs(const vector<string> &lines, vector<FsNode> &file_system) {
  auto &current = file_system[0];

  string file_name;

  for (const auto &l : lines) {
    file_name = "";
    auto command = parse_command(l, file_name);
    if (command) {
      if (command < 2)
        continue;

      if (file_name == "/") {
        current = file_system[0];
        continue;
      }

      if (file_name == "..") {
        if (current.parent)
          current = *current.parent;
        continue;
      }

      if (!current.children.count(file_name)) {
        file_system.push_back(FsNode(file_name, 0, &current));
        current.children[file_name] = file_system.size() - 1;
      }

      current = file_system[current.children[file_name]];
      continue;
    }

    auto size = parse_file_data(l, file_name);

    if (!current.children.count(file_name)) {
      file_system.push_back(FsNode(file_name, size, &current));
      current.children[file_name] = file_system.size() - 1;
    }
  }
}

int part1(FsNode &root) { return 0; }

int part2(FsNode &root) { return 0; }

int main() {

  // auto data = parse_input("input.txt");
  auto data = parse_input("test.txt");

  vector<FsNode> file_system = {FsNode("/", 0, nullptr)};
  read_fs(data, file_system);

  print(file_system[0], file_system, 0);

  auto r1 = part1(file_system[0]);
  std::cout << "Part 1: " << r1 << std::endl;

  auto r2 = part2(file_system[0]);
  std::cout << "Part 2: " << r2 << std::endl;
  return 0;
}
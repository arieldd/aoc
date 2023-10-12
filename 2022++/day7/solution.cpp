#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <map>
#include <numeric>
#include <string>
#include <vector>

using namespace std;

struct File {
  string name;

  int size = 0;

  File *parent = nullptr;

  map<string, File *> contents;

  int getSize() {
    if (size == 0) {
      for (auto &pair : contents) {
        size += pair.second->getSize();
      }
    }
    return size;
  }

  bool is_dir() { return contents.size() > 0; }

  int sumSizeSmallerThan(int max_size) {
    if (!is_dir())
      return 0;

    int sum = 0;
    for (auto &pair : contents) {
      sum += pair.second->sumSizeSmallerThan(max_size);
    }

    if (size <= max_size)
      sum += size;

    return sum;
  }

  int smallestDirLargerThan(int required_space) {
    int min_size = INT32_MAX;

    if (!is_dir())
      return min_size;

    for (auto &pair : contents) {
      auto tmp = pair.second->smallestDirLargerThan(required_space);
      if (tmp < min_size)
        min_size = tmp;
    }

    if (size >= required_space && size < min_size)
      min_size = size;

    return min_size;
  }
};

int is_command(const string &line) {
  if (line.find('$') == string::npos)
    return 0;

  return line.find("cd");
}

tuple<string, int> parse_file(const string &line) {
  int pos = line.find(' '), size = 0;

  if (line.find("dir") != 0) {
    size = stoi(line.substr(0, pos));
  }

  auto name = line.substr(pos + 1);

  return {name, size};
}

File *read_file_system(vector<string> output) {
  auto root = new File;
  root->name = "/";

  File *current = root;

  int index = 0;
  for (auto &line : output) {

    // cout << index++ << " " << current->name << endl;
    auto cd = is_command(line);
    if (cd) {
      if (cd < 0)
        continue; // ls

      auto dir_name = line.substr(cd + 3);

      if (dir_name == "..") {
        if (current->parent != nullptr)
          current = current->parent;
        continue;
      }

      if (dir_name == root->name) {
        current = root;
        continue;
      }

      current = current->contents[dir_name];

    } else {

      auto data = parse_file(line);
      auto name = get<0>(data);
      auto size = get<1>(data);

      if (current->contents.find(name) != current->contents.end())
        continue;

      current->contents[name] = new File;
      current->contents[name]->name = name;
      current->contents[name]->size = size;
      current->contents[name]->parent = current;
    }
  }

  root->getSize();
  return root;
}

void print_tree(File *root, int tabs) {
  for (auto i = 0; i < tabs; i++)
    cout << '\t';
  cout << '-' << root->name;
  if (root->is_dir())
    cout << " (dir)";
  else
    cout << " (file, size=" + to_string(root->size) + ")";
  cout << endl;

  for (auto &pair : root->contents) {
    print_tree(pair.second, tabs + 1);
  }
}

std::vector<std::string> parse_input(const std::string &file_name) {
  std::vector<std::string> ret;

  std::ifstream fs(file_name);
  std::string line;
  while (std::getline(fs, line)) {
    ret.push_back(line);
  }
  return ret;
}

int part1(File *root) { return root->sumSizeSmallerThan(100000); }

int part2(File *root) {
  auto unused_space = 70000000 - root->size;
  auto space_required = 30000000 - unused_space;

  return root->smallestDirLargerThan(space_required);
}

int main() {
  auto lines = parse_input("input.txt");
  // auto lines = parse_input("test.txt");

  auto root = read_file_system(lines);
  // print_tree(root, 0);

  auto r1 = part1(root);
  std::cout << "Part 1: " << r1 << std::endl;
  auto r2 = part2(root);
  std::cout << "Part 2: " << r2 << std::endl;
  return 0;
}
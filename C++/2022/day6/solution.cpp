#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <unordered_map>
#include <vector>

std::string parse_input(const std::string &file_name) {
  std::string ret;

  std::ifstream fs(file_name);
  std::string line;
  while (std::getline(fs, line)) {
    ret = line;
  }
  return ret;
}

int detect_marker(const std::string &data, int marker_size) {
  int count = data.length();

  for (int i = 0; i < count - marker_size; i++) {
    std::unordered_map<int, int> packet{{data[i], i}};
    for (int j = 1; j < marker_size; j++) {
      auto key = data[i + j];
      if (packet.count(key))
        break;
      packet[key] = i + j;
    }

    if (packet.size() == marker_size)
      return i + marker_size;
  }

  return -1;
}

int part1(const std::string &data) { return detect_marker(data, 4); }

int part2(std::string data) { return detect_marker(data, 14); }

int main() {

  auto data = parse_input("input.txt");
  // auto data = parse_input("test.txt");

  auto r1 = part1(data);
  std::cout << "Part 1: " << r1 << std::endl;

  auto r2 = part2(data);
  std::cout << "Part 2: " << r2 << std::endl;
  return 0;
}
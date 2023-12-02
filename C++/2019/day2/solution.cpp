#include <algorithm>
#include <fstream>
#include <functional>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include "..\intcode.h"

using namespace IntCode;

program_t parse_input(const std::string &file_name) {
  program_t ret;

  std::ifstream fs(file_name);
  std::string line;
  while (std::getline(fs, line, ',')) {
    ret.code.push_back(std::stoi(line));
  }

  return ret;
}

int part1(program_t intcode) {

  // Set state to 1202 alarm
  intcode.code[1] = 12;
  intcode.code[2] = 2;

  run_program(intcode);

  return intcode.code[0];
}

int part2(const program_t intcode) {
  int output = 19690720, noun, verb;

  for (noun = 0; noun < 100; noun++)
    for (verb = 0; verb < 100; verb++) {
      program_t trial_run(intcode);

      trial_run.code[1] = noun;
      trial_run.code[2] = verb;

      run_program(trial_run);

      if (trial_run.code[0] == output) {
        std::cout << noun << "," << verb << ":" << trial_run.code[0]
                  << std::endl;
        return 100 * noun + verb;
      }
    }

  return -1;
}

int main() {

  auto intcode = parse_input("input.txt");
  // auto intcode = parse_input("test.txt");

  auto r1 = part1(intcode);
  std::cout << "Part 1: " << r1 << std::endl;

  auto r2 = part2(intcode);
  std::cout << "Part 2: " << r2 << std::endl;
  return 0;
}
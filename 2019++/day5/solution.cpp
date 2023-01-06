#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>
#include <map>

#include "../intcode.h"

using namespace IntCode;

program_t parse_input(const std::string& file_name){
    program_t ret;
    
    std::ifstream fs(file_name);
    std::string line;
    while(std::getline(fs, line, ',')){
        ret.code.push_back(std::stoi(line));
    }

    return ret;
}

int part1(program_t intcode){
    run_program(intcode);

    return 0;
}

int part2(program_t intcode){
    return 0;
}

int main(){

    auto intcode = parse_input("input.txt");
    //auto intcode = parse_input("test.txt");

    auto r1 = part1(intcode);
    std::cout << "Part 1: "<< r1 << std::endl;

    auto r2 = part2(intcode);
    std::cout << "Part 2: "<< r2 << std::endl;
    return 0;
}
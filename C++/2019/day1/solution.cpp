#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>
#include <map>

std::vector<int> parse_input(const std::string& file_name){
    std::vector<int> ret;
    
    std::ifstream fs(file_name);
    std::string line;
    while(std::getline(fs, line)){
        ret.push_back(std::stoi(line));
    }
    return ret;
}

int part1(std::vector<int> modules){
    int total = 0,
        required;

    for (auto mass : modules){
        required = mass / 3 - 2;
        total += required;
    }
    return total;
}

int part2(std::vector<int> modules){
    int total = 0,
        required;

    for (auto mass : modules){
        required = mass;
        for(;;){
            required = required / 3 - 2;
            if(required <= 0)
                break;
            total += required;
        }
    }
    return total;
}

int main(){

    auto lines = parse_input("input.txt");
    //auto lines = parse_input("test.txt");

    auto r1 = part1(lines);
    std::cout << "Part 1: "<< r1 << std::endl;

    auto r2 = part2(lines);
    std::cout << "Part 2: "<< r2 << std::endl;
    return 0;
}
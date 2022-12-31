#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <functional>
#include <numeric>
#include <algorithm>


using elf_t = std::vector<int>;

using elves_list_t = std::vector<elf_t>;

elves_list_t parse_input(const std::string& file_name){
    elves_list_t result;

    std::ifstream fs(file_name);

    std::string line;

    elf_t current;
        
    while(std::getline(fs, line)){
        if(line.empty()){
            result.push_back(current);
            current = {};
            continue;
        }
        current.push_back(std::stoi(line));
    }

    return result;
}

int part1(const elves_list_t& list){
    int max_sum = 0;

    for(auto elf : list){
        auto calories = std::accumulate(elf.begin(), elf.end(), 0);
        max_sum = std::max(max_sum, calories);
    }

    return max_sum;
}

int part2(const elves_list_t& list){
    std::vector<int> elves_sums;

    for(auto elf : list){
        auto calories = std::accumulate(elf.begin(), elf.end(), 0);
        elves_sums.push_back(calories);
    }

    std::partial_sort(elves_sums.begin(), elves_sums.begin() + 3, elves_sums.end(), std::greater<int>());

    return std::accumulate(elves_sums.begin(), elves_sums.begin() + 3, 0);
}

void print_list(const elves_list_t& list){
    for(auto elf : list){
        for(auto value : elf)
            std::cout << value << ",";

        std::cout << std::endl;
    }
}

int main(){
    auto list = parse_input("input.txt");

    std::cout << part1(list) << std::endl;
    std::cout << part2(list) << std::endl;

    return 0;
}
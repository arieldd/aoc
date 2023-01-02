#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>
#include <map>

struct crane_move_t{
    int number_of_crates;
    int from;
    int to;
};

using crate_stacks_t = std::vector<std::vector<char>>;
using move_list_t = std::vector<crane_move_t>;

std::vector<std::string> parse_input(const std::string& file_name){
    std::vector<std::string> ret;
    
    std::ifstream fs(file_name);
    std::string line;
    while(std::getline(fs, line)){
        ret.push_back(line);
    }
    return ret;
}

crate_stacks_t read_crate_stacks(std::vector<std::string> lines){
    crate_stacks_t ret;

    for(auto line : lines){
        for (int i = 0; i < line.length(); i += 4){
            auto stack = i / 4;
            if (stack >= ret.size())
                ret.push_back({});

            auto crate_label = line[i + 1];
            if (crate_label == ' ')
                continue;

            ret[stack].push_back(crate_label);
        }
    }  

    for (auto & stack : ret)
        std::reverse(stack.begin(), stack.end());

    return ret;
}

move_list_t read_move_list(std::vector<std::string> lines){
    move_list_t ret;

    for (auto line: lines){
        crane_move_t next;

        sscanf_s(line.c_str(),
            "move %d from %d to %d",
            &next.number_of_crates, &next.from, & next.to
        );

        ret.push_back(next);
    }

    return ret;
}

std::string part1(crate_stacks_t stacks, const move_list_t& moves){
    for(auto move : moves){

        auto number = move.number_of_crates;

        for(;number; number--){
           stacks[move.to -1].push_back(stacks[move.from - 1].back());
           stacks[move.from - 1].pop_back();
        }
    }

    std::string ret = "";

    for (auto stack: stacks)
        ret += stack.back();

    return ret;
}

std::string part2(crate_stacks_t stacks, const move_list_t& moves){
     for(auto move : moves){

        auto number = move.number_of_crates;

        auto & src = stacks[move.from - 1];
        auto & dst = stacks[move.to - 1];

        dst.insert( dst.end(), src.end() - number, src.end());
        src.resize(src.size() - number);
    }

    std::string ret = "";

    for (auto stack: stacks)
        ret += stack.back();

    return ret;
}

int main(){

    auto lines = parse_input("input.txt");
    //auto lines = parse_input("test.txt");

    auto div = std::find(lines.begin(), lines.end(), "");

    auto stacks = read_crate_stacks(std::vector<std::string> (lines.begin(), div - 1));

    auto moves = read_move_list(std::vector<std::string> (div + 1, lines.end()));

    auto r1 = part1(stacks, moves);
    std::cout << "Part 1: "<< r1 << std::endl;

    auto r2 = part2(stacks, moves);
    std::cout << "Part 2: "<< r2 << std::endl;
    return 0;
}
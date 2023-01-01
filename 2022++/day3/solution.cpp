#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>
#include <set>

using rucksack_list_t = std::vector<std::string>;

rucksack_list_t parse_input(const std::string& file_name){
    rucksack_list_t ret;
    
    std::ifstream fs(file_name);
    std::string line;
    while(std::getline(fs, line)){
        ret.push_back(line);
    }
    return ret;
}

int item_priority(char item){
    if (item > 96)
        return item - 96;
    return item - 38;
}

int part1(const rucksack_list_t& rucksacks){
    std::vector<char> items;

    int half_size;
    for(auto r : rucksacks){
        half_size = r.length() / 2;
        auto pocket1 = r.substr(0, half_size),
             pocket2 = r.substr(half_size);
        
        std::set<char> set1 (pocket1.begin(), pocket1.end());
        std::set<char> set2 (pocket2.begin(), pocket2.end());

        std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), 
            std::back_inserter(items));
    }

    return std::accumulate(items.begin(), items.end(), 0, [](auto& sum, auto& r){
        return sum + item_priority(r);
    });
}

int part2(std::vector<std::string> rucksacks){
    std::vector<char> badges;

    int length = rucksacks.size();
    for(int ix = 0; ix < length; ix += 3){
        auto r1 = rucksacks[ix],
             r2 = rucksacks[ix + 1],
             r3 = rucksacks[ix + 2];
        
        std::set<char> set1 (r1.begin(), r1.end());
        std::set<char> set2 (r2.begin(), r2.end());
        std::set<char> set3 (r3.begin(), r3.end());

        std::vector<char> mid_step;

        std::set_intersection(set1.begin(), set1.end(), set2.begin(), set2.end(), std::back_inserter(mid_step));

        std::set_intersection(mid_step.begin(), mid_step.end(), set3.begin(), set3.end(), std::back_inserter(badges));
    }

    return std::accumulate(badges.begin(), badges.end(), 0, [](auto& sum, auto& r){
        return sum + item_priority(r);
    });
}

int main(){

    auto rucksacks = parse_input("input.txt");
    //auto rucksacks = parse_input("test.txt");

    auto r1 = part1(rucksacks);
    std::cout << "Part 1: "<< r1 << std::endl;

    auto r2 = part2(rucksacks);
    std::cout << "Part 2: "<< r2 << std::endl;
    return 0;
}
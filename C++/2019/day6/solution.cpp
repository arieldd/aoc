#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <sstream>

struct body_t{
    std::string name;

    std::string orbits;
    std::vector<std::string> orbitted_by;

    int total_orbits;
};

void update_orbit_total(std::unordered_map<std::string, body_t>& orbits, const std::string& current_name){
    body_t& current = orbits.at(current_name);
    for(auto name: current.orbitted_by){
        update_orbit_total(orbits, name);
        current.total_orbits += 1 + orbits.at(name).total_orbits;
    }
}

int find_orbital_tranfers(std::unordered_map<std::string, body_t>& orbits, 
    const std::string& current_name, const std::string& dst_name, 
    std::unordered_set<std::string> visited){
    
    if (current_name == dst_name)
        return 0;

    visited.insert(current_name);

    auto current = orbits[current_name];
    std::vector<std::string> to_visit (current.orbitted_by);
    to_visit.push_back(current.orbits);
    

    for(auto name : to_visit){
        if (visited.count(name) > 0)
            continue;
        auto jumps = find_orbital_tranfers(orbits, name, dst_name, visited);

        if (jumps >= 0)
            return jumps + 1;
    }

    return -1;
}

std::unordered_map<std::string, body_t> parse_input(const std::string& file_name){
    std::unordered_map<std::string, body_t> ret;
    
    std::ifstream fs(file_name);
    std::string line;
    int delim_index;
    while(std::getline(fs, line)){
        delim_index = line.find(')');
        auto name1 = line.substr(0, delim_index),
             name2 = line.substr(delim_index + 1);

        body_t b1 {.name = name1 }
              ,b2 {.name = name2 };

        if(! ret.count(name1))
            ret[name1] = b1;

        if(! ret.count(name1))
            ret[name2] = b2;

        ret[name1].orbitted_by.push_back(name2);
        ret[name2].orbits = name1;
    }
    return ret;
}

void print_orbits(const std::unordered_map<std::string, body_t>& orbits){
    for(auto pair : orbits){
        std::cout << pair.first << std::endl;
        for (auto b : pair.second.orbitted_by){
            std::cout << '\t' << b << std::endl;
        }
    }
}

int part1(const std::unordered_map<std::string, body_t>& orbits){
    int total = 0;

    for(auto pair : orbits){
        total += pair.second.total_orbits;
    }

    return total;
}

int part2(std::unordered_map<std::string, body_t>& orbits){
    auto you = orbits.at("YOU");
    auto san = orbits.at("SAN");

    return find_orbital_tranfers(orbits, you.orbits, san.orbits, {});
}

int main(){

    auto orbits = parse_input("input.txt");
    //auto orbits = parse_input("test.txt");

    update_orbit_total(orbits, "COM");

    auto r1 = part1(orbits);
    std::cout << "Part 1: "<< r1 << std::endl;

    auto r2 = part2(orbits);
    std::cout << "Part 2: "<< r2 << std::endl;
    return 0;
}
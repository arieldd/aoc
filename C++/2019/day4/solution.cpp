#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>
#include <map>

struct interval_t{
    int low;
    int high;

    interval_t() : low(0), high(0) {};
    interval_t(const int& x1, const int& x2) : low(x1), high(x2) {};
};

interval_t parse_input(const std::string& file_name){
    std::vector<int> limits;

    std::ifstream fs(file_name);
    std::string line;
    while(std::getline(fs, line, '-')){
        limits.push_back(std::stoi(line));        
    }
    return interval_t(limits[0], limits[1]);
}

std::vector<int> get_digits(int number){
    std::vector<int> digits;
    while(number){
        digits.push_back(number % 10);
        number /= 10;
    }

    return digits;
}

bool is_valid_password(int candidate){
    auto digits = get_digits(candidate);

    bool repeated = false;
    for(auto it = digits.rbegin() + 1; it < digits.rend(); it++){
        if(*it < *(it -1))
            return false;
        
        if(*it == *(it -1))
            repeated = true;
    }

    return repeated;
}

bool is_valid_password_2(int candidate){
    auto digits = get_digits(candidate);

    bool found_double = false;
    int repeated = 1;
    for(auto it = digits.rbegin() + 1; it < digits.rend(); it++){
        if(*it < *(it -1))
            return false;
        
        if(*it != *(it -1)){
            if(!found_double)
                found_double = repeated == 2;
            repeated = 0;
        }

        repeated += 1;
    }

    if(!found_double)
        found_double = repeated == 2;

    return found_double;
}

int part1(interval_t interval){
    int passwords = 0;

    for(int i = interval.low; i <= interval.high; i++){
        if(is_valid_password(i))
            passwords += 1;
    }

    return passwords;
}

int part2(interval_t interval){
    int passwords = 0;

    for(int i = interval.low; i <= interval.high; i++){
        if(is_valid_password_2(i))
            passwords += 1;
    }

    return passwords;
}

int main(){

    auto interval = parse_input("input.txt");
    //auto interval = parse_input("test.txt");

    std::cout << interval.low << ',' << interval.high << std::endl;

    auto r1 = part1(interval);
    std::cout << "Part 1: "<< r1 << std::endl;

    auto r2 = part2(interval);
    std::cout << "Part 2: "<< r2 << std::endl;
    return 0;
}
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <numeric>
#include <functional>
#include <vector>
#include <map>

unsigned mod ( int value, unsigned m);

std::map<char, int> elf_moves { 
    { 'A', 1},
    { 'B', 2},
    { 'C', 3}
}; 

std::map<char, int> own_moves {
    { 'X', 1 },
    { 'Y', 2},
    { 'Z', 3}
};

struct round_t{
    char elf;
    char player;
    int score;
};

using round_list_t = std::vector<round_t>;

round_list_t parse_input(const std::string& file_name){
    round_list_t ret;
    
    std::ifstream fs(file_name);
    std::string line;
    while(std::getline(fs, line)){
        round_t next { line[0], line[2]};

        ret.push_back(next);
    }
    return ret;
}

int part1(const round_list_t& rounds){
    int score = 0;

    int elf_move, player_move;
    for(auto round : rounds){
        elf_move = elf_moves[round.elf];
        player_move = own_moves[round.player];

        auto result = mod(elf_move -player_move, 3);

        if (result != 2)
            result = 1 - result;
        
        score += result * 3 + player_move;
    }

    return score;
}

int part2(const round_list_t& rounds){
    int score = 0;

    std::vector<int> plays {1, 2, 3, 1, 2, 3};

    int elf_move, player_move;
    for(auto round : rounds){
        elf_move = elf_moves[round.elf];

        auto result = own_moves[round.player] - 1; // 0 1 2 = lose draw win
 
        player_move = elf_move;
        if (result == 2)
            player_move = plays[elf_move];
        else if (result == 0)
            player_move = plays[elf_move + 1];

        score += result * 3 + player_move;
    }

    return score;
}

void print_moves(const round_list_t& moves){
    for(auto round : moves){
        std::cout << round.elf << " " << round.player << std::endl;
    }
}

unsigned mod ( int value, unsigned m) {
    int mod = value % (int)m;
    if (mod < 0) {
        mod += m;
    }
    return mod;
}

int main(){

    auto moves = parse_input("input.txt");
    //auto moves = parse_input("test.txt");

    auto score = part1(moves);
    std::cout << "Part 1: "<< score << std::endl;

    score = part2(moves);
    std::cout << "Part 2: "<< score << std::endl;
    return 0;
}
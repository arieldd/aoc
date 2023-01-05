#include <vector>
#include <iostream>
#include <unordered_map>

#pragma once

namespace IntCode{

enum opcode { add=1, mult=2, end=99 };

struct instr_t{
    opcode op;
    std::vector<int> params;
};

static std::unordered_map< opcode, int, std::hash<int> > instr_set ={
    {add, 4},
    {mult, 4},
    {end, 1}
};

struct program_t{
    std::vector<int> code;
};

void run_program(program_t & intcode);

void execute_instruction(const instr_t& instr, program_t& code);

void print_program(const program_t & intcode);
}// namespace IntCode
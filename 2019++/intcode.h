#include <vector>
#include <iostream>
#include <unordered_map>

#pragma once

namespace IntCode{

enum opcode { 
    add=1, mult=2, //arithmetic
    input=3, output=4, //io
    jmp_t=5, jmp_f=6, //conditionals
    lt = 7, eq= 8, //comparisons
    end=99 //terminate
};

struct instr_t{
    opcode op;
    std::vector<int> params;
    std::vector<int> modes {0,0,0};
};

static std::unordered_map< opcode, int, std::hash<int> > instr_set ={
    {add, 4},
    {mult, 4},
    {input, 2},
    {output, 2},
    {jmp_t, 3},
    {jmp_f, 3},
    {lt, 4},
    {eq, 4},
    {end, 1}
};

struct program_t{
    std::vector<int> code;
};

instr_t decode_instruction(const std::vector<int>::iterator& instr_pointer);

std::vector<int> load_instruction(const instr_t& instr, const program_t& intcode);

void run_program(program_t & intcode);

int execute_instruction(const instr_t& instr, program_t& intcode, std::vector<int>::iterator& instr_pointer);

void print_program(const program_t & intcode);
}// namespace IntCode
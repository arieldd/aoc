#include "intcode.h"

void IntCode::run_program(program_t & intcode){

    int result, size;
    for(auto it = intcode.code.begin(); it < intcode.code.end(); it += size){
        size = 1;
        

        instr_t instr = { .op = opcode(*it) };

        if ( ! instr_set.count(instr.op)){
            std::cout << "Wrong opcode" << std::endl;
            return;
        }

        size = instr_set[instr.op];

        if (instr.op == end)
            break;

        for (int i = 1; i < size; i++){
            instr.params.push_back(*(it + i));    
        }

        execute_instruction(instr, intcode);
    }
}

void IntCode::execute_instruction(const instr_t& instr, program_t& intcode){
        int op1, op2;
        
        switch (instr.op)
        {
        case add:
            op1 = intcode.code[instr.params[0]],
            op2 = intcode.code[instr.params[1]];

            intcode.code[instr.params[2]] = op1 + op2;
            break;

        case mult:
            op1 = intcode.code[instr.params[0]],
            op2 = intcode.code[instr.params[1]];

            intcode.code[instr.params[2]] = op1 * op2;
            break;
        
        default: //99 end
            break;
        }
}

void IntCode::print_program(const program_t & intcode){
    for(auto e : intcode.code)
        std::cout << e << ",";
    std::cout << std::endl;
}
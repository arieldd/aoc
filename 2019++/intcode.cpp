#include "intcode.h"

namespace IntCode{
    void run_program(program_t & intcode){
        int result, jmp_size;
        for(auto it = intcode.code.begin(); it < intcode.code.end(); ){
            instr_t instr = decode_instruction(it);

            if (instr.op == end)
                break;

           it += execute_instruction(instr, intcode, it);
        }
    }

    instr_t decode_instruction(const std::vector<int>::iterator& instr_pointer){
        instr_t ret;
        
        int op_val = *instr_pointer;
        ret.op = opcode(op_val % 100);

        op_val /= 100;
        int i = 0;
        while(op_val){
            ret.modes[i] = op_val%10;
            op_val /= 10;
            i++;
        }

        if ( ! instr_set.count(ret.op)){
            std::cout << "Wrong opcode " << ret.op << std::endl;
            ret.op = end;
            return ret;
        }

        int jmp_size = instr_set[ret.op];

        for (int i = 1; i < jmp_size; i++){
            ret.params.push_back(*(instr_pointer + i));    
        }

        return ret;
    }

    std::vector<int> load_instruction(const instr_t& instr, const program_t& intcode){
        std::vector<int> instr_vals;

        int param_count = instr_set[instr.op] - 1;
        int param,
            mode,
            value;

        for (int i = 0; i < param_count; i++){
            mode = instr.modes[i];
            param = instr.params[i];

            value = (mode)? param : intcode.code[param];
            
            instr_vals.push_back(value);
        }

        return instr_vals;
    }

    int execute_instruction(const instr_t& instr, program_t& intcode, std::vector<int>::iterator& instr_pointer){
            
            auto instr_vals = load_instruction(instr, intcode);

            int jmp_size = instr_set[instr.op];

            int op1, op2;
            switch (instr.op)
            {
            case add:
                op1 = instr_vals[0];
                op2 = instr_vals[1];

                intcode.code[instr.params[2]] = op1 + op2;
                break;

            case mult:
                op1 = instr_vals[0];
                op2 = instr_vals[1];

                intcode.code[instr.params[2]] = op1 * op2;
                break;

            case input:
                int value;
                std::cout << "Please introduce input value:" << std::endl;
                std::cin >> value;

                op1 = instr.params[0];

                intcode.code[op1] = value;
                break;

            case output:
                std::cout << "Output: " << instr_vals[0] << std::endl;
                break;

            case jmp_t:
                op1 = instr_vals[0];
                op2 = instr_vals[1];
                
                if(op1){
                    instr_pointer = std::next(intcode.code.begin(), op2);
                    jmp_size = 0;   
                }   
                break;

            case jmp_f:
                op1 = instr_vals[0];
                op2 = instr_vals[1];
                
                if( ! op1){
                    instr_pointer = std::next(intcode.code.begin(), op2);
                    jmp_size = 0;
                }   
                break;

            case lt:
                op1 = instr_vals[0];
                op2 = instr_vals[1];

                intcode.code[instr.params[2]] = (op1 < op2) ? 1 : 0;
                break;

            case eq:
                op1 = instr_vals[0];
                op2 = instr_vals[1];

                intcode.code[instr.params[2]] = (op1 == op2) ? 1 : 0;
                break;

            default: //99 end
                break;
            }

        return jmp_size;
    }

    void print_program(const program_t & intcode){
        for(auto e : intcode.code)
            std::cout << e << ",";
        std::cout << std::endl;
    }
}// namespace IntCode

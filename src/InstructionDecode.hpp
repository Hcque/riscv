// JAL loop forever


#ifndef INSTRUCTIONDECODE_HPP
#define INSTRUCTIONDECODE_HPP
#include "Instruction.hpp"
#include "memory.hpp"
#include "register.hpp"
#include "Execution.hpp"

#include "Common.hpp"

class Execution;

class InstructionDecode {
    Register *regs;
public:
    Instruction inst;
    InstructionDecode() {}
    InstructionDecode(Register *_regs) : regs(_regs) {}
    void go(){

        inst.type = inst.getType();
        regs->_types.insert(inst.type);
        inst.getArgs();
    }
    void pass(Execution& next){
        next.inst = inst;

        // cout << inst;
 
        // std::cerr << "DONE: ID: type: " << inst.type << "\n";
    }

};

#endif
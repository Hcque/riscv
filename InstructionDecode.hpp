#ifndef INSTRUCTIONDECODE_HPP
#define INSTRUCTIONDECODE_HPP
#include "Instruction.hpp"
#include "memory.hpp"
#include "register.hpp"
#include "Execution.hpp"
class Execution;

class InstructionDecode {
    Register *regs;
public:
    memory mem;
    Instruction inst;
    InstructionDecode() {}
    InstructionDecode(Register *_regs) : regs(_regs) {}
    void go(){
        inst.type = inst.getType();
        inst.getArgs();

    }
    void pass(Execution& next){
        next.inst = inst;
    }

};

#endif
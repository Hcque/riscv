#ifndef INSTRUCTIONFETCH_HPP
#define INSTRUCTIONFETCH_HPP

#include "Common.hpp"

class InstructionFetch {
    Register *regs;
public:
    memory mem;
    Instruction inst;
    InstructionFetch(){}
    InstructionFetch(Register *_regs) : regs(_regs) {}

    void go(){
        inst.fromMemory = mem.load(regs->pc, 4);
    }

    void pass(InstructionDecode &next){
            next.inst = inst;
    }

};

#endif


#ifndef MEMORYACCESS_HPP
#define MEMORYACCESS_HPP

#include "Instruction.hpp"
#include "memory.hpp"
#include "register.hpp"

#include "WriteBack.hpp"
#include "util.hpp"

#include "Stage.hpp"


class MemoryAccess: public Stage {
public:
    Register *regs;
    Instruction inst;
    bool stall;

    MemoryAccess() {}
    MemoryAccess(Register *_regs) : regs(_regs) {}

    MemoryAccess(Register *_regs, Instruction _inst) : regs(_regs), inst(_inst) {}
    MemoryAccess(Register *_regs,  bool _st) : regs(_regs), stall(_st){}

    void go(){
        switch (inst.type){
            case ERROR: return; break;
            case LB: inst.dest = signext( regs->load(inst.dest, 1), 7) ; break;
            case LH: inst.dest = signext( regs->load(inst.dest, 2), 15) ; break;
            case LW: inst.dest = regs->load(inst.dest, 4); break;

            case LBU: inst.dest = regs->load(inst.dest, 1); break;
            case LHU: inst.dest = regs->load(inst.dest, 2); break;

            case SB: regs->store(inst.dest, 1, inst.src2); break;
            case SH: regs->store(inst.dest, 2, inst.src2); break;
            case SW: regs->store(inst.dest, 4, inst.src2); break;
        }
    }

    void pass(WriteBack &next){
        next.inst = inst;
        inst.type = ERROR;
        // std::cerr << "DONE: MA:" << inst.dest << "\n";

    }

};

#endif

#ifndef MEMORYACCESS_HPP
#define MEMORYACCESS_HPP


#include "Instruction.hpp"
#include "memory.hpp"
#include "register.hpp"

#include "WriteBack.hpp"
class MemoryAccess {
    Register *regs;
    public:
    memory mem;
    Instruction inst;
    MemoryAccess() {}
    MemoryAccess(Register *_regs) : regs(_regs) {}
    void go(){
        switch (inst.type){
            case LB: inst.dest = mem.load(inst.src1, 1); break;
            case LH: inst.dest = mem.load(inst.src1, 2); break;
            case LW: inst.dest = mem.load(inst.src1, 4); break;
            case LBU: inst.dest = mem.load(inst.src1, 1); break;//TODO
            case LHU: inst.dest = mem.load(inst.src1, 2); break;

            case SB: mem.store(inst.src1, 1, inst.src2); break;
            case SH: mem.store(inst.src1, 2, inst.src2); break;
            case SW: mem.store(inst.src1, 4, inst.src2); break;
        }
    }

    void pass(WriteBack &next){
        next.inst = inst;

    }

};

#endif
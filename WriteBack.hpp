
#ifndef WRITEBACK_HPP
#define WRITEBACK_HPP

#include "Common.hpp"
class WriteBack {
    Register *regs;
    public:
    memory mem;
    Instruction inst;
    WriteBack() {}
    WriteBack(Register *_regs) : regs(_regs) {}
    void go(){
        switch (inst.type){
            case ADDI:
            case SLTI:
            case SLTIU:
            case XORI:
            case ORI:
            case ANDI:

            case SLLI:
            case SRLI:
            case SRAI:

            case ADD:
            case SUB:
            case SLL:
            case SLT:
            case SLTU:
            case XOR:
            case SRL:
            case SRA:
            case OR:
            case AND:
            regs->set(inst.rd, inst.dest);
            break;
        }
    }

};

#endif
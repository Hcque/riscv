// so LOAD not added


#ifndef WRITEBACK_HPP
#define WRITEBACK_HPP

#include "Common.hpp"
class WriteBack {
    Register *regs;
    public:
    Instruction inst;
    WriteBack() {}
    WriteBack(Register *_regs) : regs(_regs) {}

    WriteBack(Register *_regs, Instruction _inst) : regs(_regs), inst(_inst) {}


    void go(){
        switch (inst.type){
            case LUI:
            case AUIPC:
            case JAL:
            case JALR:

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

            case LB: 
            case LH: 
            case LW: 
            case LBU: 
            case LHU:
            // cout << inst.dest;
            regs->set(inst.rd, inst.dest); // write back
            break;
        }
        // std::cout << *regs;
        // std::cout << "DONE WB ====================== \n\n";
    }

};

#endif
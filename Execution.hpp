
#ifndef EXECUTION_HPP
#define EXECUTION_HPP
#include "Instruction.hpp"
#include "memory.hpp"
#include "register.hpp"
#include "MemoryAccess.hpp"
class MemoryAccess;

class Execution {
    Register *regs;
public:
    memory mem;
    Instruction inst;

    Execution() {}
    Execution(Register *_regs) : regs(_regs) {}

    void go(){
        inst.src1 = regs->get(inst.rs1);
        inst.src2 = regs->get(inst.rs2);

        switch (inst.type){
            case LUI: inst.dest = inst.imm << 12; break;//TODO
            case AUIPC: regs->pc - 4 + (inst.imm << 12); break;
            case JAL: regs->pc += inst.imm; break;
            case JALR: inst.dest = inst.src1 + inst.imm; break;

            case BEQ: inst.dest = inst.src1 == inst.src2; break;
            case BNE: inst.dest = inst.src1 != inst.src2; break;
            case BLT: inst.dest = (int)inst.src1 < (int)inst.src2; break;
            case BGE: inst.dest = (int)inst.src1 > (int)inst.src2; break;
            case BLTU: inst.dest = inst.src1 < inst.src2; break;
            case BGEU: inst.dest = inst.src1 > inst.src2; break;

            case LB: 
            case LH:
            case LW:
            case LBU:
            case LHU:

            case SB:
            case SH:
            case SW:
            inst.dest = inst.src1 + inst.imm;
            break;

            case ADDI: inst.dest = inst.src1 + inst.imm; break;
            case SLTI: inst.dest = ((int)inst.src1 < (int)inst.imm) ? 1:0; break;
            case SLTIU: inst.dest = (inst.src1 < inst.imm) ? 1:0; break;
            case XORI: inst.dest = inst.src1 ^ inst.imm; break;
            case ORI: inst.dest = inst.src1 | inst.imm; break;
            case ANDI: inst.dest = inst.src1 & inst.imm; break;

            case SLLI: inst.dest = inst.src1 << inst.imm; break;
            case SRLI: inst.dest = inst.src1 >> inst.imm; break;
            case SRAI: inst.dest = inst.src1 << inst.imm; break;
           
            case ADD: inst.dest = inst.src1 + inst.src2; break;
            case SUB: inst.dest = inst.src1 - inst.src2; break;
            case SLL: inst.dest = inst.src1 << inst.src2; break;
            case SLT: inst.dest = ((int)inst.src1 < (int)inst.src2) ? 1:0; break;
            case SLTU: inst.dest = (inst.src1 < inst.src2) ? 1:0; break;
            case XOR: inst.dest = inst.src1 ^ inst.src2; break;
            case SRL: inst.dest = inst.src1 >> inst.src2; break;
            case SRA: inst.dest = inst.src1 >> inst.src2; break; //TODO
            case OR: inst.dest = inst.src1 | inst.src2; break;
            case AND: inst.dest = inst.src1 & inst.src2; break;

        }

    }

    void pass(MemoryAccess &next){
        next.inst = inst;
    }

};

#endif

// LUI << 12 no need


// following the jump

// BNE case

// func7 SLAI


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
    Instruction inst;

    Execution() {}
    Execution(Register *_regs) : regs(_regs) {}
    Execution(Register *_regs, Instruction _inst) : regs(_regs), inst(_inst) {}

    uint32_t setlowZero(uint32_t x)
    {
        if (x & 1) return x^1;
        else return x;
    }
    void go(){
        inst.src1 = regs->get(inst.rs1);
        inst.src2 = regs->get(inst.rs2);

        switch (inst.type){
            case LUI: inst.dest = inst.imm ; break;
            case AUIPC: inst.dest = regs->pc = regs->pc -4 + inst.imm; break;
            case JAL: 
                inst.dest = regs->pc;
                regs->pc = regs->pc + inst.imm - 4; // subtract 4 from IF stage
                // cout << "==" << inst.imm<< endl;
                // cout << "==" << regs->pc << endl;
                break;
            case JALR: 
                inst.dest = regs->pc;
                regs->pc = inst.src1 + inst.imm; 
                regs->pc = setlowZero(regs->pc);
                // std::cout << regs->pc << "\n";
                break;

            case BEQ: inst.take_bch = (inst.src1 == inst.src2); break;
            case BNE: inst.take_bch = (inst.src1 != inst.src2); break;
            case BLT: inst.take_bch = (int)inst.src1 < (int)inst.src2; break;
            case BGE: inst.take_bch = (int)inst.src1 >= (int)inst.src2; break;
            case BLTU: inst.take_bch = inst.src1 < inst.src2; break;
            case BGEU: inst.take_bch = inst.src1 >= inst.src2; break;

            case LB: 
            case LH:
            case LW:
            case LBU:
            case LHU:

            case SB:
            case SH:
            case SW:
            inst.dest = inst.src1 + inst.imm;
            // cout << inst.src1 << endl;
            break;

            case ADDI: inst.dest = inst.src1 + inst.imm; break;
            case SLTI: inst.dest = ((int)inst.src1 < (int)inst.imm) ? 1:0; break;
            case SLTIU: inst.dest = (inst.src1 < inst.imm) ? 1:0; break;
            case XORI: inst.dest = inst.src1 ^ inst.imm; break;
            case ORI: inst.dest = inst.src1 | inst.imm; break;
            case ANDI: inst.dest = inst.src1 & inst.imm; break;

            case SLLI: inst.dest = inst.src1 << inst.imm; break;
            case SRLI: inst.dest = inst.src1 >> inst.imm; break;
            case SRAI: inst.dest = (inst.src1 << inst.imm ) | (inst.src1 >> 31 << 31); break; // TODO
           
            case ADD: inst.dest = inst.src1 + inst.src2; break;
            case SUB: inst.dest = inst.src1 - inst.src2; break;
            case SLL: inst.dest = inst.src1 << inst.src2; break;
            case SLT: inst.dest = ((int)inst.src1 < (int)inst.src2) ? 1:0; break;
            case SLTU: inst.dest = (inst.src1 < inst.src2) ? 1:0; break;
            case XOR: inst.dest = (inst.src1 ^ inst.src2);
            // std::cout << inst.src1 << "|XOR|" << inst.src2 << "|dest|" << inst.dest << std::endl;
            
             break;
            case SRL: inst.dest = inst.src1 >> inst.src2; break;
            case SRA: inst.dest = (inst.src1 >> inst.src2) + (inst.src1 >> 31 << 31); break; 
            case OR: inst.dest = inst.src1 | inst.src2; break;
            case AND: inst.dest = inst.src1 & inst.src2; break;
        }

        if (inst.type == BNE || inst.type == BEQ || inst.type == BLT || inst.type == BGE ||
         inst.type == BLTU || inst.type == BGEU)
         {
            if (inst.take_bch)
            {
                regs->pc = regs->pc - 4 + inst.imm;
            // std::cout << "BNE" << regs->pc << std::endl;
            }
         }
    }

    void pass(MemoryAccess &next){
        next.inst = inst;
        
        // std::cerr << "DONE: EX: dest: " << inst.dest << "\n";
    }

};

#endif
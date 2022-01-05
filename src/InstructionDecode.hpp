// JAL loop forever


#ifndef INSTRUCTIONDECODE_HPP
#define INSTRUCTIONDECODE_HPP
#include "Instruction.hpp"
#include "memory.hpp"
#include "register.hpp"
#include "Execution.hpp"

#include "Common.hpp"
#include "prediction.hpp"

extern bool stall;
class Execution;

class InstructionDecode {
public:
    Register *regs;
    StaticPred* pd;

    Instruction inst;
    InstructionDecode() {}
    InstructionDecode(Register *_regs) : regs(_regs) {}
    InstructionDecode(Register *_regs, StaticPred* _pd) : regs(_regs), pd(_pd) {}
    void go(){

        inst.type = inst.getType();
        regs->_types.insert(inst.type);
        inst.getArgs();

        inst.src1 = regs->get(inst.rs1);
        inst.src2 = regs->get(inst.rs2);

        // pred
        if (inst.type == BNE || inst.type == BEQ || inst.type == BLT || inst.type == BGE ||
         inst.type == BLTU || inst.type == BGEU )
         {
             if (pd->take()){

             }
             else {

             }

         }

    }
    void pass(Execution& next){
        if (0)// condition
        {
            stall = 1;
            inst.type = ERROR;
        }
        next.inst = inst;

        // cout << inst;
 
        // std::cerr << "DONE: ID: type: " << inst.type << "\n";
    }

};

#endif
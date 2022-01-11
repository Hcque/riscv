// JAL loop forever


#ifndef INSTRUCTIONDECODE_HPP
#define INSTRUCTIONDECODE_HPP
#include "Instruction.hpp"
#include "memory.hpp"
#include "register.hpp"
#include "Execution.hpp"

#include "Common.hpp"
#include "prediction.hpp"
#include "Stage.hpp"

// extern bool stall;
class Execution;

class InstructionDecode {
public:
    Register *regs;
    StaticPred* pd;
    bool lock;


    bool stall;
    Instruction inst;
    InstructionDecode(): lock(0) {}
    InstructionDecode(Register *_regs) : regs(_regs) , lock(0) {}
    InstructionDecode(Register *_regs, StaticPred* _pd) : regs(_regs), pd(_pd) {}
    InstructionDecode(Register *_regs, StaticPred* _pd, bool _st) : regs(_regs), pd(_pd) , stall(_st){}

    void go(){

        inst.type = inst.getType();
        // regs->_types.insert(inst.type);
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
        // std::cout << "LOCK:" << lock;
        if (!lock) 
        {
            next.inst = inst;
            inst.clear();
            // std::cout << inst;
        }
        else lock = 0;
 
    }

};

#endif
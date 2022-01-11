#ifndef INSTRUCTIONFETCH_HPP
#define INSTRUCTIONFETCH_HPP

#include "InstructionDecode.hpp"
#include <iostream>

#include "prediction.hpp"
#include "Stage.hpp"


class InstructionFetch {
public:
    friend class RISCV;
    Register *regs;

    StaticPred* pd;
    bool stall;
    // ControlUnit *ctrUnit;


    Instruction inst;
    InstructionFetch(){}
    InstructionFetch(Register *_regs) : regs(_regs) {}
    // InstructionFetch(Register *_regs) : regs(_regs) {}


    InstructionFetch(Register *_regs, StaticPred* _pd) : regs(_regs), pd(_pd) {}
    InstructionFetch(Register *_regs, StaticPred* _pd, bool _st) : regs(_regs), pd(_pd) , stall(_st){}

    void go(){

        if (regs->ctrUnit.stall) return;

        //     std::cout << "JUMP bch TAKEN: " << regs->ctrUnit.bch_taken << "\n";
        // if (regs->ctrUnit.bch_taken) 
        // {
        //     regs->pc = regs->ctrUnit.jump_pc;
        //     std::cerr << "JUMP" << regs->ctrUnit.jump_pc<< "\n";
        //     regs->ctrUnit.bch_taken = 0;
        // }

        inst.fromMemory = regs->load(regs->pc, 4);
        inst.addr = regs->pc;


    //     int I = 0;
    // int B = regs->pc;
    // for (;I<4; I++, B +=1)
    //     printf("%.2x ", regs->mem->data[B]);
    // cout << endl;

    // I = 0;
    // B = regs->pc;
    // for (;I<4; I++, B +=1)
    // {
    //     bitset<8> a(regs->mem->data[B]);
    //     cout << a << " ";
    // }
    // cout << endl;

    // cout << "after load" << endl;
    // cout << "ORZ ";

    // for (I = 3;I>=0; I-- )
    //     printf("%.2x ", (inst.fromMemory>>(8*I)) & (255u));
    // cout << endl;
    

    // for (I=3;I>=0; I-- )
    // {
    //     bitset<8> a(inst.fromMemory>>(8*I));
    //     cout << a << " ";
    // }
    // cout << endl;

    // predict
    // if (bchPred->take())
        regs->pc += 4;
    }

    void pass(InstructionDecode &next){
        next.inst = inst;
        inst.clear();
    }


};

#endif

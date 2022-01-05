#ifndef INSTRUCTIONFETCH_HPP
#define INSTRUCTIONFETCH_HPP

#include "InstructionDecode.hpp"
#include <iostream>

#include "prediction.hpp"


class InstructionFetch {
public:
    friend class RISCV;
    Register *regs;

    StaticPred* pd;


    Instruction inst;
    InstructionFetch(){}
    InstructionFetch(Register *_regs) : regs(_regs) {}
    InstructionFetch(Register *_regs, StaticPred* _pd) : regs(_regs), pd(_pd) {}

    void go(){

        if (stall) return;
        inst.fromMemory = regs->load(regs->pc, 4);


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
            // std::cerr << "DONE: IF: inst: " << inst.fromMemory << "\n";
    }

};

#endif

// from others JAL: 4168 / 4172

#ifndef RISCV_HPP
#define RISCV_HPP

#include <iostream>
#include "memory.hpp"
#include "register.hpp"

#include "Instruction.hpp"
#include "InstructionFetch.hpp"
#include "InstructionDecode.hpp"
#include "Execution.hpp"
#include "MemoryAccess.hpp"
#include "WriteBack.hpp"
#include "prediction.hpp"
#include "forward.hpp"
#include "Stage.hpp"



void _forwardResolve(Stage& st1, Stage& st2)
{
    if (st2.inst.rd !=0 && st2.inst.rd == st1.inst.rs1) st1.regs->set(st2.inst.rd, st2.regs->get( st2.inst.rd ) );
    if (st2.inst.rd !=0 && st2.inst.rd == st1.inst.rs2) st1.regs->set( st2.inst.rd, st2.regs->get( st2.inst.rd ) );
}

struct ControlUnit
{
    StaticPred* pd;
        bool stall;


};

class RISCV{
public:
    memory *mem;
    Register regs;
    StaticPred  bchPred;

    bool stall;

    InstructionFetch IF;
    InstructionDecode ID;
    Execution EX;
    MemoryAccess MA;
    WriteBack WB;

public:
    RISCV(memory* mem): mem(mem), regs(mem), bchPred(), stall(0) {
        IF = InstructionFetch(&regs, &bchPred, stall);
        ID = InstructionDecode(&regs, &bchPred, stall);
        EX = Execution(&regs, &bchPred, stall);
        MA = MemoryAccess(&regs, stall);
        WB = WriteBack(&regs, stall);
        regs.pc = 0;
    }


    void parallel(){

        int cc = 0;
        while (1)
        {
            WB.go();
            if (stall) stall = 0;
            MA.go();
            EX.go();
            ID.go();
            IF.go();
            std::cerr <<  cc++ << " pc:" << regs.pc << std::endl;

            if (regs._end) break;

            MA.pass(WB);
            forward(MA, EX);
            forward(MA, ID);
            EX.pass(MA);
            forward(EX, ID);

            // set the stall
            if ( 
                ID.inst.type == JAL || ID.inst.type == JALR ||
            ID.inst.type == AUIPC
            )
            {
                stall = 1;
            } 

 if (ID.inst.type == BNE || ID.inst.type == BEQ || ID.inst.type == BLT || ID.inst.type == BGE ||
         ID.inst.type == BLTU || ID.inst.type == BGEU )
         {
                stall = 1;
         }

            ID.pass(EX);
            IF.pass(ID);
        }
        

    }
    void serial(){
        regs.pc = 0;
        int cc = 0;
        while ( !regs._end ){
            std::cout <<  cc++ << " pc:" << regs.pc << std::endl;
            IF.go();
            IF.pass(ID);
            ID.go();
            ID.pass(EX);
            EX.go();
            EX.pass(MA);
            MA.go();
            MA.pass(WB);
            WB.go();
        }
    }

    void run(){
        // serial();
        parallel();
    }

    int output(){
        // std::cerr << "=====\n";
        // for (auto& item: regs._types) 
        // {
        //     std::cerr << _type_Name[item] << " ";
        // }
        return regs.reg[10] & 255u;
    }

};

#endif
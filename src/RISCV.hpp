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


class RISCV{
public:
    memory *mem;
    Register regs;

    InstructionFetch IF;
    InstructionDecode ID;
    Execution EX;
    MemoryAccess MA;
    WriteBack WB;

public:
    RISCV(memory* mem): mem(mem), regs(mem) {
        IF = InstructionFetch(&regs);
        ID = InstructionDecode(&regs);
        EX = Execution(&regs);
        MA = MemoryAccess(&regs);
        WB = WriteBack(&regs);
    }

    void parallel(){
        while (1)
        {
            WB.go();
            MA.go();
            EX.go();
            ID.go();
            IF.go();

            // if (End) break;

            // MA.pass(WB);
            // forward(MA, EX);
            // forward(MA, ID);
            // EX.pass(MA);
            // forward(EX, ID);

            // ID.pass(EX);
            // IF.pass(ID);
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
        regs.pc = 0;
        serial();
    }

    int output(){
        for (auto& item: regs._types) 
        {
            std::cout << _type_Name[item] << " ";
        }
        return regs.reg[10] & 255u;
    }

};

#endif
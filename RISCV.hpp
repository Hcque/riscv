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
    memory *mem;
    Register regs;
    InstructionFetch IF;
    InstructionDecode ID;
    Execution EX;
    MemoryAccess MA;
    WriteBack WB;

    public:
    RISCV() {}
    RISCV(memory* mem): mem(mem) {
        IF = InstructionFetch(&regs);
        ID = InstructionDecode(&regs);
        EX = Execution(&regs);
        MA = MemoryAccess(&regs);
        WB = WriteBack(&regs);
        
    }

    void each_instr(){
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
    void run(){
        regs.pc = 0;
        while (regs.pc != 0x30004 || regs.pc < 200){
            std::cout << "pc:" << regs.pc << std::endl;
            each_instr();
            regs.pc += 4;
        } 
    }

    int output(){
        return 0;
    }
    // void ()

};

#endif
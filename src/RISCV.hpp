// from others JAL: 4168 / 4172

// skip ID, write the schemeafter reading the book and concive the plan

// regs.ctrUnit.stall_pc = regs.pc - 8; // stall next valid addr NOT 4


#ifndef RISCV_HPP
#define RISCV_HPP

#include <iostream>
#include <cassert>
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



enum RunMode {Serial, Parallel};


// void _forwardResolve(Stage& st1, Stage& st2)
// {

//     if (st2.inst.rd !=0 && st2.inst.rd == st1.inst.rs1) st1.regs->set(st2.inst.rd, st2.regs->get( st2.inst.rd ) );
//     if (st2.inst.rd !=0 && st2.inst.rd == st1.inst.rs2) st1.regs->set( st2.inst.rd, st2.regs->get( st2.inst.rd ) );
// }



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
        regs.pc = 0;
    }


    void parallel(){

        int cc = 0;
        bool skipID = 0;
        while (1)
        {
            // go ===============================
            WB.go();

            std::cout << " regs ======= \n";
            std::cout << regs << "\n";

            if (regs.ctrUnit.stall) 
            {
                // regs.ctrUnit.stall = 0;
                // if (!regs.ctrUnit.bch_taken) regs.pc = regs.ctrUnit.stall_pc;
            }
            MA.go();
            EX.go();

            if (regs.ctrUnit.stall){
                if (regs.ctrUnit.bch_taken){}
                else
                {
                    regs.pc = regs.ctrUnit.stall_pc;
                }
                regs.ctrUnit.stall = 0;
                regs.ctrUnit.bch_taken = 0;
                skipID = 1;
            }

            if (skipID){
                skipID = 0;
            }
            else{
            ID.go();
              // set the stall
            if ( 
                ID.inst.type == JAL || ID.inst.type == JALR ||
            ID.inst.type == AUIPC
            )
            {
                regs.ctrUnit.stall = 1;
                regs.ctrUnit.stall_pc = regs.pc - 4;

                IF.inst.clear();
                regs.ctrUnit.bch_taken = 1;
                
            } 

            if (ID.inst.type == BNE || ID.inst.type == BEQ || ID.inst.type == BLT || ID.inst.type == BGE ||
            ID.inst.type == BLTU || ID.inst.type == BGEU )
            {
                    regs.ctrUnit.stall = 1;
                    regs.ctrUnit.stall_pc = regs.pc - 4;
                    IF.inst.clear();
                    // bch taken unknown
            }

            if (ID.inst.type != ERROR 
            && EX.inst.memRead
            && EX.inst.rd != 0 
            && (EX.inst.rd == ID.inst.rs1 || EX.inst.rd == ID.inst.rs2)
            )
            {
                regs.ctrUnit.stall = 1;
                regs.ctrUnit.stall_pc = regs.pc - 4; // stall next valid addr
                IF.inst.clear();
                std::cerr << "STORE THE DATA HAZARD FOR LOAD\n ";

            }

            } // skip ID

            if (!regs.ctrUnit.stall) IF.go();
            std::cout <<  cc++  << "stall:" << regs.ctrUnit.stall << " pc:" << regs.pc << std::endl;
            if (cc > 180) break;
            if (regs._end) break;

            // pass =======================================
            MA.pass(WB);
            cout << EX.inst;
            EX.pass(MA);
            cout << regs.ctrUnit ;

        if (! regs.ctrUnit.stall)
        {
            // forward before pass to EX/ need oprands
            forward(MA, ID, EX);
            forward(EX, ID);

            ID.pass(EX);
            IF.pass(ID);
        }


        } // while
        

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
            std::cout << EX.inst;
            EX.pass(MA);
            MA.go();
            MA.pass(WB);
            WB.go();
            std::cout << "regs ========= \n ";
            std::cout << regs ;
        }
    }

    void run(RunMode mode){
        if (mode == Serial)
            serial();
        else if (mode == Parallel)
            parallel();
        else assert(0);
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
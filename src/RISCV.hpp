// from others JAL: 4168 / 4172

// skip ID, write the schemeafter reading the book and concive the plan

// regs.ctrUnit.stall_pc = regs.pc - 0; // stall next valid addr NOT 4


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

           
          
            MA.go();
            std::cout << "src1" << EX.inst.src1 << std::endl;
            EX.go();
            cout << "EX inst\n";
            cout << EX.inst;
            // cout << "==========  EX dest  ========= \n";
            // cout << EX.inst.dest << "\n";
            // cout << "==========  EX dest  ========= \n";

            // cout << regs.pc << "|PC|\n";
            // cout << regs.ctrUnit;

 std::cout << " regs ======= \n";
            std::cout << regs << "\n";
           


        // cout << "IDprev inst\n";
        //     cout << ID.inst;
        //     cout << ID.inst.opcode << "\n";
            ID.go();
             cout << "ID inst\n";
            cout << ID.inst;
            

            // ==========================================
             // stall
        if (regs.ctrUnit.stall){
            regs.ctrUnit.stall = 0;
            EX.inst.stalled = 1;
        }

        if (!EX.inst.stalled){

         if ( 
                ID.inst.type == JAL || ID.inst.type == JALR ||
            ID.inst.type == AUIPC
            )
            {
                regs.ctrUnit.stall = 1;
                ID.lock = 0;

                IF.inst.clear();
            //     cout << "if inst\n";
            // cout << IF.inst;
                // ID.inst.clear();
                // regs.ctrUnit.bch_taken = 1;
                
            } 

            if (ID.inst.type == BNE || ID.inst.type == BEQ || ID.inst.type == BLT || ID.inst.type == BGE ||
            ID.inst.type == BLTU || ID.inst.type == BGEU )
            {
                    ID.lock = 0;
                    regs.ctrUnit.stall = 1;
                    // regs.ctrUnit.stall_pc = regs.pc - 4;
                    // regs.pc -= 4; // stall next valid addr
                    IF.inst.clear();
                    // ID.inst.clear();
                    // bch taken unknown
            }

            if (ID.inst.type != ERROR 
            && EX.inst.memRead
            && EX.inst.rd != 0 
            && (EX.inst.rd == ID.inst.rs1 || EX.inst.rd == ID.inst.rs2)
            )
            {
                ID.lock = 1;
                regs.ctrUnit.stall = 1;
                // regs.pc -= 4; // stall next valid addr
                IF.inst.clear();
                std::cout << "STORE THE DATA HAZARD FOR LOAD\n ";

            }
        }

            // ===================

            if (!regs.ctrUnit.stall) 
            {
                IF.go();
                cout << "IF\n";
                cout << IF.inst;
            }
            std::cout <<  cc++  << " =========++++===== stall:" << regs.ctrUnit.stall << " pc:" << regs.pc << std::endl;
            std::cout << "\n";
            // if (cc > 280) break;
            if (regs._end) break;

            // pass =======================================
            MA.pass(WB);
            EX.pass(MA); // passed!
               // forward before pass to EX/ need oprands
            forward(WB, ID);
            forward(MA, ID, EX);

            ID.pass(EX);
            // cout << "ID check\n";
            // cout << ID.inst << ID.inst.opcode << "\n";
          

        if (! regs.ctrUnit.stall)
        {
         
            IF.pass(ID);
        }

        std::cout << "\n\n\n";


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
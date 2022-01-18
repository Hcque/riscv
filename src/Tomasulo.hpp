#pragma once

#include <unordered_map>

#include "memory.hpp"
#include "register.hpp"
#include "Instruction.hpp"

enum RSID
{
    ADD1,ADD2,ADD3,ADD4,
    LOAD1,LOAD2,LOAD3,
    STORE1,STORE2,STORE3
};

struct RS{
    struct RS_entry
    {
        uint32_t op;
        uint32_t Vj, Vk, Qj, Qk, A;
        bool busy;
    } rs[16];

    int find_empty_fp()
    {
        for (int i = 1; i <= 4; i ++ ) if (!rs[i].busy) return i;
        return 0;
    }
    int find_empty_ls()
    {
        for (int i = 5; i <= 10; i ++ ) if (!rs[i].busy) return i;
        return 0;
    }
};


struct CDB
{
    std::unordered_map<RSID, uint32_t> commondatabus;
    bool busy{0};
    void publish(RSID r, uint32_t data)
    {
        assert(!busy);
        commondatabus[r] = data;
    }

};

struct Hub // envolved across all stages and units
{
    CDB cdb;
    RStbl rs;
    Register regs;
};

class Issue
{
    Instruction inst;
    Hub *h;

    Issue(Hub *_h): h(_h) {}
    void go()
    {
        // fetch
        inst.frommemory = h->regs.load(pc, 4);
        pc += 4;

        // decode
        inst.type = inst.getType();
        inst.getArgs();

        // find an empty station;
        if (inst.FP_action)
        {
            int r = h->rs.find_empty_fp();
            // FP action
            if (regs->Qi[inst.rs1] == 0)
                execu->RStbl.rs[r].Qj = regs->Qi[inst.rs1]; 
            else { 
                execu->RStbl.rs[r].Vj = regs->get(inst.rs1); execu->RStbl[r].Qj = 0;
            }
            if (regs->Qi[inst.rs2] == 0)
                execu->RStbl[r].Qk = regs->Qi[inst.rs2];
            else { 
                execu->RStbl[r].Vk = regs->get(inst.rs2); execu->RStbl[r].Qk = 0;
            }
            execu->RStbl[r].busy = 1;
            regs->Qi[inst.rd] = r;
        }

        else if (inst.load || inst.store)
        {
            int r = execu->RStbl.find_empty_ls();
            // for load store
            if (regs->Qi[inst.rs1] == 0)
                execu->RStbl[r].Qj = regs->Qi[inst.rs1]; 
            else { 
                execu->RStbl[r].Vj = regs->get(inst.rs1); execu->RStbl[r].Qj = 0;
            }
            execu->RStbl[r].busy = 1;
            execu->RStbl[r].A = inst.imm;
            if (inst.load) regs->Qi[inst.rd] = r;
            if (inst.store)
            {
                if (regs->Qi[inst.rs2] == 0)
                    execu->RStbl[r].Qk = regs->Qi[inst.rs2]; 
                else { 
                    execu->RStbl[r].Vk = regs->get(inst.rs2); execu->RStbl[r].Qk = 0;
                }
            }
        }

    }
};


struct ALUUnit
{
    
    uint32_t setlowZero(uint32_t x)
    {
        if (x & 1) return x^1;
        else return x;
    }

    // fenceng?
    // for a typical r
    // FP
    void go()
    {
        for (int r = 1; r <= 4 ; r ++) if ()
            go(r);
    }
    void go(int r){
    if (rs[r].Qj == 0 && rs[r].Qk == 0)
    {
        // do use Vj,Vk;
        inst.src1 = Vj;
        inst.src2 = Vk;

        switch (inst.type){
            case ERROR: return;
            case LUI: inst.dest = inst.imm ; break;
            case AUIPC: 
                inst.dest = inst.addr + inst.imm; 
                break;
            case JAL: 
                regs->pc = inst.addr + inst.imm; // subtract 4 from IF stage
                inst.dest = inst.addr + 4;
                break;
            case JALR: 
                inst.dest = inst.addr + 4;
                regs->ctrUnit.jump_pc = inst.src1 + inst.imm; 
                regs->ctrUnit.jump_pc = setlowZero(regs->ctrUnit.jump_pc);
                regs->pc = regs->ctrUnit.jump_pc;

                break;

            case BEQ: inst.dest = (inst.src1 == inst.src2); break;
            case BNE: inst.dest = (inst.src1 != inst.src2); break;
            case BLT: inst.dest = (int)inst.src1 < (int)inst.src2; break;
            case BGE: inst.dest = (int)inst.src1 >= (int)inst.src2; break;
            case BLTU: inst.dest = inst.src1 < inst.src2; break;
            case BGEU: inst.dest = inst.src1 >= inst.src2; break;

            case ADDI: inst.dest = inst.src1 + inst.imm; break;
            case SLTI: inst.dest = ((int)inst.src1 < (int)inst.imm) ? 1:0; break;
            case SLTIU: inst.dest = (inst.src1 < inst.imm) ? 1:0; break;
            case XORI: inst.dest = inst.src1 ^ inst.imm; break;
            case ORI: inst.dest = inst.src1 | inst.imm; break;
            case ANDI: inst.dest = inst.src1 & inst.imm; break;

            case SLLI: inst.dest = inst.src1 << inst.imm; break;
            case SRLI: inst.dest = inst.src1 >> inst.imm; break;
            case SRAI: inst.dest = (inst.src1 >> inst.imm ) | (inst.src1 >> 31 << 31); break; // TODO
           
            case ADD: inst.dest = inst.src1 + inst.src2; break;
            case SUB: inst.dest = inst.src1 - inst.src2; break;
            case SLL: inst.dest = inst.src1 << inst.src2; break;
            case SLT: inst.dest = ((int)inst.src1 < (int)inst.src2) ? 1:0; break;
            case SLTU: inst.dest = (inst.src1 < inst.src2) ? 1:0; break;
            case XOR: inst.dest = (inst.src1 ^ inst.src2);
             break;
            case SRL: inst.dest = inst.src1 >> inst.src2; break;
            case SRA: inst.dest = (inst.src1 >> inst.src2) + (inst.src1 >> 31 << 31); break; 
            case OR: inst.dest = inst.src1 | inst.src2; break;
            case AND: inst.dest = inst.src1 & inst.src2; break;
        }

        if (inst.type == BNE || inst.type == BEQ || inst.type == BLT || inst.type == BGE ||
         inst.type == BLTU || inst.type == BGEU)
         {
            if (inst.dest)
            {
                regs->pc = inst.addr + inst.imm;
            }

         }

    }

    }
    // bch? 

  
};

struct LoadStoreUnit
{
    // load & store
    void loadstore(){
    if (rs[r].Qj == 0 && r is head)
        rs[r].A += rs[r].Vj;
    }
    void load2()
    {
    // load setp 2 MA

    }
};


struct Commit
{
    void go()
    {

    }

};



class Execu
{
    ALUUnit alu[4];
    LoadStoreUnit[6];

    void go()
    { 
        // alu 4 parallel
        // write to cdb
    }

};

class WriteBack
{
    // load FP
    if (r is done && !commondatabus.busy)
    {
        for each x
            if (reg.Qi[x] == r) reg[x] = res, res.Qi[x] = 0; // !!notice r != 0;
            if (rs[r].Qj == r) rs[r].Vj = res, rs[r].Qj = 0; // !!notice r != 0;
            if (rs[r].Qk == r) rs[r].Vk = res, rs[r].Qk = 0; // !!notice r != 0;

        rs[r].busy = 0;
    }
    // store
    store(rs[r].A, rs[r].Vk);
    rs[r].busy = 0;
};

class Tomasolu
{
    Hub h;
    memory *mem;
    Issue issue;
    Execu execu;
    WriteBack wb;
    Tomasolu(memory* _mem): mem(_mem) {}

    void run()
    {
        // fetch to inst_queue;
        std::cerr << "RUN TOMASOLU\n ";
        while (1)
        {
            if (h.regs._end) break;
            wb.go();
            execu.go();
            issue.go();
        }
    }
};



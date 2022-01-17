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

struct RS
{
    uint32_t op;
    uint32_t Vj, Vk, Qj, Qk, A;
    bool busy;
};
std::unordered_map<RSID, uint32_t> commondatabus;

class Issue
{
    Register* regs
    Instruction inst;
    Execu* execu;

    void go()
    {
        // fetch
        inst.frommemory = regs->load(pc, 4);
        pc += 4;

        // decode
        inst.type = inst.getType();
        inst.getArgs();

        // find an empty station;
        int r;
        // FP action
        if (regs->Qi[inst.rs1] | 0)
            execu->rs[r].Qj = regs->Qi[inst.rs1]; 
        else { 
            execu->rs[r].Vj = regs->get(inst.rs1); execu->rs[r].Qj = 0;
        }
        if (regs->Qi[inst.rs2] | 0)
            execu->rs[r].Qk = regs->Qi[inst.rs2]; 
        else { 
            execu->rs[r].Vk = regs->get(inst.rs2); execu->rs[r].Qk = 0;
        }
        execu->rs[r].busy = 1;
        regs->Q[inst.rd] = r;

        // for load store
        if (regs->Qi[inst.rs1] | 0)
            execu->rs[r].Qj = regs->Qi[inst.rs1]; 
        else { 
            execu->rs[r].Vj = regs->get(inst.rs1); execu->rs[r].Qj = 0;
        }
        execu->rs[r].busy = 1;
        execu->rs[r].A = inst.imm;
        if (inst.type == LW) regs->Q[inst.rd] = r;
        if (store)
        {
            if (regs->Qi[inst.rs2] | 0)
                execu->rs[r].Qk = regs->Qi[inst.rs2]; 
            else { 
                execu->rs[r].Vk = regs->get(inst.rs2); execu->rs[r].Qk = 0;
            }
        }
        // =====
    }
};


struct ALUUnit
{
    // fenceng?
    // for a typical r
    // FP
    if (rs[r].Qj == 0 && rs[r].Qk == 0)
    {
        // do use Vj,Vk;

    }
    // bch? 

    // load & store
    if (rs[r].Qj == 0 && r is head)
        rs[r].A += rs[r].Vj;

    // load setp 2


};

struct Commit
{
    void go()
    {

    }

};



class Execu
{
    RS rs[16];
    ALUUnit alu[4];
    LoadStoreUnit* loadStore;

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
            if (reg.Q[x] == r) reg[x] = res, res.Q[x] = 0; // !!notice r != 0;
            if (rs[r].Qj == r) rs[r].Vj = res, rs[r].Qj = 0; // !!notice r != 0;
            if (rs[r].Qk == r) rs[r].Vk = res, rs[r].Qk = 0; // !!notice r != 0;

        rs[r].busy = 0;
    }
    // store
    store(rs[r].A, rs[r].Vk);
    rs[r].busy = 0;
};

memory *mem;
class Tomasolu
{

    Execu execu;
    Issue issue;

    void run_tomasolu()
    {
        mem.loadFromDisk();
        // fetch to inst_queue;
        while (1)
        {
            if (1) break;
        }
    }

};



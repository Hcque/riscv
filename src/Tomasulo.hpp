#pragma once

#include <unordered_map>
#include <unordered_set>

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
    RS rst;
    Register regs; 
    Hub(memory *mem): regs(mem) {}
};

#define R_S h->rst.rs
#define Q_i h->regs.Qi
#define REGS h->regs

class Issue
{
    Instruction inst;
    Hub *h;

public:
    Issue(Hub *_h): h(_h) {}
    void go()
    {
        // fetch
        inst.fromMemory = h->regs.load(h->regs.pc, 4);
        h->regs.pc += 4;

        // decode
        inst.type = inst.getType();
        inst.getArgs();

        // find an empty station;
        if (inst.FP_action)
        {

            int r = h->rst.find_empty_fp();
            // FP action
            if (Q_i[inst.rs1] == 0)
                R_S[r].Qj = Q_i[inst.rs1]; 
            else { 
                R_S[r].Vj = REGS.get(inst.rs1); R_S[r].Qj = 0;
            }
            if (Q_i[inst.rs2] == 0)
                R_S[r].Qk = Q_i[inst.rs2];
            else { 
                R_S[r].Vk = REGS.get(inst.rs2); R_S[r].Qk = 0;
            }
            R_S[r].busy = 1;
            Q_i[inst.rd] = r;
        }

        else if (inst.load || inst.store)
        {
            int r = h->rst.find_empty_ls();
            // for load store
            if (Q_i[inst.rs1] == 0)
                R_S[r].Qj = Q_i[inst.rs1]; 
            else { 
                R_S[r].Vj = REGS.get(inst.rs1); R_S[r].Qj = 0;
            }
            R_S[r].busy = 1;
            R_S[r].A = inst.imm;
            if (inst.load) Q_i[inst.rd] = r;
            if (inst.store)
            {
                if (Q_i[inst.rs2] == 0)
                    R_S[r].Qk = Q_i[inst.rs2]; 
                else { 
                    R_S[r].Vk = REGS.get(inst.rs2); R_S[r].Qk = 0;
                }
            }
        }

    }
};


struct ALUUnit
{
    Hub *h;
    Instruction inst;

    ALUUnit(){}
    ALUUnit(Hub *_h): h(_h) {}

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
        for (int r = 1; r <= 4 ; r ++) 
            go(r);
    }
    void go(int r){
        if (R_S[r].Qj == 0 && R_S[r].Qk == 0)  // the condition to check
        {
            // do use Vj,Vk;
            inst.src1 = R_S[r].Vj;
            inst.src2 = R_S[r].Vk;

            switch (inst.type){
                case ERROR: return;
                case LUI: inst.dest = inst.imm ; break;
                case AUIPC: 
                    inst.dest = inst.addr + inst.imm; 
                    break;
                case JAL: 
                    // regs->pc = inst.addr + inst.imm; // subtract 4 from IF stage
                    inst.dest = inst.addr + 4;
                    break;
                case JALR: 
                    inst.dest = inst.addr + 4;
                    // regs->ctrUnit.jump_pc = inst.src1 + inst.imm; 
                    // regs->ctrUnit.jump_pc = setlowZero(regs->ctrUnit.jump_pc);
                    // regs->pc = regs->ctrUnit.jump_pc;
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

            // if (inst.type == BNE || inst.type == BEQ || inst.type == BLT || inst.type == BGE ||
            //  inst.type == BLTU || inst.type == BGEU)
            //  {
            //     if (inst.dest)
            //     {
            //         regs->pc = inst.addr + inst.imm;
            //     }

            //  }

        }

    }
    // bch? 

  
};

struct LoadStoreUnit
{
    Hub *h;
    Instruction inst;
    LoadStoreUnit(){}
    LoadStoreUnit(Hub *_h): h(_h) {}
    // load & store
    void loadstore(int r){
    if (R_S[r].Qj == 0 && 1) // r is head)
        R_S[r].A += R_S[r].Vj;
    }
    void load2()
    {
    // load setp 2 MA
      switch (inst.type){
            case ERROR: return;
            case LB: inst.dest = signext( REGS.load(inst.dest, 1), 7) ; break;
            case LH: inst.dest = signext( REGS.load(inst.dest, 2), 15) ; break;
            case LW: inst.dest = REGS.load(inst.dest, 4); break;

            case LBU: inst.dest = REGS.load(inst.dest, 1); break;
            case LHU: inst.dest = REGS.load(inst.dest, 2); break;

            case SB: REGS.store(inst.dest, 1, inst.src2); break;
            case SH: REGS.store(inst.dest, 2, inst.src2); break;
            case SW: REGS.store(inst.dest, 4, inst.src2); break;
        }

    }
    void go()
    {
        loadstore(1);
        load2();
    }
};



class Execu
{
#define NUM_ALU 4
#define NUM_LOAD 3
#define NUM_STORE 3
    Instruction inst;
    Hub *h;
    ALUUnit alu[NUM_ALU];
    LoadStoreUnit ldu[NUM_LOAD];
    LoadStoreUnit stu[NUM_STORE];
public:
    Execu(Hub* _h): h(_h){
        for (int i = 0; i < NUM_ALU; i ++ )
            alu[i] = ALUUnit(h);
        for (int i = 0; i < NUM_LOAD; i ++ )
            ldu[i] = LoadStoreUnit(h);
        for (int i = 0; i < NUM_STORE; i ++ )
            stu[i] = LoadStoreUnit(h);
    }

    void go()
    { 
        // alu 4 parallel
        for (auto& a: alu) a.go();
        for (auto& l: ldu) l.go();
        for (auto& s: stu) s.go();
    }

};

class WriteB
{
    Hub *h;
    Instruction inst;
    WriteB() {}
public:
    WriteB(Hub* _h): h(_h) {}
    void go()
    {
        // if (inst.FP_action || inst.load)
        // {
        //     // load FP
        //     if (r is done && !commondatabus.busy)
        //     {
        //         for each x
        //             if (reg.Qi[x] == r) reg[x] = res, res.Qi[x] = 0; // !!notice r != 0;
        //             if (R_S[r].Qj == r) R_S[r].Vj = res, R_S[r].Qj = 0; // !!notice r != 0;
        //             if (R_S[r].Qk == r) R_S[r].Vk = res, R_S[r].Qk = 0; // !!notice r != 0;

        //         R_S[r].busy = 0;
        //     }
        // }
        // else if (inst.store)
        // {
        //     // store
        //     store(R_S[r].A, R_S[r].Vk);
        //     R_S[r].busy = 0;
        // }

    }
    
};


struct Commit
{
    void go()
    {

    }
};

class Tomasolu
{
    Hub h;
    memory *mem;
    Issue issue;
    Execu execu;
    WriteB wb;
public:
    Tomasolu(memory* _mem):h(_mem), mem(_mem), issue(&h), execu(&h), wb(&h) {}
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



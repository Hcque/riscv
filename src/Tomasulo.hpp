 // TODO: bch stall and pred
 // in RS , things happened not expected
 // LUI not FP : the basic flow
 // 
// if (Q_i[inst.rs1] != 0)  when not 0 , means busy, means waiting
// the meaninig of the baisc flow

// imm goes to A of RS

// stall of first JAL

// ? how to use 4 alu unit?
// busy has changed : write back done;

// lw free busy 1.22

// rs viewed ad queue, alu search for back to front,
// when rs is zero  how to do?

//  write to reg NOT SHOW! 

// line 213 , has full rs
// if cdb execu should do it ! line 455

// inst with one reg

// first BNE DONE of regs, but still not stop, jump problem

// store forget busy

// still regs, not equal in first BNE, bad for which place

// !done condition for execu,
// not for one time for each clock

// store no RD , write bc chaged , execu should read, not 

#pragma once
#include <unordered_map>
#include <unordered_set>
#include <queue>

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
        // uint32_t op;
        Inst_Type type{ERROR};
        // Instruction inst;
        bool BXX{0};
        uint32_t addr{0};
        uint32_t rd{0};
        bool load{0}, store{0}, FP_action{0};
        bool done{0};
        uint32_t dest{0};
        uint32_t Vj, Vk, Qj, Qk, A;
        bool busy{0};

    friend ostream& operator<< (ostream& out, const RS_entry& _RS) 
    {
        out << " \n";
        out << "TYPE:" <<  _type_Name[_RS.type] << " ";
        out << "addr:" <<  _RS.addr << " ";
        out << "busy:" <<  _RS.busy << " ";
        out << "Qj:" <<  _RS.Qj << " ";
        out << "Qk:" <<  _RS.Qk << " ";

        out << "Vj:" <<  _RS.Vj << " ";
        out << "Vk:" <<  _RS.Vk << " ";

        out << "Rd:" <<  _RS.rd << " ";
        out << "done:" <<  _RS.done << " \n";
        return out;
    }
    // std::queue<int> active_list;

    } rs[16]; // 1,2,3,4 for FP
        // 5,6,7 for load buffer 
        // 8,9 10 for store buffer
    
    friend ostream& operator<< (ostream& out, const RS& _RS) 
    {
        out << "=======FP ========= \n";
        for (int i = 1; i <= 4; i ++ ){
            out << i << _RS.rs[i];
        }
        // out << "=======LOAD ========= \n";
        // for (int i = 5; i <= 7; i ++ ){
        //     out << i << _RS.rs[i];
        // }
        out << "======= SW ========= \n";
        for (int i = 8; i <= 10; i ++ ){
            out << i << _RS.rs[i];
        }
        return out;
    }

    int find_empty_fp()
    {
        for (int i = 1; i <= 4; i ++ ) if (!rs[i].busy) return i;
        return 0;
    }
    int find_empty_ld()
    {
        for (int i = 5; i <= 7; i ++ ) if (!rs[i].busy) return i;
        return 0;
    }
    int find_empty_sw()
    {
        for (int i = 8; i <= 10; i ++ ) if (!rs[i].busy) return i;
        return 0;
    }
};

struct CDB // bottleneck
{
    // std::unordered_map<RSID, uint32_t> commondatabus;
    // std::unordered_set<int> commondatabus;
    std::queue<int> commondatabus;
    bool busy{0};
    // bool active{0};
    // void publish(RSID r, uint32_t data)
    // {
    //     assert(!busy);
    //     commondatabus[r] = data;
    // }
};

// for commit and speculation, else the out of ordershould stall when encounter
// jump, which comsume a lot clock
// strategy 1: stall when encounter jump
// stragegy 2: speculation and ROB & commit unit

struct ROB 
{
    struct ROB_entry
    {
        bool ready{0};
        bool busy{0};
        uint32_t dest{0};
    };
};

struct Hub // envolved across all stages and units
{
    bool stall_signal{0};
    bool RS_stall_signal{0};
    CDB cdb;
    ROB rob;
    RS rst;
    Register regs; 
    uint32_t nextpc{0};
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
        assert(inst.type == ERROR);
        // std::cout << h->regs.pc << "|pc|\n";

        inst.fromMemory = h->regs.load(h->regs.pc, 4);
        inst.addr = h->regs.pc;
        h->regs.pc += 4;

        // decode
        inst.type = inst.getType();
        inst.getArgs();
        if (inst.BXX || inst.type == AUIPC || inst.type == JALR || inst.type == JAL)
        {
            h->stall_signal = 1;
        }
        std::cout << inst;
        // std::cout << "REGS VALUE: " << REGS.get(inst.rs1) << "|" << REGS.get(inst.rs2) << "\n";

        // find an empty station;
        if (inst.FP_action)
        {
            int r = h->rst.find_empty_fp();
            assert(r != 0); // ASSERT // streategy , always find the r because any of "FULL"
            // FP action

            if (Q_i[inst.rs1] != 0)  // always busy, when not 0 , means busy, means waiting
                R_S[r].Qj = Q_i[inst.rs1]; 
            else { 
                R_S[r].Vj = REGS.get(inst.rs1); R_S[r].Qj = 0;
            }
            if (Q_i[inst.rs2] != 0)
                R_S[r].Qk = Q_i[inst.rs2];
            else { 
                R_S[r].Vk = REGS.get(inst.rs2); R_S[r].Qk = 0;
            }
            R_S[r].busy = 1;

            R_S[r].load = inst.load;
            R_S[r].FP_action = inst.FP_action;
            R_S[r].store = inst.store;
            R_S[r].addr = inst.addr;
            R_S[r].rd = inst.rd;
            R_S[r].BXX = inst.BXX;
            R_S[r].type = inst.type;

            R_S[r].A = inst.imm;
            REGS.setQi(inst.rd, r);
            assert(R_S[r].done == 0);
        }

        else if (inst.load || inst.store)
        {
            int r = inst.load ? h->rst.find_empty_ld() : h->rst.find_empty_sw();
            assert(r != 0);

            // for load store
            if (Q_i[inst.rs1] != 0)
                R_S[r].Qj = Q_i[inst.rs1]; 
            else { 
                R_S[r].Vj = REGS.get(inst.rs1); R_S[r].Qj = 0;
            }
            R_S[r].busy = 1;
            R_S[r].A = inst.imm;

            if (inst.load) REGS.setQi(inst.rd, r);
            if (inst.store)
            {
                if (Q_i[inst.rs2] != 0)
                    R_S[r].Qk = Q_i[inst.rs2]; 
                else { 
                    R_S[r].Vk = REGS.get(inst.rs2); R_S[r].Qk = 0;
                }
            }
            R_S[r].type = inst.type;
            R_S[r].load = inst.load;
            R_S[r].FP_action = inst.FP_action;
            R_S[r].store = inst.store;
            R_S[r].addr = inst.addr;
            R_S[r].rd = inst.rd;
            R_S[r].BXX = inst.BXX;

        }
        inst.clear();

    }
};


struct ALUUnit
{
    Hub *h;

    Instruction inst;
    ALUUnit(){}
    ALUUnit(Hub *_h): h(_h){}

    uint32_t setlowZero(uint32_t x)
    {
        if (x & 1) return x^1;
        else return x;
    }

    // for a typical r
    // FP
    void go()
    {
        for (int r = 4; r >= 1 ; r --) 
        {
            if (R_S[r].busy && !R_S[r].done)
            {
                assert(R_S[r].done == 0);
                go(r);
            }
        }
    }
    void go(int r){
        // Qj, Qk 0 means not waiting mode
        
        if (R_S[r].busy && R_S[r].Qj == 0 && R_S[r].Qk == 0)  // the condition to check
        {
            // do use Vj,Vk;
            inst.src1 = R_S[r].Vj;
            inst.src2 = R_S[r].Vk;
            inst.type = R_S[r].type;

            switch (inst.type){
                case ERROR: return;
                case LUI: inst.dest = R_S[r].A;
                    break;
                case AUIPC: 
                    inst.dest = R_S[r].addr + R_S[r].A; 
                    break;
                case JAL: 
                    h->nextpc = R_S[r].addr + R_S[r].A; // subtract 4 from IF stage
                    inst.dest = R_S[r].addr + 4;
                    break;
                case JALR: 
                    inst.dest = R_S[r].addr + 4;
                    h->nextpc = inst.src1 + R_S[r].A; 
                    h->nextpc = setlowZero(h->nextpc);
                    break;

                case BEQ: inst.dest = (inst.src1 == inst.src2); break;
                case BNE: inst.dest = (inst.src1 != inst.src2); break;
                case BLT: inst.dest = (int)inst.src1 < (int)inst.src2; break;
                case BGE: inst.dest = (int)inst.src1 >= (int)inst.src2; break;
                case BLTU: inst.dest = inst.src1 < inst.src2; break;
                case BGEU: inst.dest = inst.src1 >= inst.src2; break;

                case ADDI: inst.dest = inst.src1 + R_S[r].A; break;
                case SLTI: inst.dest = ((int)inst.src1 < (int)R_S[r].A) ? 1:0; break;
                case SLTIU: inst.dest = (inst.src1 < R_S[r].A) ? 1:0; break;
                case XORI: inst.dest = inst.src1 ^ R_S[r].A; break;
                case ORI: inst.dest = inst.src1 | R_S[r].A; break;
                case ANDI: inst.dest = inst.src1 & R_S[r].A; break;

                case SLLI: inst.dest = inst.src1 << R_S[r].A; break;
                case SRLI: inst.dest = inst.src1 >> R_S[r].A; break;
                case SRAI: inst.dest = (inst.src1 >> R_S[r].A ) | (inst.src1 >> 31 << 31); break; // TODO
            
                case ADD: inst.dest = inst.src1 + inst.src2; break;
                case SUB: inst.dest = inst.src1 - inst.src2; break;
                case SLL: inst.dest = inst.src1 << inst.src2; break;
                case SLT: inst.dest = ((int)inst.src1 < (int)inst.src2) ? 1:0; break;
                case SLTU: inst.dest = (inst.src1 < inst.src2) ? 1:0; break;
                case XOR: inst.dest = (inst.src1 ^ inst.src2); break;
                case SRL: inst.dest = inst.src1 >> inst.src2; break;
                case SRA: inst.dest = (inst.src1 >> inst.src2) + (inst.src1 >> 31 << 31); break; 
                case OR: inst.dest = inst.src1 | inst.src2; break;
                case AND: inst.dest = inst.src1 & inst.src2; break;
            }
            if (inst.type == BNE || inst.type == BEQ || inst.type == BLT || inst.type == BGE ||
             inst.type == BLTU || inst.type == BGEU)
             {
                if (inst.dest)
                    h->nextpc = R_S[r].addr + R_S[r].A;
             }
            R_S[r].done = 1;
            R_S[r].dest = inst.dest;
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
    LoadStoreUnit(Hub *_h, Instruction &inst): h(_h), inst(inst) {}
    // load & store
    void loadstore(int r){
            // assert(h->stall_signal == 0 || R_S[r].type == ERROR);
    if (R_S[r].busy && R_S[r].Qj == 0 ) // r is head)
    
        inst.dest = R_S[r].A = R_S[r].A + R_S[r].Vj;
    }
    void load2(int r)
    {
    // load setp 2 MA
    if (R_S[r].busy){
    inst.type = R_S[r].type;
      switch (inst.type){
            case ERROR: return;
            case LB: inst.dest = signext( REGS.load(inst.dest, 1), 7) ; break;
            case LH: inst.dest = signext( REGS.load(inst.dest, 2), 15) ; break;
            case LW: inst.dest = REGS.load(inst.dest, 4); break;

            case LBU: inst.dest = REGS.load(inst.dest, 1); break;
            case LHU: inst.dest = REGS.load(inst.dest, 2); break;

            // case SB: REGS.store(inst.dest, 1, inst.src2); break;
            // case SH: REGS.store(inst.dest, 2, inst.src2); break;
            // case SW: REGS.store(inst.dest, 4, inst.src2); break;
        }
        R_S[r].done = 1;
        R_S[r].dest = inst.dest;
    }
    }
    void go(int r)
    {
        loadstore(r);
        load2(r);
    }

    void go()
    {
        for (int r = 10 ; r >= 5; r -- )
        {
            if (R_S[r].busy && R_S[r].Qj == 0 && !R_S[r].done){
                go(r);
                break;
            }
        }
    }
};

class Execu
{
#define NUM_ALU 1
#define NUM_LOAD 1
#define NUM_STORE 1
    Hub *h;
    ALUUnit alu[NUM_ALU];
    LoadStoreUnit ldu[NUM_LOAD];
    LoadStoreUnit stu[NUM_STORE];
public:
    Execu() {}
    Execu(Hub* _h): h(_h){
        // assert(h == _h);
        for (int i = 0; i < NUM_ALU; i ++ )
            alu[i] = ALUUnit(_h);
        for (int i = 0; i < NUM_LOAD; i ++ )
            ldu[i] = LoadStoreUnit(_h);
        for (int i = 0; i < NUM_STORE; i ++ )
            stu[i] = LoadStoreUnit(_h);
    }

    void go()
    { 
        // alu 4 parallel
        // std::cout << h->rst;
        // std::cout << h->regs;
        for (auto& a: alu) 
        {
            if (h->cdb.commondatabus.size()) 
            {
                int ri = h->cdb.commondatabus.front(); 
                if (ri >= 1 && ri <= 4) 
                {
                    a.go(ri);
                    h->cdb.commondatabus.pop();
                }
            }
            else
                a.go();
        }
        for (auto& l: ldu) 
        {
            if (h->cdb.commondatabus.size()) 
            {
                int ri = h->cdb.commondatabus.front(); 
                if (ri >= 5 && ri <= 7)
                {
                    l.go(ri);
                    h->cdb.commondatabus.pop();
                }
            }
            else
                l.go();
        }
        for (auto& s: stu) 
        {
            if (h->cdb.commondatabus.size()) 
            {
                int ri = h->cdb.commondatabus.front(); 
                if (ri >= 8 && ri <= 10) 
                {
                    s.go(ri);
                    h->cdb.commondatabus.pop();
                }
            }
            else
                s.go();
        }
        // std::cout << h->rst;
        // std::cout << h->regs;
    }
};

class WriteB
{
public:
    Hub *h;
    WriteB(Hub* _h): h(_h) {}
    void go()
    {
        for (int r = 1; r <= 11; r ++ ) go(r);
    }
    void go(int r)
    {
        if (R_S[r].busy && R_S[r].done && (R_S[r].FP_action || R_S[r].load))
        {
            // load FP
            if (!h->cdb.busy)
            {
                h->cdb.busy = 1;
                for (int x = 1; x <= 31; x ++) if (Q_i[x] == r) {
                     REGS.reg[x] = R_S[r].dest, Q_i[x] = 0; // !!notice r != 0;
                    //  std::cout << "CHANGE QI" << x <<  r << "\n";
                }
                for (int ri = 1; ri <= 10; ri ++ ) if (R_S[ri].busy ){
                    if (R_S[ri].Qj == r) 
                    {
                        R_S[ri].Vj = R_S[r].dest, R_S[ri].Qj = 0; // !!notice r != 0;
                        h->cdb.commondatabus.push(ri);
                        // std::cout << "WB Q & V:" << r << "\n"; // if cdb trigger execu
                    }
                    if (R_S[ri].Qk == r) 
                    {
                        R_S[ri].Vk = R_S[r].dest, R_S[ri].Qk = 0; // !!notice r != 0;
                        h->cdb.commondatabus.push(ri);
                        // std::cout << "WB Q & V:" << r << "\n";
                    }
                }
                R_S[r].busy = 0;
                h->cdb.busy = 0;

                // write file to regs
                REGS.set(R_S[r].rd, R_S[r].dest); // write back

                // deal with branches && jumps
                if (R_S[r].BXX || R_S[r].type == JAL || R_S[r].type == JALR || R_S[r].type == AUIPC) 
                {
                    h->stall_signal = 0;
                    // std::cout << "RESET SIGNAL " << REGS.pc << "\n";

                    REGS.pc = h->nextpc;
                    if (! R_S[r].dest && R_S[r].BXX ) 
                        REGS.pc = R_S[r].addr + 4;
                }
                R_S[r].done = 0;
            }
        }
        else if (R_S[r].busy && R_S[r].done && R_S[r].store) // not done
        {
            // store
            // REGS.store(R_S[r].A, R_S[r].Vk);

            switch (R_S[r].type){
                    case ERROR: return;
                    case SB: REGS.store(R_S[r].A, 1, R_S[r].Vk); break;
                    case SH: REGS.store(R_S[r].A, 2, R_S[r].Vk); break;
                    case SW: REGS.store(R_S[r].A, 4, R_S[r].Vk); break;
            }
            R_S[r].busy = 0;
            R_S[r].done = 0;
        }
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

    int output(){
        return h.regs.reg[10] & 255u;
    }

    void run()
    {
        std::cout << "RUN TOMASOLU\n ";
        int cc = 0;

        while (1)
        {
            std::cout << "cc" << cc ;
            if (h.regs._end) break;
            // if (cc > 80) break;
            wb.go();
            execu.go();

            // RS is not full check
            if (h.rst.find_empty_fp() == 0 || h.rst.find_empty_ld() == 0 || h.rst.find_empty_sw() == 0 )
            {
                h.RS_stall_signal = 1;
            }
            else h.RS_stall_signal = 0;

            if (!h.stall_signal && h.cdb.commondatabus.size() == 0 && h.RS_stall_signal == 0) 
            {
                // std::cout << "ISSUE NOT STALL\n";
                issue.go();
            }
            cc++;
        }
    }
};



// set not reg 0

#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <cstdint>
#include <cassert>
#include <iostream>
#include <unordered_set>

#include "Instruction.hpp"
#include "prediction.hpp"

using namespace std;


struct ControlUnit
{
    bool stall;
    uint32_t stall_pc;
    uint32_t jump_pc;

    // branch prediction    
    bool bch_taken;
    // StaticPred pd;
    // TwobitPred pd;
    // AdptiveTrain pd;
    AdptiveTrain2 pd;


    ControlUnit() : pd(), stall(0), stall_pc(0), jump_pc(0), bch_taken(0) {}

    void Stall()
    {
        
    }

    friend ostream& operator<< (ostream& out, const ControlUnit& cu)
    {
        out << " === CONTRAL UNIT === \n" 
        << "| stall:" << cu.stall
        << "| stall_pc:" << cu.stall_pc
        << "| jump_pc:" << cu.jump_pc
        << "| bch_taken:" << cu.bch_taken
        << "\n";
        return out;
    }

};


class Register{
    
public:
    uint32_t reg[32]{0}; // reg x0 - x31
    uint32_t Qi[32]{0};
    uint32_t Q[32]{0};
    uint32_t pc{0};
    uint32_t savedpc{0};
    uint32_t _end{0};
    memory* mem;

    ControlUnit ctrUnit;

    std::unordered_set<Inst_Type > _types;

    Register(){}
    Register(memory* _mem): mem(_mem), _end(0),pc(0u){}

    uint32_t load(uint pos, uint p){
        uint32_t res = 0; unsigned char tmp[4];
        for (int i=0;i<p;i++) 
        {
            tmp[i] = mem->data[pos+i];
            // cout << (uint32_t) mem->data[4];
        }
        for (int i = p-1;i>=0;i--)
            res = (res << 8) + tmp[i] ; 
        // std::cerr << res << endl;
        return res;
    }

    void store(uint pos, uint p, uint item){
        if (pos == 0x30004)
        {
            // assert(0);
            _end = 1;
            // std::cout << "END\n";
        }
        for (int i=0;i<p;i++){
            mem->data[pos+i] = (item >> 8*i) & 255;
        }
    }

    uint32_t get(int i){
        if (i == 0) return 0u;
        return reg[i];
    }
    void set(int i, uint item){
        if (i == 0) return;
        reg[i] = item;
    }


    friend std::ostream& operator<<(std::ostream& out, const Register& Reg)
    {
        for (int i = 0; i < 32; i ++ )
        {
            out << "|" <<i << ":" << Reg.Qi[i] << "|    ";
        }
        out << "pc:" << Reg.pc;
        out << "\n";
        return out;
    }



};

#endif
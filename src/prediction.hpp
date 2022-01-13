
#pragma once

#include <queue>
#include <unordered_map>
#include <cassert>


// IF ID EX
//         ^ validate
//      ^ pred
// 


class StaticPred{
public:
    int tot, hit;
    int pred_taken;
    StaticPred(): tot(0), hit(0), pred_taken(0) {}

    // always not taken
    // notice this take should save 
    // the prediction  in "pred_taken" , using in RISCV.hpp
    bool take(uint32_t addr = 0u)   
    {
        // pred_taken = 0;
        return 0;
    }

    void validate(bool jump)
    {
        tot++;
        if (!jump) hit ++;
    }
    void clear()
    {
        if (tot == 0) tot = hit = 1;
        std::cerr << "accuacy rate:" <<  hit * 100.0f / tot << "% ";
        tot = hit = 0;
    }

};

class TwobitPred: public StaticPred
{
public:
    bool b0, b1;
    // int tot, hit;
    bool pred;
    TwobitPred(): b0(1), b1(0){}

    void increase()
    {
        if (b1 && b0) return;
        else if ( !b1 && b0) b1 = 1;
        else if (b1 && !b0) b1 = 0, b0 = 1;
        else if (!b1 && !b0) b1 = 1, b0 = 0;
        else assert(0);
    }
    void decrease()
    {
        if (b1 && b0) b1 = 0;
        else if ( !b1 && b0) b1 = 1, b0 = 0;
        else if (b1 && !b0) b1 = 0;
        else if (!b1 && !b0) return;
        else assert(0);
    }

    bool take(uint32_t addr = 0u)   
    {
        pred_taken = b0;
        return b0;
    }

    void validate(bool jump)
    {
        tot++;
        if (jump == b0)  // pred correct
        {
            hit ++;
        }
        if (jump) increase();
        else decrease();
    }

};

// for the whole code texts, but combine with global history imformation,
// not just local TWO BIT.
class AdptiveTrain : public StaticPred
{
    unsigned char HR; // history register / only 8 bit pasts ,2^8 possible cases.
    TwobitPred patternTable[1<<8];
public:
    AdptiveTrain(): HR(0u) {}

    bool take(uint32_t addr = 0u)   
    {
        pred_taken = patternTable[HR].b0;
        return patternTable[HR].take();
    }

    void validate(bool jump)
    {
        tot++;
        if (jump == patternTable[HR].b0) hit++;

        patternTable[HR].validate(jump);
        HR = (HR << 1) + jump;
    }

    void clear()
    {
        if (tot == 0) tot = hit = 1;
        std::cerr << "accuacy rate:" <<  hit * 100.0f / tot << "% " ;
        tot = hit = 0;
        HR = 0u;
    }


};

class AdptiveTrain2: public StaticPred
{
    std::unordered_map<uint32_t, AdptiveTrain> addrTable; // only store used addr
    uint32_t addr;
public:
    bool take(uint32_t addr)   
    {
        this->addr = addr;
        if (addrTable.count(addr) == 0)
        {
            addrTable[addr] = AdptiveTrain();
        }
        
        addrTable[addr].take();
        pred_taken = addrTable[addr].pred_taken;
        return pred_taken;
    }

    void validate(bool jump)
    {
        tot++;
        if (jump == pred_taken) hit++;

        addrTable[addr].validate(jump);
    }

    void clear()
    {
        if (tot == 0) tot = hit = 1;
        std::cerr << "accuacy rate:" <<  hit * 100.0f / tot << "% " ;
        tot = hit = 0;
        addrTable.clear();
    }

};

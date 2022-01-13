
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
    bool take()   
    {
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
        std::cerr << "accuacy rate:" <<  hit * 100.0f / tot << " %\n" ;
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

    bool take()   
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


class AdptiveTrain // 
{
    bool b0, b1;
    bool take()   
    {
        return 0;
    }

    void validate(bool jump)
    {

    }

};

class Prediction
{
    std::unordered_map<uint32_t, TwobitPred> strmap;
    std::queue<bool> predictPool;

    bool take()
    {
        return 0;
    }


};

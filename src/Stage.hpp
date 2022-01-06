#ifndef STAGE_H
#define STAGE_H

#include "register.hpp"
#include "memory.hpp"
#include "Instruction.hpp"
#include "prediction.hpp"

class Stage{
public:

    Register *regs;
    StaticPred* pd;
    bool stall;

    Instruction inst;


    Stage(){}
    Stage(Register *_regs) : regs(_regs) {}
    Stage(Register *_regs, StaticPred* _pd) : regs(_regs), pd(_pd) {}
    Stage(Register *_regs, StaticPred* _pd, bool _st) : regs(_regs), pd(_pd) , stall(_st){}


    virtual void go() {};
    virtual void pass() {};
};

#endif
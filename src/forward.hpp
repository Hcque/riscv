

#pragma once

#include "Common.hpp"

void forward(MemoryAccess& ma, Execution& ex)
{

    // if (ex.inst.rd !=0 && ex.inst.rs1 == id.inst.rs1)
    // ma.inst.src1 = ...;
    // ma.inst.src2 = ...;

}

void forward(MemoryAccess& ma, InstructionDecode& id)
{

}


void forward(Execution& ex, InstructionDecode& id)
{
    if (ex.inst.rd !=0 && ex.inst.rd == id.inst.rs1) id.regs->set( ex.regs->get( ex.inst.rd ) );
    if (ex.inst.rd !=0 && ex.inst.rd == id.inst.rs2) id.regs->set( ex.regs->get( ex.inst.rd ) );
}


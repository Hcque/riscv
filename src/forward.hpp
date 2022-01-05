

#pragma once

#include "Common.hpp"

void forward(MemoryAccess& ma, Execution& ex)
{

  switch (ma.inst.type){
            case LUI:
            case AUIPC:
            case JAL:
            case JALR:

            case ADDI:
            case SLTI:
            case SLTIU:
            case XORI:
            case ORI:
            case ANDI:

            case SLLI:
            case SRLI:
            case SRAI:

            case ADD:
            case SUB:
            case SLL:
            case SLT:
            case SLTU:
            case XOR:
            case SRL:
            case SRA:
            case OR:
            case AND:

            case LB: 
            case LH: 
            case LW: 
            case LBU: 
            case LHU:

        if (ma.inst.rd !=0 && ma.inst.rd == ex.inst.rs1) ex.regs->set(ma.inst.rd, ma.regs->get( ma.inst.rd ) );
    if (ma.inst.rd !=0 && ma.inst.rd == ex.inst.rs2) ex.regs->set( ma.inst.rd, ma.regs->get( ma.inst.rd ) );

            break;
        }

}



void forward(MemoryAccess& ma, InstructionDecode& id)
{
      switch (ma.inst.type){
            case LUI:
            case AUIPC:
            case JAL:
            case JALR:

            case ADDI:
            case SLTI:
            case SLTIU:
            case XORI:
            case ORI:
            case ANDI:

            case SLLI:
            case SRLI:
            case SRAI:

            case ADD:
            case SUB:
            case SLL:
            case SLT:
            case SLTU:
            case XOR:
            case SRL:
            case SRA:
            case OR:
            case AND:

            case LB: 
            case LH: 
            case LW: 
            case LBU: 
            case LHU:

        if (ma.inst.rd !=0 && ma.inst.rd == id.inst.rs1) id.regs->set( ma.inst.rd, ma.regs->get( ma.inst.rd ) );
    if (ma.inst.rd !=0 && ma.inst.rd == id.inst.rs2) id.regs->set(ma.inst.rd, ma.regs->get( ma.inst.rd ) );

            break;
        }

}


void forward(Execution& ex, InstructionDecode& id)
{
      switch (ex.inst.type){
            case LUI:
            case AUIPC:
            case JAL:
            case JALR:

            case ADDI:
            case SLTI:
            case SLTIU:
            case XORI:
            case ORI:
            case ANDI:

            case SLLI:
            case SRLI:
            case SRAI:

            case ADD:
            case SUB:
            case SLL:
            case SLT:
            case SLTU:
            case XOR:
            case SRL:
            case SRA:
            case OR:
            case AND:

    if (ex.inst.rd !=0 && ex.inst.rd == id.inst.rs1) id.regs->set( ex.inst.rd, ex.regs->get( ex.inst.rd ) );
    if (ex.inst.rd !=0 && ex.inst.rd == id.inst.rs2) id.regs->set( ex.inst.rd, ex.regs->get( ex.inst.rd ) );
    break;
      }

}


// ADDI SW SW SW 
//          ^ wrong


#pragma once

#include "Common.hpp"


void forward(MemoryAccess& ma, InstructionDecode& id, Execution& ex)
{
    // bool exforwardid_src1 = ( ex.inst.rd !=0 && ex.inst.rd == id.inst.rs1 && ex.inst.regWrite ) ;
    // bool exforwardid_src2 = ( ex.inst.rd !=0 && ex.inst.rd == id.inst.rs2 && ex.inst.regWrite ) ;
    // if (exforwardid_src1 || exforwardid_src2) std::cerr << "ma->id USE EX!\n";

// std::cout << "==forward=====\n ";
//     std::cout << ma.inst << id.inst;
// std::cout << "===forward====\n ";
      if (ma.inst.regWrite){
            
        if ( ma.inst.rd !=0 && ma.inst.rd == id.inst.rs1) 
        {
        std::cout << "ma->id1\n ";
            id.inst.src1 = ma.inst.dest;
        }
        if ( ma.inst.rd !=0 && ma.inst.rd == id.inst.rs2)
        {
        std::cout << "ma->id2\n ";
            id.inst.src2 = ma.inst.dest;
        }
    }

}


void forward(Execution& ex, InstructionDecode& id)
{
     
    if (ex.inst.regWrite){

    if (ex.inst.rd !=0 && ex.inst.rd == id.inst.rs1) 
    {
    std::cout << "ex->id1\n ";
        id.inst.src1 = ex.inst.dest;
    }
    if (ex.inst.rd !=0 && ex.inst.rd == id.inst.rs2)
    {
    std::cout << "ex->id2\n ";
        id.inst.src2 = ex.inst.dest;
    }

    }

}


void forward(WriteBack& wb, InstructionDecode& id)
{
     
    if (wb.inst.regWrite){

    if (wb.inst.rd !=0 && wb.inst.rd == id.inst.rs1) 
    {
    std::cout << "wb->id1\n ";
        id.inst.src1 = wb.inst.dest;
    }
    if (wb.inst.rd !=0 && wb.inst.rd == id.inst.rs2)
    {
    std::cout << "wb->id2\n ";
        id.inst.src2 = wb.inst.dest;
    }

    }

}


void forward(MemoryAccess& ma, InstructionDecode& id)
{
     
    if (ma.inst.regWrite){

    if (ma.inst.rd !=0 && ma.inst.rd == id.inst.rs1) 
    {
    std::cout << "ma->id1\n ";
        id.inst.src1 = ma.inst.dest;
    }
    if (ma.inst.rd !=0 && ma.inst.rd == id.inst.rs2)
    {
    std::cout << "ma->id2\n ";
        id.inst.src2 = ma.inst.dest;
    }

    }

}


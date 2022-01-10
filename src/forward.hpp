

#pragma once

#include "Common.hpp"


void forward(MemoryAccess& ma, InstructionDecode& id, Execution& ex)
{
    bool exforwardid_src1 = ( ex.inst.rd !=0 && ex.inst.rd == id.inst.rs1 ) ;
    bool exforwardid_src2 = ( ex.inst.rd !=0 && ex.inst.rd == id.inst.rs2 ) ;
    // if (exforwardid_src1 || exforwardid_src2) std::cerr << "ma->id USE EX!\n";

      if (ma.inst.regWrite){
            
            
        if (! exforwardid_src1 && ma.inst.rd !=0 && ma.inst.rd == id.inst.rs1) 
        {
        std::cerr << "ma->id1\n ";
            id.inst.src1 = ma.inst.dest;
        }
        if (! exforwardid_src2 && ma.inst.rd !=0 && ma.inst.rd == id.inst.rs2)
        {
        std::cerr << "ma->id2\n ";
            id.inst.src2 = ma.inst.dest;
        }
    }

}


void forward(Execution& ex, InstructionDecode& id)
{
     
    if (ex.inst.regWrite){

    if (ex.inst.rd !=0 && ex.inst.rd == id.inst.rs1) 
    {
    std::cerr << "ex->id1\n ";
        id.inst.src1 = ex.inst.dest;
    }
    if (ex.inst.rd !=0 && ex.inst.rd == id.inst.rs2)
    {
    std::cerr << "ex->id2\n ";
        id.inst.src2 = ex.inst.dest;
    }

    }

}


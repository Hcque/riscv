#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdint>
#include <iostream>
#include <cassert>

using namespace std;

enum Inst_Type { ERROR, LUI, AUIPC, JAL, JALR, BEQ, BNE, BLT, BGE, BLTU, BGEU,
                LB, LH, LW, LBU, LHU, SB, SH, SW, 
                ADDI, SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI, 
                ADD, SUB,  SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND };

typedef uint32_t uint;

class Instruction{
public:
    uint32_t imm, rs1, rs2, rd, opcode, func7, func3;
    uint fromMemory;
    Inst_Type type;    

    uint src1, src2;
    uint dest;

    void init(){
        imm = rs1 = rs2 = rd = opcode = func7 = func3 = 0;

    }

    Inst_Type getType(){
        opcode = fromMemory & 127;
        func7 = (fromMemory >> 12) & (7);
        func3 = (fromMemory >> 25) & (127);
        cout  << opcode   << endl;
        
        switch (opcode){
            case 55: type = LUI; break;
            case 23: type = AUIPC; break;
            case 111: type = JAL; break;
            case 103: type = JALR; break;
            case 99: 
                switch (func3){
                    case 0: type = BEQ; break;
                    case 1: type = BNE; break;
                    case 4: type = BLT; break;
                    case 5: type = BGE; break;
                    case 6: type = BLTU; break;
                    case 7: type = BGEU; break;
                }

            case 3:
                switch (func3){
                    case 0: type = LB; break;
                    case 1: type = LH; break;
                    case 2: type = LW; break;
                    case 4: type = LBU; break;
                    case 5: type = LHU; break;
                }
            
            case 35:
                switch (func3){
                    case 0: type = SB; break;
                    case 1: type = SH; break;
                    case 2: type = SW; break;
                }
            
            case 19:
                switch (func3){
                    case 0: type = ADDI; break;
                    case 2: type = SLTI; break;
                    case 3: type = SLTIU; break;
                    case 4: type = XORI; break;
                    case 6: type = ORI; break;
                    case 7: type = ANDI; break;
                    case 1: type = SLLI; break;
                    case 5:  
                        if (func3 == 0) type = SRLI; 
                        else if(func3 == 32) type = SRAI;
                        break;
                }
            case 51:
                switch (func3) {
                    case 0: 
                        if (func7 == 0) type = ADD;
                        else if (func7 == 32) type = SUB;
                        break;
                    case 1: type = SLL; break;
                    case 2: type = SLT; break;
                    case 3: type = SLTU; break;
                    case 4: type = XOR; break;
                    case 5: type = SRL; break;
                    case 6: type = OR; break;
                    case 7: type = AND; break;
                }
            default: type = ERROR;
        }
        return type;
    }

    static uint signext(uint x, int n){
        if ( x >> (n-1) ) x |= (0xffffffff >> n) << n;
        return x;
    }

    void getArgs(){
        uint tmp1, tmp2 , tmp3 , tmp4 ;
        switch (type){
            case LUI:
            case AUIPC:
                rd = ( fromMemory >> 7 ) & 31;
                imm = (fromMemory >> 12) << 12;
                break;
            case JAL:
            case JALR:
            tmp1 = (fromMemory >> 7) & 1; tmp2 = (fromMemory >> 8) & 15;
            tmp3 = (fromMemory >> 25) & 63; tmp4 = (fromMemory >> 31) & 1;
            imm = (tmp1 << 11) & (tmp2) & (tmp3 << 5) & (tmp4 << 12);
            imm = signext(imm, 12);
            rd = ( fromMemory >> 7 ) & 31;
            rs1 = ( fromMemory >> 15 ) & 31;
            break;

            case BEQ:
            case BNE:
            case BLT:
            case BGE:
            case BLTU:
            case BGEU:
            tmp1 = (fromMemory >> 7) & 1; tmp2 = (fromMemory >> 8) & 15;
            tmp3 = (fromMemory >> 25) & 63; tmp4 = (fromMemory >> 31) & 1;
            imm = (tmp1 << 11) & (tmp2) & (tmp3 << 5) & (tmp4 << 12);
            imm = signext(imm, 12);
            rs1 = ( fromMemory >> 15 ) & 31;
            rs2 = ( fromMemory >> 20 ) & 31;
            break;

            case LB:
            case LH:
            case LW:
            case LBU:
            case LHU:
            imm = (fromMemory >> 20) & 4095;
            rd = ( fromMemory >> 7 ) & 31;
            rs1 = ( fromMemory >> 15 ) & 31;
            break;

            case SB:
            case SH:
            case SW:
            tmp1 = (fromMemory >> 7) & 31; tmp2 = (fromMemory >> 25) & 127;
            imm = tmp1 + (tmp2 << 5);
            rd = ( fromMemory >> 7 ) & 31;
            rs1 = ( fromMemory >> 15 ) & 31;
            break;

            case ADDI:
            case SLTI:
            case SLTIU:
            case XORI:
            case ORI:
            case ANDI:
            imm = (fromMemory >> 20) & 4095;
            rd = ( fromMemory >> 7 ) & 31;
            rs1 = ( fromMemory >> 15 ) & 31;

            break;

            case SLLI:
            case SRLI:
            case SRAI:
            rd = ( fromMemory >> 7 ) & 31;
            rs1 = ( fromMemory >> 15 ) & 31;
            break;

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
            rd = ( fromMemory >> 7 ) & 31;
            rs1 = ( fromMemory >> 15 ) & 31;
            rs2 = ( fromMemory >> 20 ) & 31;
            break;

        }
    }

   

};


#endif
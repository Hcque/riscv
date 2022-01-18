
// func3, funct 7
// + not &
// JALR


// extend 11? 10
// extend 10 12
// from mem flush

#ifndef INSTRUCTION_HPP
#define INSTRUCTION_HPP

#include <cstdint>
#include <iostream>
#include <cassert>
#include <string>
#include <bitset>

using namespace std;

enum Inst_Type { ERROR, //0
                LUI, //1
                AUIPC, //2
                JAL, //3
                JALR, //4
                
                BEQ,  // 5
                BNE, //6
                BLT, // 7
                BGE,  // 8
                BLTU,  // 9
                BGEU, // 10

                LB, // 11
                LH,  // 12
                LW, // 13
                LBU, //14
                LHU, // 15
                SB, // 16
                SH, // 17
                SW, // 18
                
                ADDI, // 19
                SLTI, // 20
                SLTIU, // 21
                XORI, // 22
                ORI,  // 23
                ANDI,  // 24
                SLLI, // 25
                SRLI, // 26
                SRAI, // 27

                ADD, // 28
                SUB,  //29
                SLL, // 30
                SLT, // 31
                SLTU, // 32
                XOR, //33
                SRL, //34
                SRA, //35
                OR, // 36
                AND // 37
                
                };

std::string _type_Name[40] = { "ERROR", //0
                "LUI", //1
                "AUIPC", //2
                "JAL", //3
                "JALR", //4
                
                "BEQ",  // 5
                "BNE", //6
                "BLT", // 7
                "BGE",  // 8
                "BLTU",  // 9
                "BGEU", // 10

                "LB", // 11
                "LH",  // 12
                "LW", // 13
                "LBU", //14
                "LHU", // 15
                "SB", // 16
                "SH", // 17
                "SW", // 18
                
                "ADDI", // 19
                "SLTI", // 20
                "SLTIU", // 21
                "XORI", // 22
                "ORI",  // 23
                "ANDI",  // 24
                "SLLI", // 25
                "SRLI", // 26
                "SRAI", // 27

                "ADD", // 28
                "SUB",  //29
                "SLL", // 30
                "SLT", // 31
                "SLTU", // 32
                "XOR", //33
                "SRL", //34
                "SRA", //35
                "OR", // 36
                "AND" // 37
                
                };


typedef uint32_t uint;

class Instruction{
public:
    uint32_t imm, rs1, rs2, rd, opcode, func7, func3;
    uint fromMemory;
    Inst_Type type;

    uint src1, src2;
    uint dest;
    int take_bch;
    int BXX;
    int succ;
    bool FP_action, load, store;

    uint32_t addr;
    bool regWrite;
    bool memRead;
    bool stalled;

    Instruction(){
        imm = rs1 = rs2 = rd = opcode = func7 = func3 = 0u;
        src1 = src2 = fromMemory = 0u;
        dest = 0u;
        take_bch = addr = 0u;
        type = ERROR;
        stalled = regWrite = memRead = 0;
        BXX = 0; succ = 0;
        FP_action = load = store = 0;

    }
    void clear()
    {
        imm = rs1 = rs2 = rd = opcode = func7 = func3 = 0u;
        dest = 0u;
        src1 = src2 = fromMemory = 0u;
        take_bch = addr = 0u;
        type = ERROR;
        opcode = 0u;
        regWrite = memRead = 0;
        BXX = 0; succ = 0;
        FP_action = load = store = 0;
    }


    friend std::ostream& operator<< (std::ostream& out, const Instruction& inst)
    {
        out << "=================== \n";
        out << "INST FROM ADDR: " << _type_Name[inst.type] << " " << inst.addr << "\n";
        out << "rs1 "  << "rs2 " << "rd" << "\n";
        out << "TYPE:" << _type_Name[inst.type] << "  " << inst.rs1 << "  " << inst.rs2 << "  " << inst.rd << "\n";
        out << "TYPE:" << _type_Name[inst.type] << "  " << inst.src1 << "  " << inst.src2 << "  " << inst.dest << "\n";
        // bitset<21> a(inst.imm);
        // bitset<10> b(inst.func3);

        // out << "TYPE:" << _type_Name[inst.type] << " | " << a << " \n ";
        // out << "TYPE:" << _type_Name[inst.type] << " func3| " << b << " \n ";
        return out;
    }

    Inst_Type getType(){
        opcode = fromMemory & 127;
        func3 = (fromMemory >> 12) & (7);
        func7 = (fromMemory >> 25) & (127);
        // cout  << "opcode: " << opcode  << endl;
        
        switch (opcode){
            case 55: type = LUI; break;
            case 23: type = AUIPC; FP_action = 1; break;
            case 111: type = JAL; FP_action = 1; break;
            case 103: type = JALR; FP_action = 1; break;
            case 99: 
                switch (func3){
                    case 0: type = BEQ; break;
                    case 1: type = BNE; break;
                    case 4: type = BLT; break;
                    case 5: type = BGE; break;
                    case 6: type = BLTU; break;
                    case 7: type = BGEU; break;
                }
                FP_action = 1;
                break;
            case 3:
                switch (func3){
                    case 0: type = LB; break;
                    case 1: type = LH; break;
                    case 2: type = LW; break;
                    case 4: type = LBU; break;
                    case 5: type = LHU; break;
                }
                break;
            
            case 35:
                switch (func3){
                    case 0: type = SB; break;
                    case 1: type = SH; break;
                    case 2: type = SW; break;
                }
                break;
            
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
                        if (func7 == 0) type = SRLI; 
                        else if(func7 == 32) type = SRAI;
                        break;
                
                }
                FP_action = 1;
                break;
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
                FP_action = 1;
                break;
            default: type = ERROR; break;
        }
        // assert(type != ERROR);

        if (
            type == LW ||
            type == LB ||
            type == LH ||
            type == LBU ||
            type == LHU 
        ) { memRead = 1; load = 1; }
         if (
            type == SW ||
            type == SB ||
            type == SH ||
        ) { store = 1; }

        if (
            type == LW ||
            type == LB ||
            type == LH ||
            type == LBU ||
            type == LHU ||

            type == LUI ||
            type == AUIPC ||
            type == JAL ||
            type == JALR ||

            type == ADDI ||
            type == SLTI ||
            type == SLTIU ||
            type == XORI ||
            type == ORI ||
            type == ANDI ||
            type == SLLI ||
            type == SRLI ||
            type == SRAI ||

            type == ADD ||
            type == SUB ||
            type == SLL ||
            type == SLT ||
            type == SLTU ||
            type == XOR ||
            type == SRL ||
            type == SRA ||
            type == OR ||
            type == AND 

        ) { regWrite = 1; }

        return type;
    }

    static uint signext(uint x, int n){
        if ( (x >> n) & 1 ) x |= 0xffffffff >> n << n;
        return x;
    }

    void getArgs(){
        uint tmp1, tmp2 , tmp3 , tmp4 ;
        switch (type){
            case ERROR: return;
            case LUI:
            case AUIPC:
                rd = ( fromMemory >> 7 ) & 31;
                imm = (fromMemory >> 12) << 12;
                break;
            case JAL:
            tmp1 = (fromMemory >> 12) & 255;
            tmp2 = (fromMemory >> 20) & 1; 
            tmp3 = (fromMemory >> 21) & 1023; 
            tmp4 = (fromMemory >> 31) & 1;
            imm = (tmp1 << 12) + (tmp2<<11) + (tmp3<<1) + (tmp4 << 20);
            imm = signext(imm, 20);
            // using namespace std;
            // cout << "imm:" << imm << endl;
            rd = ( fromMemory >> 7 ) & 31;
            break;
            case JALR:
            rd = ( fromMemory >> 7 ) & 31;
            imm = (fromMemory >> 20) & 4095;
            imm = signext(imm, 11);
            rs1 = ( fromMemory >> 15 ) & 31;
            break;

            case BEQ:
            case BNE:
            case BLT:
            case BGE:
            case BLTU:
            case BGEU:
            //  ... tmp2 tmp1
            tmp1 = (fromMemory >> 7) & 1; tmp2 = (fromMemory >> 8) & 15;
            tmp3 = (fromMemory >> 25) & 63; tmp4 = (fromMemory >> 31) & 1;
            imm = (tmp1 << 11) + (tmp2 << 1) + (tmp3 << 5) + (tmp4 << 12);
            imm = signext(imm, 12);
            rs1 = ( fromMemory >> 15 ) & 31;
            rs2 = ( fromMemory >> 20 ) & 31;
            BXX = 1;
            break;

            case LB:
            case LH:
            case LW:
            case LBU:
            case LHU:
            imm = (fromMemory >> 20) & 4095;
            imm = signext(imm, 11);
            rd = ( fromMemory >> 7 ) & 31;
            rs1 = ( fromMemory >> 15 ) & 31;
            break;

            case SB:
            case SH:
            case SW:
            tmp1 = (fromMemory >> 7) & 31; tmp2 = (fromMemory >> 25) & 127;
            imm = tmp1 + (tmp2 << 5);
            imm = signext(imm, 11);
            // rd = ( fromMemory >> 7 ) & 31;
            rs1 = ( fromMemory >> 15 ) & 31;
            rs2 = ( fromMemory >> 20 ) & 31;
            break;

            case ADDI:
            case SLTI:
            case SLTIU:
            case XORI:
            case ORI:
            case ANDI:
            imm = (fromMemory >> 20) & 4095;
            imm = signext(imm, 11);
            rd = ( fromMemory >> 7 ) & 31;
            rs1 = ( fromMemory >> 15 ) & 31;
            break;

            case SLLI:
            case SRLI:
            case SRAI:
            imm = (fromMemory >> 20) & 31;
            imm = signext(imm, 4);
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
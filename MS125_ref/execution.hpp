# include "memoryaccess.hpp"
# include "register.hpp"
# include "tools.hpp"
# include "prediction.hpp"

#include <iostream>

# ifndef _EXECUTION_
	# define _EXECUTION_

class Execution {
		friend class RISCV;
	private:
		Register *reg;
		globalPrediction *glb;

	public:
		Instruction inst;

		Execution(Register *_reg) {
			reg = _reg;
		}

		// Execution(Register *_reg, globalPrediction *_glb) {
		// 	reg = _reg;
		// 	glb = _glb;
		// }

		void go() {
			if(inst.type == ERR) return ;
			switch(inst.type) {
				case LUI: inst.result = inst.imm; break;
				case AUIPC: inst.result = reg->pc - 4 + inst.imm; break;
				case JAL:
			     inst.result = reg->pc;
                reg->pc = reg->pc + inst.imm - 4; 	
				break;

				case JALR: 
				inst.result = reg->pc;
                reg->pc = inst.src1 + inst.imm ; 
				// std::cout << "pc:: " << reg->pc<< std::endl;
                reg->pc = setlowzero(reg->pc);

				// inst.resultpc = inst.src1 + inst.imm ; 
				// inst.resultpc = setlowzero(inst.resultpc); 
				break;

				case BEQ: inst.result =  (inst.src1 == inst.src2); break;
				case BNE: inst.result =  (inst.src1 != inst.src2); break;
			  case BLTU: inst.result =  (inst.src1 < inst.src2); break;
				case BGEU: inst.result =  (inst.src1 >= inst.src2); break;
				case BLT: inst.result =  ((int)inst.src1 < (int)inst.src2); break;
			  case BGE: inst.result =  ((int)inst.src1 >= (int)inst.src2); break;
				case LB:
				case LW:
				case LH:
				case LHU:
				case LBU: inst.src1 = inst.src1 + sgnextend(inst.imm, 11); break;
				case SB:
				case SW:
				case SH: inst.src1 = inst.src1 + sgnextend(inst.imm, 11); break;
				case ADDI: inst.result = inst.src1 + inst.imm; break;
				case SLTI: inst.result =  ((int)inst.src1 < (int)inst.imm); break;
				case SLTIU: inst.result =  (inst.src1 < inst.imm); break;
				case ANDI: inst.result = (inst.src1 & inst.imm); break;
				case ORI: inst.result = (inst.src1 | inst.imm); break;
				case XORI: inst.result = (inst.src1 ^ inst.imm); break;
				case SLLI: inst.result = (inst.src1 << inst.imm); break;
				case SRLI: inst.result = (inst.src1 >> inst.imm); break;
				case SRAI: inst.result = ((inst.src1 >> inst.imm) | (inst.src1 >> 31 << 31)); break;
				case ADD: inst.result = inst.src1 + inst.src2; break;
				case SUB: inst.result = inst.src1 - inst.src2; break;
				case SLL: inst.result = (inst.src1 << inst.src2); break;
				case SRL: inst.result = (inst.src1 >> inst.src2); break;
				case SRA: inst.result = ((inst.src1 >> inst.src2) | (inst.src1 >> 31 << 31)); break;
				case SLT: inst.result =  ((int)inst.src1 < (int)inst.src2); break;
				case SLTU: inst.result =  (inst.src1 < inst.src2); break;
				case XOR: inst.result = (inst.src1 ^ inst.src2);
				// std::cout << "XOR" << inst.result << "|" << inst.src1 
				// << "|" << inst.src2
				// << std::endl;
				break;
				case OR: inst.result = (inst.src1 | inst.src2); break;
				case AND: inst.result = (inst.src1 & inst.src2); break;
				default: break;
			}

			// take the branch

        if (inst.type == BNE || inst.type == BEQ || inst.type == BLT || inst.type == BGE ||
         inst.type == BLTU || inst.type == BGEU)
         {
            if (inst.result)
            {
                reg->pc = reg->pc - 4 + inst.imm;
            // std::cout << "BNE" << reg->pc << std::endl;
            }
         }


		}
		
    // bool check() {
    //   if(inst.type == BEQ || inst.type == BNE || inst.type == BGE || inst.type == BLT || inst.type == BGEU || inst.type == BLTU) {
    //     if(inst.pred != inst.result) {
    //       glb -> addResult(inst.type, inst.result, 0);
    //       if(inst.pred == 0) reg -> setpc(inst.resultpc - 4 + inst.imm);
    //       else reg -> setpc(inst.resultpc);
    //       return false;
    //     } else glb -> addResult(inst.type, inst.result, 1);
    //     return true;
    //   }
    //   return true;
    // }
		
    void pass(MemoryAccess &nxt) {
			nxt.inst = inst;
		}
};

# endif

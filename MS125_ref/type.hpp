# ifndef _TYPE_HPP_
	# define _TYPE_HPP_

	typedef unsigned int uint;

	enum InstructionType {EMPTY, ERR, LUI, AUIPC, JAL, 
	JALR, 
	BEQ, BNE, BLT, BGE, 
	BLTU,
	BGEU, LB, LH, LW, 
	LBU, 
	LHU, SB, SH, SW, 
	ADDI, 
	
	SLTI, SLTIU, XORI, ORI, ANDI, SLLI, SRLI, SRAI, 
	ADD, SUB, SLL, SLT, SLTU, XOR, SRL, SRA, OR, AND};


std::string _type_Name[40] = { "EMPTY", "ERROR", //0
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


// enum InstructionType2 { ERR, //0
//                 LUI, //1
//                 AUIPC, //2
//                 JAL, //3
//                 JALR, //4
                
//                 BEQ,  // 5
//                 BNE, //6
//                 BLT, // 7
//                 BGE,  // 8
//                 BLTU,  // 9
//                 BGEU, // 10

//                 LB, // 11
//                 LH,  // 12
//                 LW, // 13
//                 LBU, //14
//                 LHU, // 15
//                 SB, // 16
//                 SH, // 17
//                 SW, // 18
                
//                 ADDI, // 19
//                 SLTI, // 20
//                 SLTIU, // 21
//                 XORI, // 22
//                 ORI,  // 23
//                 ANDI,  // 24
//                 SLLI, // 25
//                 SRLI, // 26
//                 SRAI, // 27

//                 ADD, // 28
//                 SUB,  //29
//                 SLL, // 30
//                 SLT, // 31
//                 SLTU, // 32
//                 XOR, //33
//                 SRL, //34
//                 SRA, //35
//                 OR, // 36
//                 AND, // 37
// 				EMPTY
				
                
//                 };

	enum pipelineType {Serial, Pararrel};

	const int max_memory = 0x3fffff;

# endif

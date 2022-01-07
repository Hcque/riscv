// cp other tenplate for cmake
// filename
// reg mem not the same


// jal, imm + / &


// 4164 | 4168 JAL, bad for JALR,loop



#include "Common.hpp"
#include <gtest/gtest.h>
#include <bitset>

memory mem;
TEST(ArrayTest, test1)
{
    RISCV riscv = RISCV(&mem);
    mem.loadFromDisk();
    uint32_t base = 0x0000;
    uint32_t ans = riscv.regs.load(base, 4);
    char buf[4];
    memcpy(buf, mem.data, 4);
    // printf("%x\n", mem.data[base+0]);
    // printf("%x\n", mem.data[base+1]);
    // printf("%x\n", mem.data[base+2]);
    // printf("%x\n", mem.data[base+3]);
    EXPECT_EQ((uint32_t)mem.data[base+0], 0x37);
    EXPECT_EQ((uint32_t)mem.data[base+1], 0x01);
}

TEST(ArrayTest, test2)
{
    RISCV riscv = RISCV(&mem);
    mem.loadFromDisk();
    uint32_t base = 0x1000;
    uint32_t ans = riscv.regs.load(base, 4);
    // printf("%x\n", mem.data[base+0]);
    // printf("%x\n", mem.data[base+1]);
    // printf("%x\n", mem.data[base+2]);
    // printf("%x\n", mem.data[base+3]);
    EXPECT_EQ((uint32_t)mem.data[base+0], 0x37);
    EXPECT_EQ((uint32_t)mem.data[base+1], 0x17);
}


TEST(ArrayTest, test3)
{
    RISCV riscv = RISCV(&mem);
    mem.loadFromDisk("../data/array_test1.data");
    uint32_t base = 0x0000;
    uint32_t ans = riscv.regs.load(base, 4);
    // EXPECT_EQ(ans, 0x020137);
}

TEST(ArrayTest, test4)
{
    RISCV riscv = RISCV(&mem);
    mem.loadFromDisk("../data/array_test1.data");

    // inspect memory bits
    uint32_t base = 0x0000;

    Instruction inst;
    int I = 0;
    int B = base;
    for (;I<4; I++, B +=1)
        printf("%.2x ", mem.data[B]);
    cout << endl;
    I = 0;
    B = base;
    for (;I<4; I++, B +=1)
    {
        bitset<8> a(mem.data[B]);
        cout << a << " ";
    }
    cout << endl;

    // stage IF
    riscv.regs.pc = base;
    inst.fromMemory = riscv.regs.load(base, 4);
    riscv.regs.pc += 4;

    cout << "after load" << endl;

    for (I = 0;I<4; I++ )
        printf("%.2x ", (inst.fromMemory>>(8*I)) & (255u));
    cout << endl;
    for (I=0;I<4; I++ )
    {
        bitset<8> a(inst.fromMemory>>(8*I));
        cout << a << " ";
    }
    cout << endl;
    
    // stage ID
    inst.type = inst.getType();
    inst.getArgs();


    // stage EX
    Execution ex = Execution(&riscv.regs, inst);
    ex.go();

    cout << inst ;
    cout << riscv.regs ;

    MemoryAccess ma = MemoryAccess(&riscv.regs);
    WriteBack wb = WriteBack(&riscv.regs);

    ex.pass(ma);
    ma.go();
    ma.pass(wb);
    // cout << riscv.regs ;
    wb.go();
    cout << riscv.regs ;

    // EXPECT_EQ(riscv.regs.reg[2] >> 12, 131072);

}


TEST(ArrayTest, test5)
{
    RISCV riscv = RISCV(&mem);
    mem.loadFromDisk("../data/array_test1.data");

    // inspect memory bits
    uint32_t base = 0x0010;

    Instruction inst;
    int I = 0;
    int B = base;
    for (;I<4; I++, B +=1)
        printf("%.2x ", mem.data[B]);
    cout << endl;
    I = 0;
    B = base;
    for (;I<4; I++, B +=1)
    {
        bitset<8> a(mem.data[B]);
        cout << a << " ";
    }
    cout << endl;

    // stage IF
    riscv.regs.pc = base;
    inst.fromMemory = riscv.regs.load(base, 4);
    riscv.regs.pc += 4;

    cout << "after load" << endl;

    for (I = 3;I>=0; I-- )
        printf("%.2x ", (inst.fromMemory>>(8*I)) & (255u));
    cout << endl;
    for (I=3;I>=0; I-- )
    {
        bitset<8> a(inst.fromMemory>>(8*I));
        cout << a << " ";
    }
    cout << endl;
    
    // stage ID
    inst.type = inst.getType();
    inst.getArgs();


    // stage EX
    Execution ex = Execution(&riscv.regs, inst);
    ex.go();

    cout << inst ;
    cout << riscv.regs ;

    // printf("mem: %x\n", mem.data[4]);
    MemoryAccess ma = MemoryAccess(&riscv.regs);
    WriteBack wb = WriteBack(&riscv.regs);

    ex.pass(ma);
    ma.go();
    ma.pass(wb);
    // cout << riscv.regs ;
    wb.go();
    cout << riscv.regs ;
    

    EXPECT_EQ(mem.data[4], 0);
    // cout <<  ex.inst.dest << "mem:"  << endl;
    // printf("%x\n", mem.data[4]);

}



TEST(ArrayTest, test6)
{
    RISCV riscv = RISCV(&mem);
    mem.loadFromDisk("../data/array_test1.data");

    // inspect memory bits
    uint32_t base = 0x0014;

    Instruction inst;
    int I = 0;
    int B = base;
    for (;I<4; I++, B +=1)
        printf("%.2x ", mem.data[B]);
    cout << endl;
    I = 0;
    B = base;
    for (;I<4; I++, B +=1)
    {
        bitset<8> a(mem.data[B]);
        cout << a << " ";
    }
    cout << endl;

    // stage IF
    riscv.regs.pc = base;
    inst.fromMemory = riscv.regs.load(base, 4);
    riscv.regs.pc += 4;

    cout << "after load" << endl;

    for (I = 3;I>=0; I-- )
        printf("%.2x ", (inst.fromMemory>>(8*I)) & (255u));
    cout << endl;
    for (I=3;I>=0; I-- )
    {
        bitset<8> a(inst.fromMemory>>(8*I));
        cout << a << " ";
    }
    cout << endl;
    
    // stage ID
    inst.type = inst.getType();
    inst.getArgs();

    uint pcold = riscv.regs.pc;
    // stage EX
    Execution ex = Execution(&riscv.regs, inst);
    ex.go();

    cout << ex.inst ;
    cout << riscv.regs ;

    bitset<20> aa(ex.inst.imm);
    cout << aa << endl;

    uint im = ex.inst.imm;
    // printf("mem: %x\n", mem.data[4]);
    MemoryAccess ma = MemoryAccess(&riscv.regs);
    WriteBack wb = WriteBack(&riscv.regs);

    ex.pass(ma);
    ma.go();
    ma.pass(wb);
    // cout << riscv.regs ;
    wb.go();
    cout << riscv.regs ;

    // EXPECT_EQ(pcold + im, riscv.regs.pc);
}


TEST(ArrayTest, test7)
{
    RISCV riscv = RISCV(&mem);
    mem.loadFromDisk("../data/array_test1.data");

    // inspect memory bits
    uint32_t base = 4168;

    Instruction inst;
    int I = 0;
    int B = base;
    for (;I<4; I++, B +=1)
        printf("%.2x ", mem.data[B]);
    cout << endl;
    I = 0;
    B = base;
    for (;I<4; I++, B +=1)
    {
        bitset<8> a(mem.data[B]);
        cout << a << " ";
    }
    cout << endl;

    // stage IF
    riscv.regs.pc = base;
    inst.fromMemory = riscv.regs.load(base, 4);
    riscv.regs.pc += 4;

    cout << "after load" << endl;

    for (I = 3;I>=0; I-- )
        printf("%.2x ", (inst.fromMemory>>(8*I)) & (255u));
    cout << endl;
    for (I=3;I>=0; I-- )
    {
        bitset<8> a(inst.fromMemory>>(8*I));
        cout << a << " ";
    }
    cout << endl;
    
    // stage ID
    inst.type = inst.getType();
    inst.getArgs();

    uint pcold = riscv.regs.pc;
    // stage EX
    Execution ex = Execution(&riscv.regs, inst);
    ex.go();

    cout << ex.inst ;
    cout << riscv.regs ;

    bitset<20> aa(ex.inst.imm);
    cout << aa << endl;

    uint im = ex.inst.imm;
    // printf("mem: %x\n", mem.data[4]);
    MemoryAccess ma = MemoryAccess(&riscv.regs);
    WriteBack wb = WriteBack(&riscv.regs);

    ex.pass(ma);
    ma.go();
    ma.pass(wb);
    // cout << riscv.regs ;
    wb.go();
    cout << riscv.regs ;

    // EXPECT_EQ(pcold + im, riscv.regs.pc);
    

}


TEST(ArrayTest, test8)
{
    RISCV riscv = RISCV(&mem);
    mem.loadFromDisk("../data/array_test1.data");

    // inspect memory bits
    uint32_t base = 0x000c;
int II = 0;
while  ( II++ < 2){
    Instruction inst;
    int I = 0;
    int B = base;
    for (;I<4; I++, B +=1)
        printf("%.2x ", mem.data[B]);
    cout << endl;
    I = 0;
    B = base;
    for (;I<4; I++, B +=1)
    {
        bitset<8> a(mem.data[B]);
        cout << a << " ";
    }
    cout << endl;

    // stage IF
    riscv.regs.pc = base;
    inst.fromMemory = riscv.regs.load(base, 4);
    riscv.regs.pc += 4; base += 4;

    cout << "after load" << endl;

    for (I = 3;I>=0; I-- )
        printf("%.2x ", (inst.fromMemory>>(8*I)) & (255u));
    cout << endl;
    for (I=3;I>=0; I-- )
    {
        bitset<8> a(inst.fromMemory>>(8*I));
        cout << a << " ";
    }
    cout << endl;
    
    // stage ID
    inst.type = inst.getType();
    inst.getArgs();

    uint pcold = riscv.regs.pc;
    // stage EX
    Execution ex = Execution(&riscv.regs, inst);
    ex.go();

    cout << ex.inst ;
    cout << riscv.regs ;

    bitset<20> aa(ex.inst.imm);
    cout << aa << endl;

    uint im = ex.inst.imm;
    // printf("mem: %x\n", mem.data[4]);
    MemoryAccess ma = MemoryAccess(&riscv.regs);
    WriteBack wb = WriteBack(&riscv.regs);

    ex.pass(ma);
    ma.go();
    ma.pass(wb);
    // cout << riscv.regs ;
    wb.go();
    cout << riscv.regs ;

    // EXPECT_EQ(pcold + im, riscv.regs.pc);
}
    

}





int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

// int main()
// {
//     RISCV riscv = RISCV(&mem);
//     mem.loadFromDisk("../data/array_test1.data");
//     auto ans = riscv.regs.load(0, 4);
// }
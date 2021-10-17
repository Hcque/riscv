#include <iostream>
#include "RISCV.hpp"
#include "memory.hpp"

RISCV riscv;
memory mem;
int main(){
    riscv = RISCV(&mem);

    mem.loadFromDisk();
    riscv.run();
    // std::cout << riscv.output() << std::endl;
    return 0;
}
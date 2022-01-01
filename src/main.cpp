#include <iostream>
#include "RISCV.hpp"
#include "memory.hpp"



memory mem;
int main(int argc, char* argv[]){
    RISCV riscv = RISCV(&mem);

    mem.loadFromDisk(argv[1]);
    riscv.run();
    std::cout << " === RISCV OUTPUT: === " << std::endl;
    std::cout << riscv.output() << std::endl;
    std::cerr << riscv.output() << std::endl;

    return 0;
}
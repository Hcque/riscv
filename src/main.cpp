// notice the memory size!

#include <iostream>
#include "RISCV.hpp"
#include "memory.hpp"

memory mem;
int main(int argc, char* argv[]){
    RISCV riscv = RISCV(&mem);


    // std::cerr << "argc:" << argc << std::endl;
    mem.loadFromDisk();
    int mode; 
    if (argc != 2) mode = 1;
    else mode = atoi(argv[1]);
    // std::cerr << "mode:" << mode << std::endl;
    
    riscv.run((RunMode)mode);
    // std::cout << " === RISCV OUTPUT: === " << std::endl;
    // std::cout << riscv.output() << std::endl;
    int ans = riscv.output();
    std::cerr << ans << std::endl;

    return 0;
}
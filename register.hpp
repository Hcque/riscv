#ifndef REGISTER_HPP
#define REGISTER_HPP

#include <cstdint>

class Register{
public:
    uint32_t reg[32]{}; // reg x0 - x31
    uint32_t pc{0};
    
    uint32_t get(int i){
        if (i == 0) return 0u;
        return reg[i];
    }
    void set(int i, uint item){
        reg[i] = item;
    }

};

#endif
#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>

uint32_t decode16(char c){
    if ( '0' <= c && c <= '9' ) return c - '0';
    else if ( 'A' <= c && c <= 'F' ) return c - 'A' + 10;
    else return 0; // never reached
}

uint32_t decode16(char* str){ // "A4"
    uint32_t res = 0;
    for (int i=0;str[i];i++) 
        res = (res << 4) + decode16(str[i]);
    return res;
}

    uint32_t signext(uint32_t x, int n){
        if ( x >> (n-1) ) x |= (0xffffffff >> n) << n;
        return x;
    }




#endif
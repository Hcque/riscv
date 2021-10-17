#ifndef UTIL_HPP
#define UTIL_HPP

#include <cstdint>

uint32_t decode16(char c){
    if ( '0' <= c && c <= '9' ) return c - '0';
    else if ( 'A' <= c &&c <= 'F' ) return c - 'A' + 10;
    else return 0; // never reached
}

uint32_t decode16(char* str){
    uint32_t res = 0;
    for (int i=0;i<2;i++) 
        res = (res << 4) + decode16(str[i]);
    return res;

}

#endif
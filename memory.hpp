#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <iostream>
#include "util.hpp"

const int max_mem_size = 1e5;

class memory{
public:
    char array[max_mem_size];
    char data[10000];
    uint32_t pc_value;

    memory(){}
    void loadFromDisk(){
        char buf[20];
       
        int x;
        int count;
        while ( (x = scanf("%s",buf)) > 0 ){
            std::cout << x << buf << "\n";
            if (buf[0] == '@') pc_value = decode16(buf+1);
            data[count] = decode16(buf);
        }

    }

    uint load(uint pos, uint p){
        uint res = 0;
        for (int i=0;i<p;i++)
            res = res + ( array[pos+i] << i*8 ); //little endian
        return res;
    }
    void store(uint pos, uint p, uint item){
        for (int i=0;i<p;i++){
            array[pos+i] = (item >> 8*i) & 255;
        }


    }


};

#endif
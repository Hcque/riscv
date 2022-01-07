#ifndef MEMORY_HPP
#define MEMORY_HPP

#include <iostream>
#include <cstring>
#include <string>
#include <cstdio>
#include <fstream>
#include "util.hpp"

const int MAX_MEM_SZ = 0x3fffff;

using namespace std;

class memory{
    friend class Register;
public:
    unsigned char data[MAX_MEM_SZ];
    uint32_t pc_value;

    memory(){
        memset(data,0,sizeof data);
        pc_value = 0u;
    }
    void loadFromDisk(const char *filename){
    }
    void loadFromDisk(){
        char buf[32]; memset(buf, 0, sizeof buf);
        int count = 0;
        // std::ifstream infile(filename);

        while ( std::cin >> buf ){
        // while ( infile >> buf ){
            // std::cout << "buf: " << buf << "\n";
            if (buf[0] == '@') count = decode16(buf+1);//, cout << count;
            else 
            {
                data[count] = (unsigned char) decode16(buf);
                count++;
            }
            memset(buf, 0, sizeof buf);
        }
        // cout << (uint32_t)data[4] << endl;
    }







};

#endif
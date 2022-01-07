#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
using namespace std;

int main(int argc, char* argv[])
{

    fstream f1;
    f1.open(argv[1]);
    string buf;
    while( getline(f1, buf))
    {
        cout << buf << endl;

    }


}
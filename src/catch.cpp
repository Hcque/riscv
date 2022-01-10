#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

vector<string> v1,v2;


void rd(string fname, vector<string>& v1)
{
 fstream f1;
    f1.open(fname);
    string buf;
    while( getline(f1, buf))
    {
        stringstream ss(buf);
        string tmp, type, addr;
        while (ss >> tmp)
        {
            if (tmp == "ADDR:")
            {
                ss >> type >> addr;
                // cout << type << addr;
                if (type != "ERROR")
                    v1.push_back(type+addr);
            }
        }

    }
}

int main(int argc, char* argv[])
{
    string n1 = argv[1];
    string n2 = argv[2];

    rd(n1,  v1);
    rd(n2,  v2);

    cout << "size:" << v1.size() << "\n";
    cout << "size:" << v2.size() << "\n";

    int i = 0, j = 0;
    for (; i < v1.size(), j < v2.size(); )
    {
        if (v1[i] != v2[j])
        {
            j++;
        }
        if (v1[i] != v2[j])
        {
            cout << "i|j" << i  << "|" << j << "\n";
            cout << v1[i] << endl;
            cout << v2[j] << endl;
            break;
        }
        cout << v1[i] << " " << v2[j] << endl;
        i++, j ++;
    }

}
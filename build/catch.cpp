#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;


struct Node
{
    string type, addr;
    uint32_t rs1, rs2, rd;
    uint32_t src1, src2, dest;
    friend bool operator == (const Node& a, const Node& b)
    {
        return a.type == b.type && a.addr == b.addr && a.rs1 == b.rs1 && a.rs2 == b.rs2 && 
        a.rd == b.rd && a.src1 == b.src1 && a.src2 == b.src2 && a.dest == b.dest;
    }
        friend bool operator != (const Node& a, const Node& b)
    {
        return !(a == b);
    }

    friend ostream& operator << (ostream& out, const Node& b)
    {
        out << b.type << b.addr << "\n";
        out << b.rs1 << " " << b.rs2 << " " <<  b.rd << "\n";
        out << b.src1 << " " << b.src2 <<  " " << b.dest << "\n";
        return out;
    }

};
vector<Node> v1,v2;

void rd(string fname, vector<Node>& v1)
{
 fstream f1;
    f1.open(fname);
    string buf;
    while( getline(f1, buf))
    {
        stringstream ss(buf);
        string tmp, type, addr;
        Node node;
        while (ss >> tmp)
        {
            if (tmp == "ADDR:")
            {

                ss >> type >> addr;
                // cout << type << addr;
                if (type != "ERROR")
                {
                    node.type = type;
                    node.addr = addr;
                }
                
            }
            
            
        }
        getline(f1, buf);
        getline(f1, buf);
        stringstream ss2(buf);
        ss2 >> tmp;
        ss2 >> node.rs1 >> node.rs2 >> node.rd;

        getline(f1, buf);
        stringstream ss3(buf);
        ss3 >> tmp;
        ss3 >> node.rs1 >> node.rs2 >> node.rd;
        v1.push_back(node);

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
        // if (v1[i] != v2[j])
        // {
        //     j++;
        // }
        if (v1[i] != v2[j])
        {
            cout << "i|j" << i  << "|" << j << "\n";
            cout << v1[i] << endl;
            cout << v2[j] << endl;
            break;
        }
        // cout << v1[i] << "\n " << v2[j] << endl;
        i++, j ++;
    }

}
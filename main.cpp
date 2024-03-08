
#include <bits/stdc++.h>
#include "formats.cpp"

using namespace std;

int main(){
    ifstream opcode;
    opcode.open("opcode.txt");
    string type, func_opcode;
    while (!opcode.eof())
    {
        opcode >> type >> func_opcode;
        op_map.insert(pair<string, string>(type, func_opcode));
    }
}
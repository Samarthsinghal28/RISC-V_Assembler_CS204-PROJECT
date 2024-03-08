#include <bits/stdc++.h>
using namespace std;

int programCounter = 0;
int locationAddress = 0;
string mem[4000];
int tpc;
map<string, char> tab = {
    {"add", 'R'},{"and", 'R'},{"or", 'R'},{"sll", 'R'},{"slt", 'R'},{"sra", 'R'},{"srl", 'R'},{"sub", 'R'},{"xor", 'R'},{"mul", 'R'},{"div", 'R'},
    {"rem", 'R'},{"addi", 'I'},{"andi", 'I'},{"ori", 'I'},{"lb", 'I'},{"lh", 'I'},{"lw", 'I'},{"jalr", 'I'},{"sb", 'S'},{"sw", 'S'},{"sh", 'S'},
    {"beq", 'B'},{"bne", 'B'},{"bge", 'B'},{"blt", 'B'},{"auipc", 'U'},{"lui", 'U'},{"jal", 'J'}
};
map<string, int> label;
int ship=0,offset_of_first_load_inst;
map <string,string> map_op;



int binaryToDec(string binary);
int decStringToInt(string decString);
void loadHexaWord(string s);
void loadHexHalfWord(string s);
string convertToLength5(string s);
string convertToLength20(string s);
string convertToLength12(string s);
string decToBinary(int n);
int binToDec(string binary);
string binToHexa(string bin);
string hexaToBinary(string hexdec);
string rFormatCommands(string line);
string uFormatCommands(string line);
string iFormatCommands(string line);
string sFormatCommands(string line);
string ujFormatCommands(string line);
string sbFormatCommands(string line);
string assemblyToMachine(string line);
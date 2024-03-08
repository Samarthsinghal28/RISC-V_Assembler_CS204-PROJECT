#include <bits/stdc++.h>
using namespace std;

map<string, string> op_map;
int pc = 0;

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
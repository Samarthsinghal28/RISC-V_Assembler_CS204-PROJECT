#include <bits/stdc++.h>
#include <string>
#include "functions.h"
using namespace std;

int binaryToDec(string binary){
    int n = 0;
    int p = 1;
    for (int i = binary.length() - 1; i >= 0; i--){
        if (binary[i] == '1'){
            n += p;
        }
        p *= 2;
    }
    return n;
}

int decStringToInt(string decString){
    int n = 0;
    int p = 1;
    if (decString[0] == '-'){
        for (int i = 1; i < decString.length(); i++){
            n = n * 10 + decString[i] - '0';
        }
        return -1*n; 
    }
    else{
        for (int i = 0; i < decString.length(); i++){
            n = n * 10 + decString[i] - '0';
        }
        return n;
    }
}

string convertToLength5(string s){
    while (s.length() < 5) {
        s = "0" + s;
    }
    return s;
}


string convertToLength20(string s){
    while (s.length() < 20) {
        s = "0" + s;
    }
    return s;
}

string convertToLength12(string s){
    while (s.length() < 12) {
        s = "0" + s;
    }
    return s;
}

string decToBinary(int n){
    string binary;
    while (n > 0){
        char temp = 48 + n % 2;
        n /= 2;
        binary += temp;
    }
    reverse(binary.begin(), binary.end());
    return binary;
}

int binToDec(string binary){
    int num = 0, x = binary.length() - 1;
    string::iterator i;
    for (i = binary.begin(); i != binary.end(); i++){
        num += (*i - 48) * pow(2, x);
        x--;
    }
    return num;
}

string binToHexa(string bin){
    string hex = "";
    int i;
    int p;
    for (i = 0; i < 32; i += 4){
        p = bin.length() - i;

        string temp;
        if (p < 4){
            temp = bin.substr(0, p);
        }
        else{
            temp = bin.substr(p - 4, 4);
        }
        int b = binToDec(temp);

        if (b <= 9){
            char x = 48 + b;
            hex = x + hex;
        }
        else{
            char y = 55 + b; 
            hex = y + hex;
        }
        if (p <= 4){
            return hex;
        }
    }
    return hex;
}

string hexaToBinary(string hexdec){
    int i = 0;
    string out = "";
    while (hexdec[i]){

        switch (hexdec[i]){
        case '0':
            out = out + "0000";
            break;
        case '1':
            out = out + "0001";
            break;
        case '2':
            out = out + "0010";
            break;
        case '3':
            out = out + "0011";
            break;
        case '4':
            out = out + "0100";
            break;
        case '5':
            out = out + "0101";
            break;
        case '6':
            out = out + "0110";
            break;
        case '7':
            out = out + "0111";
            break;
        case '8':
            out = out + "1000";
            break;
        case '9':
            out = out + "1001";
            break;
        case 'A':
        case 'a':
            out = out + "1010";
            break;
        case 'B':
        case 'b':
            out = out + "1011";
            break;
        case 'C':
        case 'c':
            out = out + "1100";
            break;
        case 'D':
        case 'd':
            out = out + "1101";
            break;
        case 'E':
        case 'e':
            out = out + "1110";
            break;
        case 'F':
        case 'f':
            out = out + "1111";
            break;
        }
        i++;
    }
    return out;
}

void loadHexaWord(string s){
    while (s.length() < 8) {
        s = "0" + s;
    }
    int len = 8;
    mem[locationAddress] = s.substr(len - 2, 2);
    mem[locationAddress + 1] = s.substr(len - 4, 2);
    mem[locationAddress + 2] = s.substr(len - 6, 2);
    mem[locationAddress + 3] = s.substr(len - 8, 2);
}

void loadHexHalfWord(string s){
    while (s.length() < 4) {
        s = "0" + s;
    }
    int len = 4;
    mem[locationAddress] = s.substr(len - 2, 2);
    mem[locationAddress + 1] = s.substr(len - 4, 2);
}

string assemblyToMachine(string line){
    istringstream ss(line);
    string inst;
    ss >> inst;
    
    auto itr = tab.find(inst);
    if (itr == tab.end()){
        cout << "Invalid Instruction!" << endl;
        exit(0);
    }
    string cd;
    if (itr != tab.end())
    {
        char type = itr->second;
        if (type == 'R'){
            cd = rFormatCommands(line);
        }

        if (type == 'I'){
            cd = iFormatCommands(line);
        }

        if (type == 'S')
            cd = sFormatCommands(line);

        if (type == 'B')
            cd = sbFormatCommands(line);

        if (type == 'U')
            cd = uFormatCommands(line);

        if (type == 'J')
            cd = ujFormatCommands(line);
    }
    return cd;
}


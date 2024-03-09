#include <bits/stdc++.h>
#include <string>
#include <regex>
#include <math.h>
#include "utilityFunctions.cpp"

using namespace std;

string rFormatCommands(string line)
{
    //add, and, or, sll, slt, sra, srl, sub, xor, mul, div, rem
    // func7(7)   rs2(5)   rs1(5)   func3(3)   rd(5)     opcode(7)
    stringstream instruction(line);
    string word;
    instruction >> word;
    string risc_code;
    risc_code = map_op.find(word)->second; // for add, 0000000 000 0110011(func7func3opcode)
    int r1, r2, r3;
    int temp;
    char ones, tens;
    for(int i=0;i<3;i++){
        instruction >> word;
        tens = (int)word[1] - 48;
        temp = tens;
        ones = (int)word[2] - 48;
        if (ones < 10 && ones >= 0){
            temp = 10 * tens + ones;
        }
        if(i==0) r3=temp;
        else if (i==1) r1=temp;
        else r2=temp;
    }
    if(r1<0 || r1>31 || r2<0 || r2>31 || r3<0 || r3>31){
        cout<<"Invalid register numbers given"<<endl;
        exit(0);
    }
    if (instruction >> word)
    {
        cout << "error :got more than three arguments."<< endl;
        exit(0);
    }
    string machine_code, rs1, rs2, rd, func3, func7, op;

    rs1 = convertToLength5(decToBinary(r1));
    rs2 = convertToLength5(decToBinary(r2));
    rd = convertToLength5(decToBinary(r3));

    func7 = risc_code.substr(0, 7);
    func3 = risc_code.substr(7, 3);
    op = risc_code.substr(10, 7);

    machine_code = "0x" + binToHexa((func7 + rs2 + rs1 + func3 + rd + op));

    string PC = binToHexa(decToBinary(programCounter));

    machine_code = "0x" + PC + "     " + machine_code;
    programCounter += 4;
    return machine_code;
}

string uFormatCommands(string line)
{
    // lui x4,0x12AB7
    //x4 = value<<12   0x12AB7237
    //   imm[31:12](20)      rd(5)      opcode(7)
    stringstream instruction(line);
    string word;
    instruction >> word;
    string risc_code;
    risc_code = map_op.find(word)->second; // for add, 0000000 000 0110011(func7func3opcode)
    int reg, imm;
    char ones, tens;
    instruction >> word;
    tens = (int)word[1] - 48;
    reg = tens;
    ones = (int)word[2] - 48;
    if (ones < 10 && ones >= 0){
        reg = 10 * tens + ones;
    }
    
    instruction >> word;
    int imm_begin = (int)word[0];
    
    if (word.substr(0, 2) == "0b"){ // positive binary
        word = word.substr(2, word.length());
        if (word.length()>20){
            cout <<  "Immediate value ( " << imm << " ) out of range " << endl;
            exit(0);
        }
        imm = binToDec(word);
    }
    else if (word.substr(0, 2) == "0x"){ // positive hexadecimal
        word = word.substr(2, word.length());
        stringstream ss;
        ss << hex << word;
        ss >> imm;
        if (imm > 1048575){
            cout <<  "Immediate value ( " << imm << " ) out of range " << endl;
            exit(0);
        }
    }
    else if (imm_begin <= 57 && imm_begin >= 48){
        stringstream dummy(word);
        dummy >> imm;
        if (imm> 1048575){
            cout << "Immediate value ( " << imm << " ) out of range " << endl;
            exit(0);
        } 
    }
    if(reg<0 || reg>31){
        cout<<"Invalid register numbers given"<<endl;
        exit(0);
    } 
    if (instruction >> word){
        cout << "error : got three arguments(expected 2)." << endl;
        exit(0);
    } 

    string machine_code, rout, imme, op;

    imme = convertToLength20(decToBinary(imm));
    rout = convertToLength5(decToBinary(reg));

    op = risc_code;

    machine_code = imme + rout + op;
    machine_code = binToHexa(machine_code);
    machine_code = "0x" + machine_code;

    string PC = binToHexa(decToBinary(programCounter));

    machine_code = "0x" + PC + "     " + machine_code;
    programCounter += 4;
    return machine_code;
}


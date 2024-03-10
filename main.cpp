#include <bits/stdc++.h>
#include "formats.cpp"

using namespace std;

int main(){
    ifstream asmbl;             //creating input type stream
    ofstream machCode;          //creating output type stream

    string line;
    ifstream opcode;
    opcode.open("opcode.txt");
    string type, func_opcode;
    while (!opcode.eof())
    {
        opcode >> type >> func_opcode;
        map_op.insert(pair<string, string>(type, func_opcode));
    }
    //map_op ready

    asmbl.open("inputFile/asmbl.asm");        //opening assembly code file
    machCode.open("outputFile/mcode.mc");      //opening output code file (outputFile/mcode.mc) 

    regex storeData("(.*[^:]):(.*)");      //regular expression for storing data (Eg -- N : .word 5 )
    smatch match;
    int temp = 1;
    
    while (!asmbl.eof()){               //making the label list
        getline(asmbl, line);
        //cout<<line<<"ip";
        if (line == ".data"){           //checking for data label
            temp = 0;
            continue;
        }
        else if (line == ".text"){      //checking for text label
            temp = 1;
            continue;
        }
        else if (regex_search(line, match, storeData) == true){  //checking store data instruction
            string k = match.str(1);                             //extracting out 1st capturing grp
            istringstream spn(k);                               //converting matched string to input string 
            spn>>k;                                             //removing spaces from the string
            istringstream ss1(match.str(2));                    //extracting out 2nd capturing grp and converting to inp
            string word;
            ss1 >> word;                                        //removing spaces from the string

            if (word == ".word"){                               //checking word type data
                if (label.find(k) == label.end()){              //checking if the label already exists
                    label.insert(pair<string, int>(k, locationAddress));         
                }
                else
                {
                    cout << k << " label declared twice! Error" << endl;        //error if the label already exists
                    exit(0);
                }
                while (ss1 >> word){
                    locationAddress += 4;                                               //increasing location address
                }
            }
            else if (word == ".byte"){                              //checking byte type data
                if (label.find(k) == label.end()){                  //checking if the label already exists
                    label.insert(pair<string, int>(k, locationAddress));
                }
                else{
                    cout << k << " label declared twice! Error" << endl;          //error if the label already exists
                    exit(0);
                }
                while (ss1 >> word){
                    locationAddress += 1;                                            //increasing location address
                }   
            }
            else if (word == ".half"){                                      //checking half type data
                if (label.find(k) == label.end()){                          //checking if the label already exists
                    label.insert(pair<string, int>(k, locationAddress));
                }
                else{
                    cout << k << " label declared twice! Error" << endl;    //error if the label already exists
                    exit(0);
                }
                while (ss1 >> word){
                    locationAddress += 2;                                    //increasing location address
                }
            }
            else if (word == ".dword"){                                       //checking double word type data
                if (label.find(k) == label.end()){                            //checking if the label already exists
                    label.insert(pair<string, int>(k, locationAddress));
                }
                else{
                    cout << k << " label declared twice! Error" << endl;        //error if the label already exists
                    exit(0);
                }
                while (ss1 >> word){
                    locationAddress += 8;                                       //increasing location address
                }
            }
            else if (word == ".asciiz"){                                        //checking asciiz type data
                if (label.find(k) == label.end()){                              //checking if the label already exists
                    label.insert(pair<string, int>(k, locationAddress));
                }
                else{
                    cout << k << " label declared twice" << endl;        //error if the label already exists
                    exit(0);
                }
                ss1 >> word;
                int line = word.length() - 1;
                locationAddress += line;                                        //increasing location address
            }
            else{                                                               //labels will come here
                if (label.find(k) == label.end()){
                    label.insert(pair<string, int>(k, programCounter));
                }
                else{
                    cout << k << " label declared twice" << endl;
                    exit(0);
                }
            }
        }
        else{
            istringstream inputStr(line);                                    //creating input string for the line
            string instr;
            inputStr >> instr;
            if (instr != "\0" && temp == 1){
                //cout << line << " ";
                if (instr != "lw" && instr != "lb" && instr != "ld" ){        //all instructions other than lw/lb/ld 
                    programCounter+=4;                                              //incrementing programCounter by 4
                }
                else{
                    inputStr>>instr;
                    if(instr!="\0"){
                        inputStr>>instr;
                        if (instr != "\0"){                        
                            regex re1("[(]");                           // regular expression for the load instruction from memory
                            smatch m1;
                            if (regex_search(instr, m1, re1) == true){
                                programCounter+=4;                                  //load instructions from the memory
                            }
                            else{
                                programCounter+=8;                                  //loading data from the data segment, each instruction converts to 2 instructions
                            }
                        }
                        else{
                            cout << "Invalid instruction at program counter=" << programCounter << endl;     //invalib lb/lw/ld instruction
                            exit(0);
                        }
                    }
                    else{
                        cout << "Invalid instruction at program counter=" << programCounter << endl;          //invalib lb/lw/ld instruction
                        exit(0);
                    }
                }
            }
        }
    }
    asmbl.close();

    //Second Parsing of the file
    programCounter = 0;
    locationAddress = 0;
    
    asmbl.open("inputFile/asmbl.asm");            //opening assembly code to load data in memory
    while (!asmbl.eof()){

        getline(asmbl, line);
        if (regex_search(line, match, storeData) == true){     //checking for storing regular expression

            string k = match.str(1);            //extracting first part of the matched string
            istringstream ss1(match.str(2));    //extracting second part of the matched string and converting it to input string
            string word;
            ss1 >> word;
            if (word == ".word"){                //checking for word input
                while (ss1 >> word){
                    if (word.substr(0, 2) == "0b"){         //binary input
                        int dec = binaryToDec(word.substr(2, word.length() - 2));       //converting binary to dec
                        stringstream ss;
                        ss << hex << dec;                   //dec to hexadecimal
                        string hexv;
                        ss >> hexv;
                        loadHexaWord(hexv);                  //loading hexadecimal word
                    }
                    else if (word.substr(0, 2) == "0x"){                //hexadecimal input
                        string hexv = word.substr(2, word.length() - 2);
                        loadHexaWord(hexv);
                    }
                    else{
                        regex re1("^(^-?[0-9]+$)");
                        smatch match1;
                        if (regex_search(word, match1, re1) == true){ //decimal input
                            int temp = decStringToInt(word);
                            stringstream ss;
                            ss << hex << temp;                          //dec to hexadecimal
                            string hexv;
                            ss >> hexv;
                            loadHexaWord(hexv);                     //loading hexadecimal word
                        }
                        else{ //label
                            auto f = label.find(word);
                            if (f == label.end()){
                                cout << word << " label not defined but used" << endl;
                            }
                            else
                            {
                                int temp = f->second;
                                stringstream ss;
                                ss << hex << temp;
                                string hexv;
                                ss >> hexv;
                                loadHexaWord(hexv);
                            }
                        }
                    }
                    locationAddress += 4;
                }
            }
            if (word == ".byte"){                       //checking for byte input
                while (ss1 >> word){
                    if (word.substr(0, 2) == "0b"){             //binary input
                        int temp = binaryToDec(word);           //binary to decimal 
                        stringstream ss;
                        ss << hex << temp;                      //decimal to hexadecimal
                        string hexv;
                        ss >> hexv;
                        if (hexv.length() == 1){                //converting to byte
                            hexv = "0" + hexv;
                        }
                        mem[locationAddress] = hexv.substr(hexv.length() - 2, 2);   //storing hexadecimal byte
                    }
                    else if (word.substr(0, 2) == "0x"){        //hexadecimal input                    
                        string hexv = word.substr(2, word.length() - 2);
                        if (hexv.length() == 1){            //converting to byte
                            hexv = "0" + hexv;
                        }
                        mem[locationAddress] = hexv.substr(hexv.length() - 2, 2);       //storing hexadecimal byte
                    }
                    else{
                        regex re1("^(^-?[0-9]+$)");                         //decimal regular expression
                        smatch m1;                                          
                        if (regex_search(word, m1, re1) == true){            //decimal input
                            int temp = decStringToInt(word);                    
                            stringstream ss;
                            ss << hex << temp;                               //int to hexadecimal
                            string hexv;
                            ss >> hexv;
                            if (hexv.length() == 1){                            //converting to byte
                                hexv = "0" + hexv;
                            }
                            mem[locationAddress] = hexv.substr(hexv.length() - 2, 2);       //storing hexadecimal byte
                        }
                        else{                                                   //label
                            auto f = label.find(word);
                            if (f == label.end()){                              
                                cout << word << " label not defined but used" << endl;
                            }
                            else{               //insert label
                                int temp = f->second;
                                stringstream ss;
                                ss << hex << temp;
                                string hexv;
                                ss >> hexv;
                                if (hexv.length() == 1)
                                    hexv = "0" + hexv;
                                mem[locationAddress] = hexv.substr(hexv.length() - 2, 2);
                            }
                        }
                    }
                    locationAddress += 1;
                }
            }
            if (word == ".half"){                               //checking for half word input
                while (ss1 >> word){
                    if (word.substr(0, 2) == "0b"){             //binary input to hexadecimal
                        int temp = binaryToDec(word);
                        stringstream ss;
                        ss << hex << temp;
                        string hexv;
                        ss >> hexv;
                        loadHexHalfWord(hexv);
                    }
                    if (word.substr(0, 2) == "0x"){              //hexadecimal input
                        string hexv = word.substr(2, word.length() - 2);
                        loadHexHalfWord(hexv);
                    }
                    else{
                        regex re1("^(^-?[0-9]+$)");               //decimal input
                        smatch m1;
                        if (regex_search(word, m1, re1) == true){ //decimal to hexadecimal
                            int temp = decStringToInt(word);
                            stringstream ss;
                            ss << hex << temp;
                            string hexv;
                            ss >> hexv;
                            loadHexHalfWord(hexv);
                        }
                        else{                                      //label 
                            auto f = label.find(word);
                            if (f == label.end())
                            {
                                cout << word << " label not defined but used" << endl;
                            }
                            else
                            {
                                int temp = f->second;
                                stringstream ss;
                                ss << hex << temp;
                                string hexv;
                                ss >> hexv;
                                loadHexHalfWord(hexv);
                            }
                        }
                    }
                    locationAddress += 2;
                }
            }

            if (word == ".asciiz"){                                         //checking for asciiz input
                ss1 >> word;
                char c = word[0];
                char cl = word[word.length() - 1];
                if (c != '\"' || cl != '\"'){                               //checking for valib ascii input
                    cout << "Unable to parse string error!" << endl;
                    exit(0);
                } 
                for (int i = 1; i < word.length() - 1; i++){                //converting ascii character to int and then to hexadecimal
                    c = word[i];
                    int tmp = (int)c;
                    stringstream ss;
                    ss << hex << tmp;
                    string hexv;
                    ss >> hexv;
                    mem[locationAddress] = hexv;                            //storing hexadecimal ascii value
                    locationAddress += 1;
                }
                mem[locationAddress] = "\0";
                locationAddress += 1;
                if (ss1 >> word){
                    cout << "ERROR!" << endl;                               
                    exit(0);
                }
            }
        }
    }


    asmbl.close();

    temp = 1;

    //third parsing to convert the instructions to machine code
    asmbl.open("inputFile/asmbl.asm");
    while (!asmbl.eof()){
        getline(asmbl, line);
        istringstream inputStr(line);
        string instr;
        inputStr >> instr;
        if (instr == "\0"){
            continue;
        }
        else if (line == ".data"){
            temp = 0;
            continue;
        }
        else if (line == ".text"){                  //finding text segment
            temp = 1;
            continue;
        }
        if (temp == 1){                             //converting each instruction to machine code and printing it in machine code file
            if (regex_search(line, match, storeData) == false){
                string cd = assemblyToMachine(line);
                machCode << cd << endl;
            }
        }
    }
    machCode<<endl;

    for (int i = 0; i < locationAddress; i++){              //printing the stored data in the machine code file
        long long int dataStartLocation = 268435456 + i;                  //0x10000000
        string st;
        stringstream sss;
        sss << hex << dataStartLocation;                        //converting to hexadecimal
        sss>>st;
        machCode <<"0x"<< st << "   "<< "0x" << mem[i] << endl;
    }
    machCode.close();
}

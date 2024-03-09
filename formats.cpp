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

string sFormatCommands(string l)
{
    string::iterator j, k;
    int rs1 = 0, rs2 = 0, rem = 0, imme = 0;
  
    stringstream s(l);
    string i, temp;
    s >> i;
    temp = map_op.find(i)->second; //funct3+opcode
    s >> i;                      //rs2
    rs2=0;
    //finding the nth numbered register example: x31 ---> we are finding out 31
    for (j = i.begin() + 1; j != i.end(); j++)
    {
        rs2 *= 10;
        rs2 += ((*j) - '0');
    }
    imme = 0;
    s >> i; //immed

    if(i[0]=='x'||i[0]=='('){
        cout<<" no offset/immediate field given"<<endl;
        exit(0);
    }

    // finding out the immediate offset example: 8(x10) ----> finding out 8
    for (j = i.begin(); (*j) != '('; j++)
    {
        imme *= 10;
        imme += ((*j) - 48);
    }

    rs1 = 0;
    //finding the nth numbered register example: x31 ---> we are finding out 31
    for (k = j + 2; (*k) != ')'; k++)
    {
        rs1 *= 10;
        rs1 += ((*k) - '0');
    }
    if(imme <-2048 && imme >=2048)
    {
       cout<<"Error : immediate is out of range(>= (-2^11) && <= (2^11 - 1)) which is "<<imme<<endl;
       exit(0);
    }  
    if (s >> i)
    {
        cout << "error :"<< " : got more than three arguments." << endl;
        exit(0);
    }
    if(rs1<0 || rs1>32 || rs2<0 || rs2>32){
        cout<<"Invalid register numbers given"<<endl;
        exit(0);
    }  
    string ans, a, fin, im1, im2, f, op;
    a = decToBinary(imme);
    a = convertToLength12(a);
    im1 = a.substr(7, 5);
    im2 = a.substr(0, 7);
    ans += im2;
    a = decToBinary(rs2);
    a = convertToLength5(a);
    ans += a;
    a = decToBinary(rs1);
    a = convertToLength5(a);
    ans += a;
    f = temp.substr(0, 3);
    ans += f;

    ans += im1;
    op = temp.substr(3, 7);
    ans += op;
    fin = binToHexa(ans);
    fin = "0x" + fin;

    string PC = binToHexa(decToBinary(programCounter));

    fin = "0x" + PC + "     " + fin;
    programCounter += 4;
    return fin;
}

string ujFormatCommands(string l)
{
    //jal x2 13
    //13(odd)=>12=>stored 6  if imm is odd, it is stored as imm/2 and then mutiplied by 2 .So, effectively, imm-1
    // imm[20]  imm[10:1]   imm [11]   imm [19:12]         rd      opcode
    //    0     0000000110     0        00000000          00010    1101111
    stringstream instruction(l);

    string word;
    instruction >> word;
    string func_and_opcode;
    func_and_opcode = map_op.find(word)->second; //  0110011(opcode)
    int rd, imm = 0;
    char j, i;
    instruction >> word;
    i = word[1];
    i = (int)i - 48;
    rd = i;
    j = (int)word[2] - 48;
    if (j < 10 && j >= 0)   rd = 10 * i + j;

    if(rd<0 || rd>32)
    {
        cout<<"Invalid register number range is [0,31]"<<endl;
        exit(0);
    }

    string dup_imm, x;
    stringstream DUPLICATE_instruction(l);
    DUPLICATE_instruction >> x;
    DUPLICATE_instruction >> x;
    DUPLICATE_instruction >> dup_imm;
    int dup_imm_start = (int)dup_imm[0];
    
    if (dup_imm.substr(0, 2) == "0b")
    { // positive binary
        dup_imm = dup_imm.substr(2, (dup_imm.length()) - 2);
        imm = binToDec(dup_imm);
        if (imm > 2047)
        {
            cout <<"Immediate is out of range which is"<<imm<< endl;
            exit(0);
        }
        if (imm == 0)
        {
            cout <<"Immediate value is 0 (offset should be > 0)" << endl;
            exit(0);
        }
    }
    else if (dup_imm.substr(0, 3) == "-0b")
    { // negative binary
        dup_imm = dup_imm.substr(3, (dup_imm.length()) - 3);
        imm = binToDec(dup_imm);
        if (imm > 2047)
        {
            cout <<"Immediate is out of range which is"<<imm<< endl;
            exit(0);
        }
        if (imm == 0)
        {
            cout <<"Immediate value is 0 (offset should be > 0)" << endl;
            exit(0);
        }
        imm*=-1;
    }
    else if (dup_imm.substr(0, 2) == "0x")
    { // positive hexadecimal
        dup_imm = dup_imm.substr(2, (dup_imm.length()) - 2);

        stringstream ss;
        ss << hex << dup_imm;
        ss >> imm;
        if (imm > 2047)
        {
            cout <<"Immediate is out of range which is"<<imm<< endl;
            exit(0);
        }
        if (imm == 0)
        {
            cout <<"Immediate value is 0 (offset should be > 0)" << endl;
            exit(0);
        }
    }
    else if (dup_imm.substr(0, 3) == "-0x")
    { // negative hexadecimal
        dup_imm = dup_imm.substr(3, (dup_imm.length()) - 3);

        stringstream ss;
        ss << hex << dup_imm;
        ss >> imm;
        if (imm > 2047)
        {
            cout <<"Immediate is out of range which is"<<imm<< endl;
            exit(0);
        }
        if (imm == 0)
        {
            cout <<"Immediate value is 0 (offset should be > 0)" << endl;
            exit(0);
        }
        imm*=-1;
    }
    else if (dup_imm_start <= 57 && dup_imm_start >= 48)
    {

        instruction >> imm;
        if (abs(imm)> 2047)
        {
            cout <<"Immediate is out of range which is"<<imm<< endl;
            exit(0);
        }
        if (imm == 0)
        {
            cout <<"Immediate value is 0 (offset should be > 0)" << endl;
            exit(0);
        }
    }
    else
    {
    int yatch=0;
    string lbl;
        map<string, int>::iterator itr; 
        for (itr = label.begin(); itr != label.end(); ++itr) { 
        
            
            stringstream ss(itr->first);
            ss>>lbl;
            if(lbl==dup_imm){
                yatch=1;
                imm=itr->second;
                imm=imm-programCounter;
            }
            
        }
        if(yatch==0){
            cout<<"This label "<<lbl<<"does not exist"<<endl;
            exit(0);
        }
    }
    
    imm /= 2;

    string machine_code, rout, imme, imme_20, imme_11, imme_10_to_1, imme_19_to_12, op;
    
    if(imm<0){
    //   cout<<"----------negative----------";
       stringstream ss; 
       ss<<hex<<imm;
       string hexv;
       ss>>hexv;
       imme=hexaToBinary(hexv);
       reverse(imme.begin(),imme.end());
       imme=imme.substr(0,20);
       reverse(imme.begin(),imme.end());
      // cout<<imme<<endl;
    }    

    else{imme = convertToLength20(decToBinary(imm));}
    rout = convertToLength5(decToBinary(rd));

    op = func_and_opcode;
    imme_20 = imme[0];
    imme_10_to_1 = imme.substr(10, 10);
    imme_11 = imme[9];
    imme_19_to_12 = imme.substr(1, 8);
    imme = imme_20 + imme_10_to_1 + imme_11 + imme_19_to_12;

    machine_code = imme + rout + op;
    machine_code = binToHexa(machine_code);
    machine_code = "0x" + machine_code;
    string PC = binToHexa(decToBinary(programCounter));

    machine_code = "0x" + PC + "     " + machine_code;

    programCounter += 4;
    return machine_code;
}

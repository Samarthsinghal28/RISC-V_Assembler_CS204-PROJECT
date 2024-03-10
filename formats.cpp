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

    string PC = "0x" + binToHexa(decToBinary(programCounter));

    machine_code = PC + "     " + machine_code;
    programCounter += 4;
    return machine_code;
}

string uFormatCommands(string line)
{
    stringstream instruction(line);
    string word;
    instruction >> word;
    string risc_code;
    risc_code = map_op.find(word)->second; 
    int reg, immed;
    char ones, tens;
    instruction >> word;
    tens = (int)word[1] - 48;
    reg = tens;
    ones = (int)word[2] - 48;
    if (ones < 10 && ones >= 0)
    {
        reg = 10 * tens + ones;
    }
    
    instruction >> word;
    int imm_begin = (int)word[0];
    
    if (word.substr(0, 2) == "0b")
    { // positive binary
        word = word.substr(2, word.length());
        if (word.length()>20){
            cout <<  "Immediate value ( " << immed << " ) out of range " << endl;
            exit(0);
        }
        immed = binToDec(word);
    }
    else if (word.substr(0, 2) == "0x")
    { // positive hexadecimal
        word = word.substr(2, word.length());
        stringstream ss;
        ss << hex << word;
        ss >> immed;
        if (immed > 1048575){
            cout <<  "Immediate value ( " << immed << " ) out of range " << endl;
            exit(0);
        }
    }
    else if (imm_begin <= 57 && imm_begin >= 48)
    {
        stringstream dummy(word);
        dummy >> immed;
        if (immed> 1048575){
            cout << "Immediate value ( " << immed << " ) out of range " << endl;
            exit(0);
        } 
    }
    if(reg<0 || reg>31)
    {
        cout<<"Invalid register numbers given"<<endl;
        exit(0);
    } 
    if (instruction >> word)
    {
        cout << "error : got three arguments(expected 2)." << endl;
        exit(0);
    } 

    string machine_code, rd, imm, op;

    imm = convertToLength20(decToBinary(immed));
    rd = convertToLength5(decToBinary(reg));

    op = risc_code;

    machine_code = "0x" + binToHexa(imm + rd + op);

    string PC = "0x" + binToHexa(decToBinary(programCounter));

    machine_code = PC + "     " + machine_code;
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

    if(i[0]=='x'||i[0]=='(')
    {
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
    stringstream instruction(l);

    string word;
    instruction >> word;
    string func_and_opcode;
    func_and_opcode = map_op.find(word)->second; 
    int rd, imm = 0;
    char j, i;
    instruction >> word;
    i = word[1] - '0';
    rd = i;
    j = word[2] - '0';
    if (j < 10 && j >= 0)   rd = 10 * i + j;

    if(rd<0 || rd>32)
    {
        cout<<"Invalid register number. range is [0,31]"<<endl;
        exit(0);
    }

    string imm2;
    instruction >> imm2;
    int imm2_start = (int)imm2[0];
    
    if (imm2.substr(0, 2) == "0b")
    { // positive binary
        imm2 = imm2.substr(2, (imm2.length()) - 2);
        imm = binToDec(imm2);
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
    else if (imm2.substr(0, 3) == "-0b")
    { // negative binary
        imm2 = imm2.substr(3, (imm2.length()) - 3);
        imm = binToDec(imm2);
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
    else if (imm2.substr(0, 2) == "0x")
    { // positive hexadecimal
        imm2 = imm2.substr(2, (imm2.length()) - 2);

        stringstream ss;
        ss << hex << imm2;
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
    else if (imm2.substr(0, 3) == "-0x")
    { // negative hexadecimal
        imm2 = imm2.substr(3, (imm2.length()) - 3);

        stringstream ss;
        ss << hex << imm2;
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
    else if (imm2_start <= 57 && imm2_start >= 48)
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
        bool label_found = false; // Variable to track if label is found
        string lbl;
        int imm = 0; // Variable to store the imm offset

        for (auto itr = label.begin(); itr != label.end(); ++itr) {
            stringstream ss(itr->first);
            ss >> lbl;

            if (lbl == imm2) {
                label_found = true;
                imm=itr->second;
                imm=imm-programCounter;
            }
        }

        if (!label_found) {
            cout<<"This label "<<lbl<<"does not exist"<<endl;
            exit(0);
        }
    }
    
    imm /= 2;

    string machine_code, rout, imme, imme_20, imme_11, imme_10_to_1, imme_19_to_12, op;
    
    if(imm<0)
    {
       stringstream ss; 
       ss<<hex<<imm;
       string hexv;
       ss>>hexv;
       imme=hexaToBinary(hexv);
       reverse(imme.begin(),imme.end());
       imme=imme.substr(0,20);
       reverse(imme.begin(),imme.end());
    }    

    else    imme = convertToLength20(decToBinary(imm));
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

string sbFormatCommands(string l)
{
    string::iterator j, k;
    int rs1 = 0, rs2 = 0, rem = 0, imme2 = 0;

    stringstream s(l);

    string i, temp;
    s >> i;
    temp = map_op.find(i)->second; 

    s >> i; 
    string PC = binToHexa(decToBinary(programCounter));
    if (i.substr(0, 1) != "x")
    {
        cout << "error in 0x" << PC << " :register " << i << " not recognised" << endl;
        exit(0);
    }
    string i_check = i.substr(1, i.length() - 1);
    regex re("[^0-9]");
    smatch match;
    if (regex_search(i_check, match, re) == true)
    { 
        cout << "error in 0x" << PC << " :register " << i << " not recognised" << endl;
        exit(0);
    }
    for (j = i.begin() + 1; j != i.end(); j++)
    {   
        rs2 *= 10;
        rs2 += ((*j) - 48);
        rem++;
    }
    rem = 0;
    if (rs2 > 31 || rs2 < 0)
    {
        cout << "error in 0x" << PC << " :register " << i << " not recognised" << endl;
        exit(0);
    }

    s >> i; //rs1
    if (i.substr(0, 1) != "x")
    {
        cout << "error in 0x" << PC << " :register " << i << " not recognised" << endl;
        exit(0);
    }
    i_check = i.substr(1, i.length() - 1);
    regex re1("[^0-9]");
    smatch match1;
    if (regex_search(i_check, match1, re1) == true)
    { 
        cout << "error in 0x" << PC << " :register " << i << " not recognised" << endl;
        exit(0);
    }
    for (j = i.begin() + 1; j != i.end(); j++)
    {
        rs1 *= 10;
        rs1  += ((*j) - 48);
        rem++;
    }
    rem = 0;
    if (rs1 > 31 || rs1 < 0)
    {
        cout << "error in 0x" << PC << " :register " << i << " not recognised" << endl;
        exit(0);
    }

    s >> i; //immed
    string ii;
    int imm = 0;

    if (i.length()>=2 && i.substr(0, 2) == "0b")
    { // positive binary
        ii = i.substr(2, (i.length()) - 2);
        imm = binToDec(ii);
        if (imm > 2047)
        {
            cout << "error in 0x" << PC << " :offset is not in range(immediate <= 2047) offset == "<<i<< endl;
            exit(0);
        }
        if (imm == 0)
        {
            cout << "error in 0x" << PC << " :infinite loop" << endl;
            exit(0);
        }
    }
    if (i.length() >= 3 && i.substr(0, 3) == "-0b")
    { // negative binary
        ii = i.substr(3, (i.length()) - 3);
        imm = binToDec(ii);
        if (imm > 2047)
        {
            cout << "error in 0x" << PC << " :offset is not in range(immediate <= 2047) offset == "<<i<< endl;
            exit(0);
        }
        if (imm == 0)
        {
            cout << "error in 0x" << PC << " :infinite loop" << endl;
            exit(0);
        }
        imm *= -1;
    }
    if (i.length() >= 2&&i.substr(0, 2) == "0x")
    { // positive hexadecimal
        ii = i.substr(2, (i.length()) - 2);

        stringstream ss;
        ss << hex << ii;
        ss >> imm;
        if (imm > 2047)
        {
            cout << "error in 0x" << PC << " :offset is not in range(immediate <= 2047) offset == "<<i<<endl;
            exit(0);
        }
        if (imm == 0)
        {
            cout << "error in 0x" << PC << " :infinite loop" << endl;
            exit(0);
        }
    }
    if (i.length() >= 3 && i.substr(0, 3) == "-0x")
    { // negative hexadecimal
        ii = i.substr(3, (i.length()) - 3);

        stringstream ss;
        ss << hex << ii;
        ss >> imm;
        if (imm > 2047)
        {
            cout << "error in 0x" << PC << " :offset " << i << " is not in range" << endl;
            exit(0);
        }
        if (imm == 0)
        {
            cout << "error in 0x" << PC << " :infinite loop" << endl;
            exit(0);
        }
        imm *= -1;
    }
    else
    {
        regex re1("[^0-9]");
        smatch m1;
        if (regex_search(i, m1, re1) == false)
        { // positive decimal
            stringstream ss2(i);
            ss2 >> imm;
            if (imm > 2047)
            {
                cout << "error in 0x" << PC << " :offset is not in range(immediate <= 2047) offset == "<<i<< endl;
                exit(0);
            }
            if (imm == 0)
            {
                cout << "error in 0x" << PC << " :infinite loop" << endl;
                exit(0);
            }
        }
        else
        {
            string ineg, ineg2;
            ineg = i.substr(0, 1);

            if (ineg == "-")
            {
                ineg2 = i.substr(1, (i.length()) - 1);
                regex re1("[^0-9]");
                smatch m1;
                if (regex_search(ineg2, m1, re1) == false)
                { 
                    stringstream ss2(ineg2);
                    ss2 >> imm;
                    if (imm > 2047)
                    {
                        cout << "error in 0x" << PC << " :offset is not in range(immediate <= 2047) offset == "<<i<< endl;
                        exit(0);
                    }
                    if (imm == 0)
                    {
                        cout << "error in 0x" << PC << " :infinite loop" << endl;
                        exit(0);
                    }
                    imm *= -1;
                }
            }
            else
            {            
                
                int pc_of_l1, current_pc = programCounter; //assuming pc_of_l1 is the pc of instruction following l1
                if (label.find(i) == label.end())
                {
                    cout << "label : " << i << " not found";
                    exit(0);
                }
                pc_of_l1 = label.find(i)->second;
                
                imm = pc_of_l1 - current_pc;
                if (imm < -2048 || imm > 2047)
                {
                    cout << "error in 0x" << PC << " :offset is not in range(immediate <= 2047 && immediate >= -2048) offset == "<<i<< endl;
                    exit(0);
                }
                if (imm == 0)
                {
                    cout << "error in 0x" << PC << " :infinite loop" << endl;
                    exit(0);
                }
                
            }
        }
    }
    if (s >> i)
    {
        cout << "error in 0x" << PC << " : got more than three arguments." << endl;
        exit(0);
    }
    string c2;
    imme2 = imm;
    if ((imme2 % 2) == 0)
        ;
    else
    {
        imme2 = imme2 - 1;
    }
    imme2 = imme2 >> 1;

    if (imme2 > 0)
    {
        c2 = decToBinary(imme2);
        c2 = convertToLength12(c2);
        
    }
    else
    {
        stringstream ss3;
        ss3 << hex << imme2;
        string res = ss3.str();
        string res2;
        res2 = res.substr(5, 3);
       
        c2 = hexaToBinary(res2);
    }
    string im1, im2, im3, im4;
    im1 = c2[0];
    im2 = c2.substr(2, 6);
    im3 = c2.substr(8, 4);
    im4 = c2[1];
    string rs2Str, rs1Str;
    rs2Str = decToBinary(rs2);
    rs2Str = convertToLength5(rs2Str); // rs2Str has rs2 in string form
    rs1Str = decToBinary(rs1);
    rs1Str = convertToLength5(rs1Str); // rs1Str has rs1 in string form
    string f3, op;
    f3 = temp.substr(0, 3);  // f3 has func3 in string form
    op = temp.substr(3, 7); // op2 has opcode in string form
    string machineCode;

    machineCode = im1 + im2 + rs1Str + rs2Str + f3 + im3 + im4 + op; 

    string ans;
    ans = binToHexa(machineCode);
    ans = "0x" + ans;
    
    string PC2 = binToHexa(decToBinary(programCounter));
    ans = "0x" + PC2 + "     " + ans;
    programCounter += 4;
    return ans;
}

string iFormatCommands(string line){
    // imm[11:0](12) rs1(5) func3(3) rd(5) opcode
    stringstream instruction(line);
    string word,type;
    instruction >> word;
    type = word;
    string risc_code;
    risc_code = map_op.find(word)->second;
    int r1,r2,immed;
    int flag=0;
    char ones, tens;
    string new1, new2;

    instruction >> word;
    tens = (int)word[1] - 48;
    r1 = tens;
    ones = (int)word[2] - 48;
    if (ones < 10 && ones >= 0){
        r1 = 10 * tens + ones;
    }

    if (type == "andi" || type == "addi" || type == "ori" || type == "jalr"){
        instruction >> word; //EXTRACT REGISTER 2
        tens = (int)word[1] - 48;
        r2 = tens;
        ones = (int)word[2] - 48;
        if (ones < 10 && ones >= 0){
            r2 = 10 * tens + ones;
        }
        instruction >> word; //EXTRACT IMMEDIATE VALUE
        if(word[0]=='x'){
            cout<<"The third argument has to be an immediate value,not a register"<<endl;
            exit(0);
        }
        if (word.substr(0, 2) == "0b"){ // positive binary
            word = word.substr(2, word.length());
            immed = binToDec(word);
            if (immed > 2047){
                cout << "Immediate " << immed << " is not in range" << endl;
                exit(0);
            }
        }
        else if (word.substr(0, 3) == "-0b"){ // negative binary
            word = word.substr(3, word.length());
            immed = binToDec(word);
            immed*=-1;
            if (immed < -2048){
                cout << "Immediate " << immed << " is not in range" << endl;
                exit(0);
            }
        }
        else if (word.substr(0, 2) == "0x"){ // positive hexadecimal
            word = word.substr(2, word.length());
            stringstream ss;
            ss << hex << word;
            ss >> immed;
            if (immed > 2047){
                cout << "Immediate " << immed << " is not in range" << endl;
                exit(0);
            }
        }
        else if (word.substr(0, 3) == "-0x"){ // negative hexadecimal
            word = word.substr(3, word.length());
            stringstream ss;
            ss << hex << word;
            ss >> immed;
            immed*=-1;
            if (immed < -2048){
                cout << "Immediate " << immed << " is not in range" << endl;
                exit(0);
            }  
        }
        else{     //DECIMAL NUMBERS
            stringstream dummy(word);      //addi x1,x1,-9
            dummy >> immed;
            if (immed > 2047 || immed < -2048){
                cout << "Immediate " << immed << " is not in range" << endl;
                exit(0);
            }
        }
    }
    else if (type == "lb" ||  type == "lw" || type == "lh"){
        instruction >> word;
        auto it = word.begin();
        if(word[0]=='x'|| word[0]=='('){
            cout<<" no offset/immediate field given"<<endl;
            exit(0);
        }
        else if (*it >= 48 && *it <= 57){
            immed=0;
            while ((*it) != '('){
                immed = (10 * immed) + ((*it) - 48); //offset
                it++;
            }
            
            it+=2;
            r2 =0;
            while ((*it) != ')'){ //lb x1 100(x2)
                r2 = 10 * r2 + ((*it) - 48);
                it++;
            }
        }
        else{                //lw x1 var1    
            flag = 1;
            string var;
            stringstream dummy_inst(line);
            dummy_inst>>var;//lw
            dummy_inst>>var;//x1
            new1 = "auipc " + var + " 65536";
            new2 = uFormatCommands(new1);     //auipc x1 65536
            dummy_inst>>var;//var1
            auto itr2 = label.begin();  
            while (itr2 != label.end()){ 
                if(itr2->first==var){
                    tpc=itr2->second;      
                }
                itr2++;
            }
            //offset_of_first_load_inst;
            if(!ship){
                immed = tpc-programCounter+4; 
                ship=1;
                offset_of_first_load_inst=immed;
            }
            else{
                immed=offset_of_first_load_inst-programCounter; 
            }
            stringstream dummy; 
            dummy<<hex<<immed;
            string hexv;
            dummy>>hexv;
            string im=hexaToBinary(hexv);
            im=im.substr(im.length()-12, im.length());
            new2 = new2 + "\r\n"; //result from auipc
            r2 = r1;
        }
    }
    if(immed <-2047 && immed >=2048){
       cout<<"Error : immediate ="<<immed<<" is out of range\n";
       exit(0);
    } 
    if (instruction >> word){
        cout << "error :"<< " : got more than three arguments." << endl;
        exit(0);
    }
    if(r2<0 || r2>31 || r1<0 || r1>31){
        cout<<"Invalid register numbers given"<<endl;
        exit(0);
    }  

    string machine_code;
    if(immed<0){
       stringstream ss; 
       ss<<hex<<immed;
       string hexv;
       ss>>hexv;
       string im=hexaToBinary(hexv);
       im=im.substr(im.length()-12, im.length());
       machine_code+=im;
    }
    else{ 
        string im = decToBinary(immed);
        im = convertToLength12(im);
        machine_code += im;
    }
    string temp;
    temp = decToBinary(r2);
    temp = convertToLength5(temp);
    machine_code += temp;
    temp = risc_code.substr(0, 3);
    machine_code += temp;
    temp = decToBinary(r1);
    temp = convertToLength5(temp);
    machine_code += temp;
    temp = risc_code.substr(3, 7);
    machine_code += temp;
    machine_code = "0x" + binToHexa(machine_code);

    string PC = binToHexa(decToBinary(programCounter));

    machine_code = "0x" + PC + "     " + machine_code;
    programCounter += 4;
    if (flag){
        machine_code = new2 + machine_code;
    }
    return machine_code;      
}
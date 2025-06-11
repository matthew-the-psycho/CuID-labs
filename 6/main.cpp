#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

#define jmp goto

#define WHITE 0
#define MAGENTA 1
#define RED 2
#define GREEN 3

#ifdef _WIN32
#include <windows.h>
void MEIN_SETCOLOR(int COLOR) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    switch(COLOR) {
        case 0:
            SetConsoleTextAttribute(hConsole, 15);
            break;
        case 1:
            SetConsoleTextAttribute(hConsole, 13);
            break;
        case 2:
            SetConsoleTextAttribute(hConsole, 12);
            break;
        case 3:
            SetConsoleTextAttribute(hConsole, 10);
            break;
        default:
            break;
    }
    return;
}
#else
void MEIN_SETCOLOR(int COLOR) {
    switch(COLOR) {
        case 0:
            cout << "\033[38;5;255m";
            break;
        case 1:
            cout << "\033[38;5;200m";
            break;
        case 2:
            cout << "\033[38;5;1m";
            break;
        case 3:
            cout << "\033[38;5;40m";
            break;
        default:
            break;
    }
    return;
}
#endif

void checkpoint(const char* line) {
    MEIN_SETCOLOR(GREEN);
    cout << line;
    MEIN_SETCOLOR(MAGENTA);
    return;
}

unsigned char RKT = 0;
unsigned char if_fix;
unsigned int BASE_NUM;
unsigned int MOD_BASE;
unsigned long long TRACER_MUL;

int CHECK_KEY_FILE(string KFN) {
    ifstream hKF;
    hKF.open(KFN, ios::in);
    int ret = 0;
    if(hKF.is_open() == true) {
        return 1;
    } else {
        return 0;
    }
    while(!hKF.eof()) {
        char bf;
        hKF.read(&bf,1);
        ret++;
    }
    hKF.close();
    return ret;
}

void RESET_RANDOM() {
    srand(time(0));
    return;
}

void SECRETKEY_ENLOAD() {
    BASE_NUM = 3;
    MOD_BASE = 0x11;
    return;
}

unsigned char ENCRYPTOR(unsigned int OwnCharacter, unsigned int BaseCharacter) {
    unsigned int PubCharacter;
    unsigned int eax = 1;
    unsigned int base_eax = BaseCharacter;
    unsigned int flag_used;
    printf("\nStarted by :: \nBase :: %d; Pow :: %d; mod :: %d\n", BaseCharacter, OwnCharacter, MOD_BASE);
    for (unsigned char i = 0x0; i < 0x20; i++) {
        flag_used = 0x0;
        unsigned int moved = 0b00000001 << i;
        unsigned int edge_crutch = OwnCharacter & moved;
        if(edge_crutch != 0) {
            //base_eax += eax;
            eax = eax * (base_eax % MOD_BASE) % MOD_BASE;
            TRACER_MUL++;
            //base_eax += eax;
            flag_used = 0x1;
        }
        //printf("                                                                pow :: %d; base_eax :: %d; eax :: %d; used :: %d\n", 0b00000001 << i, base_eax, eax, flag_used);
        base_eax *= base_eax;
        TRACER_MUL++;
        base_eax = base_eax % MOD_BASE;
        
        printf("    pow :: %d; base_eax :: %d; eax :: %d; used :: %d\n", i, base_eax, eax, flag_used);
    }
    PubCharacter = eax;
    printf("Base :: %d; Pow :: %d; Res :: %d\n", BaseCharacter, OwnCharacter, PubCharacter);
    return PubCharacter;
}

void KEY_REGENERATOR(string kFile, string bFile) {
    ofstream hOut;
    hOut.open(kFile, ios::out | ios:: trunc | ios::binary);
    if(if_fix != 0) {
        //First Own, First Pub, Sec Own, Sec Pub;
        unsigned char FO, FP, SO, SP;
        FO = 0b00001111;
        FP = ENCRYPTOR(FO, BASE_NUM);
        SO = 0b00001101;
        SP = ENCRYPTOR(SO, BASE_NUM);
        hOut << FO << FP << SO << SP;
    } else {
        unsigned char FO, FP, SO, SP;
        FO = rand() % 0x100;
        FP = ENCRYPTOR(FO, BASE_NUM);
        SO = rand() % 0x100;
        SP = ENCRYPTOR(SO, BASE_NUM);
        hOut << FO << FP << SO << SP;
    }
    hOut.close();
    checkpoint("keys re-/generated!\n");
    return;
}

void KEYLOADER(string kFile, unsigned char *place) {
    ifstream hIn;
    hIn.open(kFile, ios::in | ios::binary);
    for(int i = 0b00000000; i < 0b00000100; i++) {
        char CRUTCH;
        hIn.read(&CRUTCH, 1);
        place[i] = (unsigned char)CRUTCH;
    }
    hIn.close();
    return;
}

string UC2S(unsigned char num) {
    string res = "";
    unsigned char fh, sh;
    fh = (num & (0xF << 4)) >> 4;
    sh = num & 0xF;
    switch(fh) {
        case 0x0:
            res += "0";
            break;
        case 0x1:
            res += "1";
            break;
        case 0x2:
            res += "2";
            break;
        case 0x3:
            res += "3";
            break;
        case 0x4:
            res += "4";
            break;
        case 0x5:
            res += "5";
            break;
        case 0x6:
            res += "6";
            break;
        case 0x7:
            res += "7";
            break;
        case 0x8:
            res += "8";
            break;
        case 0x9:
            res += "9";
            break;
        case 0xA:
            res += "A";
            break;
        case 0xB:
            res += "B";
            break;
        case 0xC:
            res += "C";
            break;
        case 0xD:
            res += "D";
            break;
        case 0xE:
            res += "E";
            break;
        case 0xF:
            res += "F";
            break;
        default:
            break;
    }
    switch(sh) {
        case 0x0:
            res += "0";
            break;
        case 0x1:
            res += "1";
            break;
        case 0x2:
            res += "2";
            break;
        case 0x3:
            res += "3";
            break;
        case 0x4:
            res += "4";
            break;
        case 0x5:
            res += "5";
            break;
        case 0x6:
            res += "6";
            break;
        case 0x7:
            res += "7";
            break;
        case 0x8:
            res += "8";
            break;
        case 0x9:
            res += "9";
            break;
        case 0xA:
            res += "A";
            break;
        case 0xB:
            res += "B";
            break;
        case 0xC:
            res += "C";
            break;
        case 0xD:
            res += "D";
            break;
        case 0xE:
            res += "E";
            break;
        case 0xF:
            res += "F";
            break;
        default:
            break;
    }
    return res;
}

unsigned char if_decode;

void POLYCRYPTO(string FileIn, string FileOut, string kFile) {
    unsigned char KEYS[0b00000100];
    KEYLOADER(kFile, KEYS);
    ifstream hIn;
    ofstream hOut;
    hIn.open(FileIn, ios::in | ios::binary);
    hOut.open(FileOut, ios::out | ios::binary | ios::trunc);
    unsigned char SKey[2];
    SKey[0] = ENCRYPTOR(KEYS[0], KEYS[3]);
    SKey[1] = ENCRYPTOR(KEYS[2], KEYS[1]);
    if (SKey[0] != SKey[1]) {
        MEIN_SETCOLOR(RED);
        cout << "Problem occured :: Keys aren't equal!\n";
        MEIN_SETCOLOR(WHITE);
    }
    cout << "SKey[0] == " << UC2S(SKey[0]) << ";    SKey[1] == " << UC2S(SKey[1]) << endl;
    int local_counter = 0;
    hIn.seekg(0, hIn.end);
    long long INFILELENGTH = hIn.tellg();
    hIn.seekg(0, hIn.beg);
    long long JCTR = 0;
    while (!hIn.eof() & JCTR < INFILELENGTH) {
        char CRUTCH;
        hIn.read(&CRUTCH, 1);
        unsigned char res = (unsigned char) CRUTCH;
        if(if_decode == 0) {
            res = res ^ ((unsigned char)local_counter + SKey[0]);
        } else {
            res = res ^ ((unsigned char)local_counter + SKey[1]);
        }
        hOut << res;
        JCTR++;
    }
    hIn.close();
    hOut.close();
    return;
}

void ENCODE(string FileName, string KeyFile, int regenerate_need) {
    string FileOut = "";
    for (int i = 0; FileName[i] != '.'; i++) {
        FileOut += FileName[i];
    }
    FileOut += ".enc";
    if (regenerate_need == 1) {
        checkpoint("KEY_Regenerator called\n");
        KEY_REGENERATOR(KeyFile, FileName);
    }
    POLYCRYPTO(FileName, FileOut, KeyFile);
    return;
}

void DECODE(string FileName, string KeyFile) {
    string FileOut = "";
    for (int i = 0; FileName[i] != '.'; i++) {
        FileOut += FileName[i];
    }
    FileOut += ".dcd";
    POLYCRYPTO(FileName, FileOut, KeyFile);
    return;
}

void FILE_PROCESSER(string FNI, string RGMNT) {
    string KeyFileName = "";
    for (int i = 0; FNI[i] != '.'; i++) {
        KeyFileName += FNI[i];
    }
    int IKE = 0;
    KeyFileName += ".key";
    IKE = CHECK_KEY_FILE(KeyFileName);
    //checkpoint("KEY FILE NAME\n");
    //cout << "IKE == " << IKE << endl;
    SECRETKEY_ENLOAD();
    if (RGMNT == "--code") {
        checkpoint("--code process\n");
        if (RKT != 0 || IKE == 0) {
            checkpoint("Need key-reload 1\n");
            ENCODE(FNI, KeyFileName, 1);
        } else {
            checkpoint("Need key-reload 0\n");
            cout << "Key File is " << KeyFileName << endl;
            ENCODE(FNI, KeyFileName, 0);
        }
    } else 
    if (RGMNT == "--decode") {
        if (IKE == 0) {
            MEIN_SETCOLOR(RED);
            cout << "Isn't possible to DECODE :: .key-file doesN'T EXIST!\n";
            MEIN_SETCOLOR(MAGENTA);
        } else {
            DECODE(FNI, KeyFileName);
        }
    }
    return;
}

int main(int argc, const char* argv[]) {
    MEIN_SETCOLOR(MAGENTA);
    string file = "";
    string regiment = "";
    string type_flagge = "";
    string regenerator_flag = "";
    unsigned char rediska_flag = 0b00000000; 
    TRACER_MUL = 0;
    if (argc < 4 || argc > 5) {
        MEIN_SETCOLOR(RED);
        cout << "argc == " << argc << endl;
        cout << "\nUSAGE::\n<.exe/.elf>\n<file to process>\n<--code or --decode>\n<--type-rand/--type-fix>\n<--regenerate-key>\n";
        jmp wychod_point;
    }
    //checkpoint("First Errors\n");
    if (argc == 4) {
        for(int i = 0; argv[1][i] != 0; i++) {
            file += argv[1][i];
        }
        for(int i = 0; argv[2][i] != 0; i++) {
            regiment += argv[2][i];
        }
        for(int i = 0; argv[3][i] != 0; i++) {
            type_flagge += argv[3][i];
        }
    } else {
        for(int i = 0; argv[1][i] != 0; i++) {
            file += argv[1][i];
        }
        for(int i = 0; argv[2][i] != 0; i++) {
            regiment += argv[2][i];
        }
        for(int i = 0; argv[3][i] != 0; i++) {
            type_flagge += argv[3][i];
        }
        for(int i = 0; argv[4][i] != 0; i++) {
            regenerator_flag += argv[4][i];
        }
    }
    //checkpoint("args init\n");
    if ((regiment != "--code" && regiment != "--decode")) {
        rediska_flag |= 0b00000001;
    }
    if ((type_flagge != "--type-rand" && type_flagge != "--type-fix")) {
        rediska_flag |= 0b00000100;
    }
    //checkpoint("regiment flag\n");
    if (argc == 5 && regenerator_flag != "--regenerate-key") {
        rediska_flag |= 0b00000010;
    }
    //checkpoint("regen flag\n");
    //cout << "redis_flag is " << (int)rediska_flag << endl;
    if (rediska_flag > 0b00000000) {
        MEIN_SETCOLOR(RED);
        cout << "Caused Problemenn\n";
        switch(rediska_flag) {
            case 0b00000001:
                cout << "\nWRONG USAGE CODE/DECODE FLAG!\n";
                jmp wychod_point;
                break;
            case 0b00000010:
                cout << "\nWRONG USAGE KEY REGENERATOR FLAG!\n";
                jmp wychod_point;
                break;
            case 0b00000011:
                cout << "\nWRONG USAGE FLAGS :: CODE/DECODE, KEY-REGENERATOR\n";
                jmp wychod_point;
                break;
            case 0b00000100:
                cout << "\nWRONG USAGE FLAGS :: TYPE FLAG\n";
                jmp wychod_point;
                break;
            case 0b00000101:
                cout << "\nWRONG USAGE FLAGS :: CODE/DECODE, TYPE FLAG\n";
                jmp wychod_point;
                break;
            case 0b00000110:
                cout << "\nWRONG USAGE FLAGS :: TYPE FLAG, KEY-REGENERATOR\n";
                jmp wychod_point;
                break;
            case 0b00000111:
                cout << "\nWRONG USAGE FLAGS :: Full HAUS! :: CODE/DECODE, TYPE FLAG, KEY-REGENERATOR\n";
                jmp wychod_point;
                break;
        }
    } else {
    //    cout << "First 3 args initialized!\n";
    }
    //checkpoint("problems out\n");
    if (argc == 5 && regenerator_flag == "--regenerate-key") {
        RKT++;
    }
    if (type_flagge == "--type-rand") {
        if_fix = 0;
    }
    if (type_flagge == "--type-fix") {
        if_fix = 1;
    }
    if (regiment == "--decode") {
        if_decode = 1;
    } else {
        if_decode = 0;
    }
    FILE_PROCESSER(file, regiment);
    cout << endl << "Resulted MULTIPLY counted is " << TRACER_MUL << endl;
    wychod_point:
    MEIN_SETCOLOR(WHITE);
    return 0;
}
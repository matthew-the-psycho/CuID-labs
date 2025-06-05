#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>
#include <openssl\sha.h>

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
    switch (COLOR) {
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
    switch (COLOR) {
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
unsigned char hasherwise = 0b00000000; //128/256/512 bit for .hash.key
unsigned char if_threadtype = 0;
unsigned char type_size = 0x100 / 0x008;

string HASH128(const string str) {
    unsigned char hash[SHA_DIGEST_LENGTH];
    SHA_CTX sha1;
    SHA1_Init(&sha1);
    SHA1_Update(&sha1, str.c_str(), str.size());
    SHA1_Final(hash, &sha1);
    stringstream ss;
    for (int i = 0; i < SHA_DIGEST_LENGTH; i++) {
        ss << hex << (int)hash[i];
    }
    return ss.str();
}

string HASH256(const string str) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    stringstream ss;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        ss << hex << (int)hash[i];
    }
    return ss.str();
}

string HASH512(const string str) {
    unsigned char hash[SHA512_DIGEST_LENGTH];
    SHA512_CTX sha512;
    SHA512_Init(&sha512);
    SHA512_Update(&sha512, str.c_str(), str.size());
    SHA512_Final(hash, &sha512);
    stringstream ss;
    for (int i = 0; i < SHA512_DIGEST_LENGTH; i++) {
        ss << hex << (int)hash[i];
    }
    return ss.str();
}

void RESET_RANDOM() {
    srand(time(0));
    return;
}

int CHECK_KEY_FILE(string KFN) {
    ifstream hKF;
    hKF.open(KFN, ios::in);
    int ret = 0;
    if (hKF.is_open() == true) {
        return 1;
    }
    else {
        return 0;
    }
    while (!hKF.eof()) {
        char bf;
        hKF.read(&bf, 1);
        ret++;
    }
    hKF.close();
    return ret;
}

unsigned char BYTESTR2UCHAR(string bytee) {
    string fh = "";
    string sh = "";
    string voc = "0123456789abcdef";
    string VOC = "0123456789ABCDEF";
    fh += bytee[0];
    sh += bytee[1];
    unsigned char result = 0;
    if (voc.find(fh) >= 0) {
        result += (unsigned char)(voc.find(fh) << 4);
    }
    else {
        result += (unsigned char)(VOC.find(fh) << 4);
    }
    if (voc.find(sh) >= 0) {
        result += (unsigned char)(voc.find(sh));
    }
    else {
        result += (unsigned char)(VOC.find(sh));
    }
    return result;
}

void STR2UCHAR(string hash, unsigned char* buffer, int length) {
    int counter = 0;
    for (int i = 0; i < hash.length() && counter < length; i += 2) {
        string byter = "";
        byter += hash[i];
        byter += hash[i + 1];
        buffer[counter] = BYTESTR2UCHAR(byter);
        counter++;
    }
    return;
}

void KEY128(string KEYFILE, string BASEFILE) {
    ofstream hKey;
    ifstream hIn;
    hKey.open(KEYFILE, ios::trunc | ios::binary);
    hIn.open(BASEFILE, ios::in);
    string mid_hash = "";
    while (!hIn.eof()) {
        string str = "";
        getline(hIn, str);
        mid_hash += str;
        mid_hash = HASH128(mid_hash);
    }
    mid_hash += KEYFILE;
    mid_hash = HASH128(mid_hash);
    unsigned char uint8_hash[type_size] = { 0 };
    STR2UCHAR(mid_hash, uint8_hash, type_size);
    for (int i = 0; i < type_size; i++) {
        hKey << uint8_hash[i];
    }
    hIn.close();
    hKey.close();
    return;
}

void KEY256(string KEYFILE, string BASEFILE) {
    ofstream hKey;
    ifstream hIn;
    hKey.open(KEYFILE, ios::trunc | ios::binary);
    hIn.open(BASEFILE, ios::in);
    string mid_hash = "";
    while (!hIn.eof()) {
        string str = "";
        getline(hIn, str);
        mid_hash += str;
        mid_hash = HASH256(mid_hash);
    }
    mid_hash += KEYFILE;
    mid_hash = HASH256(mid_hash);
    unsigned char uint8_hash[type_size] = { 0 };
    STR2UCHAR(mid_hash, uint8_hash, type_size);
    for (int i = 0; i < type_size; i++) {
        hKey << uint8_hash[i];
    }
    hIn.close();
    hKey.close();
    return;
}

void KEY512(string KEYFILE, string BASEFILE) {
    ofstream hKey;
    ifstream hIn;
    hKey.open(KEYFILE, ios::trunc | ios::binary);
    hIn.open(BASEFILE, ios::in);
    string mid_hash = "";
    while (!hIn.eof()) {
        string str = "";
        getline(hIn, str);
        mid_hash += str;
        mid_hash = HASH512(mid_hash);
    }
    mid_hash += KEYFILE;
    mid_hash = HASH512(mid_hash);
    unsigned char uint8_hash[type_size] = { 0 };
    STR2UCHAR(mid_hash, uint8_hash, type_size);
    for (int i = 0; i < type_size; i++) {
        hKey << uint8_hash[i];
    }
    hIn.close();
    hKey.close();
    return;
}

void KEY_REGENERATOR(string KeyFile, string BaseFile) {
    //if (hasherwise & 0b10000000 == 0) {
    if (hasherwise == 0) {
        RESET_RANDOM();
        checkpoint("RESETRANDOM\n");
        ofstream hKey;
        hKey.open(KeyFile, ios::trunc | ios::binary);
        for (int i = 0; i < type_size; i++) {
            hKey << (char)(rand() % 0x100);
        }
        hKey.close();
        checkpoint("KeyFile loaded!\n");
    }
    else {
        checkpoint("Hasherwise isn't null\n");
        cout << "Hasherwise :: " << (int)hasherwise << endl;
        switch (hasherwise & 0b00000011) {
        case 0b00000001:
            KEY128(KeyFile, BaseFile);
            break;
        case 0b00000010:
            KEY256(KeyFile, BaseFile);
            break;
        case 0b00000011:
            KEY512(KeyFile, BaseFile);
            break;
        default:
            break;
        }
    }
    return;
}

void BLOCKWISE(string FNI, string FNO, string KF) {
    ifstream hIn(FNI, ios::binary | ios::in);
    ofstream hOut(FNO, ios::binary | ios::out);
    ifstream hKey(KF, ios::binary | ios::in);
    vector<unsigned char> GENUINE_KEY;
    vector<unsigned int> DRIFTVECTOR;
    vector<unsigned char> CURRENT_KEY;
    while (!hKey.eof()) {
        char key_byte;
        hKey.read(&key_byte, 1);
        GENUINE_KEY.push_back((unsigned char)key_byte);
        CURRENT_KEY.push_back((unsigned char)key_byte);
    }
    DRIFTVECTOR.push_back(0);
    int counter = 0;
    hIn.seekg(0, hIn.end);
    long long INFILELENGTH = hIn.tellg();
    hIn.seekg(0, hIn.beg);
    long long JCTR = 0;
    unsigned char stoppe = 0;
    while (!hIn.eof() && !stoppe) {
        unsigned char res[type_size] = { 0 };
        char buf[type_size] = { 0 };
        hIn.read(buf, type_size);
        for (int i = 0; i < type_size; i++) {
            CURRENT_KEY[i] = GENUINE_KEY[(i + DRIFTVECTOR[counter]) % type_size];
        }
        for (int i = 0; i < type_size; i++) {
            res[i] = (unsigned char)buf[i] ^ CURRENT_KEY[i];
            if (JCTR < INFILELENGTH) {
                hOut << res[i];
            }
            JCTR++;
        }
        DRIFTVECTOR.push_back((res[0] + (unsigned char)buf[0]) % type_size);
        counter++;
    }
    checkpoint("Rounder vector :: \n");
    for (int i = 0; i < counter; i++) {
        cout << DRIFTVECTOR[i] << ' ';
    }
    cout << endl;
    hIn.close();
    hOut.close();
    hKey.close();
    return;
}

void THREADWISE(string FNI, string FNO, string KF) {
    ifstream hIn(FNI, ios::binary | ios::in);
    ofstream hOut(FNO, ios::binary | ios::out);
    ifstream hKey(KF, ios::binary | ios::in);
    vector<unsigned char> GENUINE_KEY;
    vector<unsigned int> DRIFTVECTOR;
    while (!hKey.eof()) {
        char key_byte;
        hKey.read(&key_byte, 1);
        GENUINE_KEY.push_back((unsigned char)key_byte);
    }
    DRIFTVECTOR.push_back(0);
    int counter = 0;
    hIn.seekg(0, hIn.end);
    long long INFILELENGTH = hIn.tellg();
    hIn.seekg(0, hIn.beg);
    long long JCTR = 0;
    while (!hIn.eof() && JCTR < INFILELENGTH) {
        unsigned char res;
        char buf;
        hIn.read(&buf, 1);
        res = (unsigned char)buf ^ GENUINE_KEY[DRIFTVECTOR[counter]];
        DRIFTVECTOR.push_back((res + (unsigned char)buf) % type_size);
        counter++;
        hOut << res;
        JCTR++;
    }
    checkpoint("Rounder vector :: \n");
    for (int i = 0; i < counter; i++) {
        cout << DRIFTVECTOR[i] << ' ';
    }
    cout << endl;
    hIn.close();
    hOut.close();
    hKey.close();
    return;
}

void ENCODE(string FileName, string KeyFile, int regenerate_need) {
    string FileOut = "";
    for (int i = 0; FileName[i] != '.'; i++) {
        FileOut += FileName[i];
    }
    if (hasherwise != 0) {
        FileOut += ".hash.enc";
    }
    else {
        FileOut += ".rand.enc";
    }
    if (regenerate_need == 1 || hasherwise & 0b10000000 != 0) {
        checkpoint("KEY_Regenerator called\n");
        KEY_REGENERATOR(KeyFile, FileName);
    }
    if (if_threadtype == 1) {
        THREADWISE(FileName, FileOut, KeyFile);
    }
    else {
        BLOCKWISE(FileName, FileOut, KeyFile);
    }
    return;
}

void DECODE(string FileName, string KeyFile) {
    string FileOut = "";
    for (int i = 0; FileName[i] != '.'; i++) {
        FileOut += FileName[i];
    }
    if (hasherwise != 0) {
        FileOut += ".hash.dcd";
    }
    else {
        FileOut += ".rand.dcd";
    }
    if (if_threadtype == 1) {
        THREADWISE(FileName, FileOut, KeyFile);
    }
    else {
        BLOCKWISE(FileName, FileOut, KeyFile);
    }
    return;
}

void FILE_PROCESSER(string FNI, string RGMNT) {
    string KeyFileName = "";
    for (int i = 0; FNI[i] != '.'; i++) {
        KeyFileName += FNI[i];
    }
    int IKE = 0;
    if (hasherwise != 0) {
        KeyFileName += ".hash.key";
    }
    else {
        KeyFileName += ".rand.key";
    }
    IKE = CHECK_KEY_FILE(KeyFileName);
    //checkpoint("KEY FILE NAME\n");
    cout << "IKE == " << IKE << endl;
    if (RGMNT == "--code") {
        checkpoint("--code process\n");
        if (RKT != 0 || IKE == 0) {
            checkpoint("Need key-reload 1\n");
            ENCODE(FNI, KeyFileName, 1);
        }
        else {
            checkpoint("Need key-reload 0\n");
            cout << "Key File is " << KeyFileName << endl;
            cout << "Hasherwise :: " << (int)hasherwise << endl;
            ENCODE(FNI, KeyFileName, 0);
        }
    }
    else
        if (RGMNT == "--decode") {
            if (IKE == 0) {
                MEIN_SETCOLOR(RED);
                cout << "Isn't possible to DECODE :: .key-file doesN'T EXIST!\n";
                MEIN_SETCOLOR(MAGENTA);
            }
            else {
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
    if (argc < 4 || argc > 5) {
        MEIN_SETCOLOR(RED);
        cout << "argc == " << argc << endl;
        cout << "\nUSAGE::\n<.exe/.elf>\n<file to process>\n<--code or --decode>\n<--type-thread-rand/--type-thread-sha128/--type-thread-sha256/--type-thread-sha512/--type-block-rand/--type-block-sha128/--type-block-sha256/--type-block-sha512>\n<--regenerate-key>\n";
        cout << "// <--regenerate-key is usable for --type-rand>\n";
        jmp wychod_point;
    }
    //checkpoint("First Errors\n");
    if (argc == 4) {
        for (int i = 0; argv[1][i] != 0; i++) {
            file += argv[1][i];
        }
        for (int i = 0; argv[2][i] != 0; i++) {
            regiment += argv[2][i];
        }
        for (int i = 0; argv[3][i] != 0; i++) {
            type_flagge += argv[3][i];
        }
    }
    else {
        for (int i = 0; argv[1][i] != 0; i++) {
            file += argv[1][i];
        }
        for (int i = 0; argv[2][i] != 0; i++) {
            regiment += argv[2][i];
        }
        for (int i = 0; argv[3][i] != 0; i++) {
            type_flagge += argv[3][i];
        }
        for (int i = 0; argv[4][i] != 0; i++) {
            regenerator_flag += argv[4][i];
        }
    }
    //checkpoint("args init\n");
    if ((regiment != "--code" && regiment != "--decode")) {
        rediska_flag |= 0b00000001;
    }
    if ((type_flagge != "--type-thread-rand" && type_flagge != "--type-thread-sha128" && type_flagge != "--type-thread-sha256" && type_flagge != "--type-thread-sha512" && type_flagge != "--type-block-rand" && type_flagge != "--type-block-sha128" && type_flagge != "--type-block-sha256" && type_flagge != "--type-block-sha512")) {
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
        switch (rediska_flag) {
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
    }
    else {
        //    cout << "First 3 args initialized!\n";
    }
    //checkpoint("problems out\n");
    if (argc == 5 && regenerator_flag == "--regenerate-key") {
        RKT++;
    }
    if (type_flagge == "--type-thread-rand") {
        hasherwise = 0b00000000;
        if_threadtype = 1;
    }
    if (type_flagge == "--type-thread-sha128") {
        hasherwise = 0b10000001;
        if_threadtype = 1;
        type_size = 0x80 / 0x08;
    }
    if (type_flagge == "--type-thread-sha256") {
        hasherwise = 0b10000010;
        if_threadtype = 1;
    }
    if (type_flagge == "--type-thread-sha512") {
        hasherwise = 0b10000011;
        if_threadtype = 1;
        type_size = 0x200 / 0x008;
    }
    if (type_flagge == "--type-block-rand") {
        hasherwise = 0b00000000;
    }
    if (type_flagge == "--type-block-sha128") {
        hasherwise = 0b10000001;
        type_size = 0x80 / 0x08;
    }
    if (type_flagge == "--type-block-sha256") {
        hasherwise = 0b10000010;
    }
    if (type_flagge == "--type-block-sha512") {
        hasherwise = 0b10000011;
        type_size = 0x200 / 0x008;
    }
    FILE_PROCESSER(file, regiment);
wychod_point:
    MEIN_SETCOLOR(WHITE);
    return 0;
}
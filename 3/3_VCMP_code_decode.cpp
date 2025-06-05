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

void RESET_RANDOM() {
    srand(time(0));
    return;
}

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

void KEY_REGENERATOR(string KeyFile, int length){
    RESET_RANDOM();
    ofstream hKey;
    hKey.open(KeyFile, ios::trunc | ios::binary);
    for (int i = 0; i < length; i++) {
        hKey << (char)(rand() % 0x100);
    }
    hKey.close();
    return;
}

void VMPC(string FNI, string FNO, string KF){//VMPC-поточный шифр, модификация RC4
    //так же состоит из инициализации s-блока и формирования псевдослучайного слова на каждый байт текста
    ifstream hIn(FNI, ios::binary | ios::in);//исходный файл
    ofstream hOut(FNO, ios::binary | ios::out);//шифротекст
    ifstream hKey(KF, ios::binary | ios::in);//файл с ключом
    if ((!hIn.is_open()) || (!hOut.is_open()) || (!hKey.is_open())) {
        MEIN_SETCOLOR(RED);
        cout << "ERROR OF OPENING FILES :: INPUT | OUT | KEY\n";
        MEIN_SETCOLOR(MAGENTA);
        return;
    }
    vector<unsigned char> s(0x100);//s-блок
    for (int i = 0; i < 0x100; i++) {
        s[i] = i;//начальная инициализация s-блока
    }
    int i, j = 0;
    vector<unsigned char> keyarr(0x100);//массив ключа
    char keybyte;
    int ind = 0;
    while (hKey.read(&keybyte, sizeof(char)) && ind < 0x100) {
        //считываем ключ в массив
        keyarr[ind++]=keybyte;
    }
    int p = 0;
    for (i = 0; i < 0x299; i++) {//тасуем s-блок (продолжение инициализации)
        j = i % 0x100;//делим по модулю i на 256, чтобы не выходить за пределы массива
        p = s[(p + s[j] + keyarr[i % 0x100]) % 0x100];//получаем индекс элемента, который мы поменяем местами с j-тым
        swap(s[j], s[p]);//меняем местами два байта s-блока
    }
    i = j = 0;
    char byte;//вторая часть алгоритма - формирование псевдослучайного слова для побайтного шифрования
    while(hIn.read(&byte, sizeof(char))) {
        p = s[(p + s[i]) % 0x100];//вспомогательный индекс для выбора байта - псевдослучайного слова
        unsigned char symb = s[(s[s[p]] + 1) % 0x100];//байт (псевдослучайное слово)
        swap(s[i], s[p]);//тасуем байты в s-блоке
        i = (i + 1) % 0x100;//делим на 256 по модулю, чтобы не выходить за границы массива
        char resbyte = symb ^ byte;//xor'им байт из s-блока с байтом исходного текста
        hOut.write(&resbyte, sizeof(char));//результат записываем в шифротекст
    }  
    hIn.close();
    hOut.close();
    hKey.close();
    return;
}

void ENCODE(string FileName, string KeyFile, int regenerate_need) {
    if (regenerate_need == 1) {
        KEY_REGENERATOR(KeyFile, 0x100 / 0x008); //generate key of 256 bit, not byte
    }
    string FileOut = "";
    for (int i = 0; FileName[i] != '.'; i++) {
        FileOut += FileName[i];
    }
    FileOut += ".vmpc.enc";
    VMPC(FileName, FileOut, KeyFile);
    return;
}

void DECODE(string FileName, string KeyFile) {
    string FileOut = "";
    for (int i = 0; FileName[i] != '.'; i++) {
        FileOut += FileName[i];
    }
    FileOut += ".vmpc.dcd";
    ifstream hIn, hKey;
    ofstream hOut;
    VMPC(FileName, FileOut, KeyFile);
    return;
}

void FILE_PROCESSER(string FNI, string RGMNT) {
    string KeyFileName = "";
    for (int i = 0; FNI[i] != '.'; i++) {
        KeyFileName += FNI[i];
    }
    KeyFileName += ".vmpc.key";
    //checkpoint("KEY FILE NAME\n");
    int IKE = CHECK_KEY_FILE(KeyFileName);
    if (RGMNT == "--code") {
        if (RKT != 0 || IKE == 0) {
            ENCODE(FNI, KeyFileName, 1);
        } else {
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
    string regenerator_flag = "";
    unsigned char rediska_flag = 0b00000000; 
    if (argc < 3 || argc > 4) {
        MEIN_SETCOLOR(RED);
        cout << "argc == " << argc << endl;
        cout << "\nUSAGE: <.exe/.elf> <file to process> <--code or --decode> <--regenerate-key>\n";
        jmp wychod_point;
    }
    //checkpoint("First Errors\n");
    if (argc == 3) {
        for(int i = 0; argv[1][i] != 0; i++) {
            file += argv[1][i];
        }
        for(int i = 0; argv[2][i] != 0; i++) {
            regiment += argv[2][i];
        }
    } else {
        for(int i = 0; argv[1][i] != 0; i++) {
            file += argv[1][i];
        }
        for(int i = 0; argv[2][i] != 0; i++) {
            regiment += argv[2][i];
        }
        for(int i = 0; argv[3][i] != 0; i++) {
            regenerator_flag += argv[3][i];
        }
    }
    //checkpoint("args init\n");
    if ((regiment != "--code" && regiment != "--decode")) {
        rediska_flag |= 0b00000001;
    }
    //checkpoint("regiment flag\n");
    if (argc == 4 && regenerator_flag != "--regenerate-key") {
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
                cout << "\n2 of 2 flags are incorrect :: hope that file is not wrong...\n";
                jmp wychod_point;
                break;
        }
    } else {
    //    cout << "First 3 args initialized!\n";
    }
    //checkpoint("problems out\n");
    if (argc == 4 && regenerator_flag == "--regenerate-key") {
        RKT++;
    }
    FILE_PROCESSER(file, regiment);
    wychod_point:
    MEIN_SETCOLOR(WHITE);
    return 0;
}
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

struct WAVHEADER {
    // WAV-формат начинается с RIFF-заголовка:

    // Содержит символы "RIFF" в ASCII кодировке
    // (0x52494646 в big-endian представлении)
    char chunkId[4];

    // 36 + subchunk2Size, или более точно:
    // 4 + (8 + subchunk1Size) + (8 + subchunk2Size)
    // Это оставшийся размер цепочки, начиная с этой позиции.
    // Иначе говоря, это размер файла - 8, то есть,
    // исключены поля chunkId и chunkSize.
    unsigned long chunkSize;

    // Содержит символы "WAVE"
    // (0x57415645 в big-endian представлении)
    char format[4];

    // Формат "WAVE" состоит из двух подцепочек: "fmt " и "data":
    // Подцепочка "fmt " описывает формат звуковых данных:
    
    // Содержит символы "fmt "
    // (0x666d7420 в big-endian представлении)
    char subchunk1Id[4];

    // 16 для формата PCM.
    // Это оставшийся размер подцепочки, начиная с этой позиции.
    unsigned long subchunk1Size;

    // Аудио формат, полный список можно получить здесь http://audiocoding.ru/wav_formats.txt
    // Для PCM = 1 (то есть, Линейное квантование).
    // Значения, отличающиеся от 1, обозначают некоторый формат сжатия.
    unsigned short audioFormat;

    // Количество каналов. Моно = 1, Стерео = 2 и т.д.
    unsigned short numChannels;

    // Частота дискретизации. 8000 Гц, 44100 Гц и т.д.
    unsigned long sampleRate;

    // sampleRate * numChannels * bitsPerSample/8
    unsigned long byteRate;

    // numChannels * bitsPerSample/8
    // Количество байт для одного сэмпла, включая все каналы.
    unsigned short blockAlign;

    // Так называемая "глубиная" или точность звучания. 8 бит, 16 бит и т.д.
    unsigned short bitsPerSample;

    // Подцепочка "data" содержит аудио-данные и их размер.

    // Содержит символы "data"
    // (0x64617461 в big-endian представлении)
    char subchunk2Id[4];

    // numSamples * numChannels * bitsPerSample/8
    // Количество байт в области данных.
    unsigned long subchunk2Size;

    // Далее следуют непосредственно Wav данные.
};



unsigned char if_decode;

long long TO_PACK;

long long GETSIZE (string FILENAME) {
    ifstream hIn;
    hIn.open(FILENAME, ios::in | ios::binary);
    hIn.seekg(0, hIn.end);
    long long INFILELENGTH = hIn.tellg();
    hIn.seekg(0, hIn.beg);
    hIn.close();
    return INFILELENGTH;
}

int CHECK_HEADER(struct WAVHEADER in) {
    union {
        char chr[4];
        int intgr;
    } MASS[4];
    for(int i = 0x0; i < 0b00000100; i++) {
        MASS[0].chr[i] = in.chunkId[i];
        MASS[1].chr[i] = in.format[i];
        MASS[2].chr[i] = in.subchunk1Id[i];
        MASS[3].chr[i] = in.subchunk2Id[i];
    }
    if ((MASS[0].intgr & 0x52494646) && (MASS[1].intgr & 0x57415645) && (MASS[2].intgr & 0x666d7420) && (MASS[3].intgr & 0x64617461)) {
        return 1;
    } else {
        return -1;
    }
}

int GET_OPTIMAL_READ_CTR(struct WAVHEADER in, string TO_BEPACKED) {
    //Need subchunk2Size und bitsPerSample;
    long long MUST_BE_WRITTEN = GETSIZE(TO_BEPACKED);
    TO_PACK = MUST_BE_WRITTEN;
    MUST_BE_WRITTEN <<= 3; //needed LSB_PLACES;
    long long LENGTH = in.subchunk2Size;
    long long BYTES_ON_SAMPLE = in.bitsPerSample >> 3;
    int CTR = -1;
    if (LENGTH / BYTES_ON_SAMPLE < MUST_BE_WRITTEN) {
        return CTR;
    }
    //return (int)BYTES_ON_SAMPLE;
    char *ptr = (char*)&in;
    int res = (int)ptr[34] >> 3;
    return res;
}

int GET_UNPACK_CTR(struct WAVHEADER in) {
    long long BYTES_ON_SAMPLE = in.bitsPerSample >> 3;
    char *ptr = (char*)&in;
    int res = (int)ptr[34] >> 3;
    return res;
}

void ENCODE(string FileName, string KeyFile) {
    struct WAVHEADER WH;
    string FileOut = "ENC_" + FileName;
    ifstream hIn, hKey;
    hIn.open(FileName, ios :: in | ios::binary);
    hKey.open(KeyFile, ios :: in | ios::binary);
    char *CRUTCH = (char*)&WH;
    hIn.read(CRUTCH, 0x2C);
    int FREQER = GET_OPTIMAL_READ_CTR(WH, KeyFile);
    ofstream hOut;
    if (FREQER == -1) {
        cout << "WAV-FILE IS TOO SHORT TO ENLOAD SOME STUFF HERE!\n";
        jmp point_0;
    }
    if (!CHECK_HEADER(WH)) {
        cout << ".wav-file CORRUPTED!\n";
        jmp point_0;
    }
    hOut.open(FileOut, ios :: out | ios::trunc | ios::binary);
    hOut.write(CRUTCH, 0x2C);
    cout << "Freqer == " << FREQER << endl;
    while(!hIn.eof()) {
        char* fileee = new char[0x08 * FREQER];
        hIn.read(fileee, 0x08 * FREQER);
        if(!hKey.eof()){
            char key;
            hKey.read(&key, 1);
            for(int i = 0x00; i < 0b00001000; i++) {
                fileee[FREQER * i] = (fileee[FREQER * i] & 0xFE) + ((key & (0x01 << i)) >> i);
            }
        } else {
            char key = 0;
            for(int i = 0x00; i < 0b00001000; i++) {
                fileee[FREQER * i] = (fileee[FREQER * i] & 0xFE) + ((key & (0x01 << i)) >> i);
            }
        }
        hOut.write(fileee, 0x08 * FREQER);
    }   
    hOut.close();
    point_0:
    hIn.close();
    hKey.close();
    return;
}

void DECODE(string FileName, string KeyFile) {
    string FileOut = "DEC_" + KeyFile;
    ifstream hIn;
    struct WAVHEADER WH;
    hIn.open(FileName, ios :: in | ios::binary);
    char *CRUTCH = (char*)&WH;
    hIn.read(CRUTCH, 0x2C);
    int FREQER = GET_UNPACK_CTR(WH);
    ofstream hOut;
    hOut.open(FileOut, ios :: out | ios::trunc | ios::binary);
    cout << "Freqer == " << FREQER << endl;
    while(!hIn.eof()) {
        char* fileee = new char[0x08 * FREQER];
        hIn.read(fileee, 0x08 * FREQER);
        char key = 0x00;
        for(int i = 0x00; i < 0b00001000; i++) {
            key |= (fileee[FREQER * i] & ((0x01))) << i;
        }
        hOut.write(&key, 1);
    }   
    hOut.close();
    hIn.close();
    return;
}

void FILE_PROCESSER(string FNI, string SecFile, string RGMNT) {
    if (RGMNT == "--code") {
        checkpoint("--code process\n");
        ENCODE(FNI, SecFile);
    } else 
    if (RGMNT == "--decode") {
        checkpoint("--decode process\n");
        DECODE(FNI, SecFile);
    }
    return;
}

int main(int argc, const char* argv[]) {
    MEIN_SETCOLOR(MAGENTA);
    string file = "";
    string regiment = "";
    string info_file = "";
    unsigned char rediska_flag = 0b00000000; 
    if (argc != 4) {
        MEIN_SETCOLOR(RED);
        cout << "argc == " << argc << endl;
        cout << "\nUSAGE:: <.exe/.elf> <.wav> <file to enload via / to be enloaded> <--code or --decode>\n";
        jmp wychod_point;
    }
    //checkpoint("First Errors\n");
    if (argc == 4) {
        for(int i = 0; argv[1][i] != 0; i++) {
            file += argv[1][i];
        }
        for(int i = 0; argv[2][i] != 0; i++) {
            info_file += argv[2][i];
        }
        for(int i = 0; argv[3][i] != 0; i++) {
            regiment += argv[3][i];
        }
    }    
    //checkpoint("args init\n");
    if ((regiment != "--code" && regiment != "--decode")) {
        rediska_flag |= 0b00000001;
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
        }
    } 
    //checkpoint("problems out\n");
    if (regiment == "--decode") {
        if_decode = 1;
    } else {
        if_decode = 0;
    }
    FILE_PROCESSER(file, info_file, regiment);
    wychod_point:
    MEIN_SETCOLOR(WHITE);
    return 0;
}
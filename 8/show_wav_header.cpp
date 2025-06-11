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


int main(int argc, const char* argv[]) {
    MEIN_SETCOLOR(MAGENTA);
    string file = "";
    string regiment = "";
    string info_file = "";
    unsigned char rediska_flag = 0b00000000; 

    if (argc != 2) {
        MEIN_SETCOLOR(RED);
        cout << "argc == " << argc << endl;
        cout << "\nUSAGE:: <.exe/.elf> <.wav>\n";
        jmp wychod_point;
    }
{
    //checkpoint("First Errors\n");
    if (argc == 2) {
        for(int i = 0; argv[1][i] != 0; i++) {
            file += argv[1][i];
        }
    }
    struct WAVHEADER header;
    union {
        char ch[44];
        struct WAVHEADER wavv;
    } crutch;
    char *crutch_ptr = (char*)&header;
    ifstream hIn;
    hIn.open(file, ios::in | ios::binary);
    hIn.read(crutch_ptr, sizeof(header));
    hIn.close();
    ofstream hOut;
    hOut.open(file + ".log", ios::out | ios::trunc);
    for(int i = 0; i < 44; i++) {
        hOut << UC2S((unsigned char) crutch_ptr[i]) << endl;
    }
    hOut.close();
    //header = crutch.wavv;
    printf("Chunk ID :: %x%x%x%X\n", header.chunkId[0], header.chunkId[1], header.chunkId[2], header.chunkId[3]);
    printf("Chunksize :: %x\n", header.chunkSize);
    printf("Format :: %x%x%X%x\n", header.format[0], header.format[1], header.format[2], header.format[3]);
    printf("Subchunk1Id :: %x%x%x%x\n", header.subchunk1Id[0], header.subchunk1Id[1], header.subchunk1Id[2], header.subchunk1Id[3]);
    printf("Subchunk1Size :: %x\n", header.subchunk1Size);
    printf("AudioFormat :: %x\n", header.audioFormat);
    printf("NumChanels :: %x\n", header.numChannels);
    printf("SampleRate :: %x\n", header.sampleRate);
    printf("ByteRate :: %x\n", header.byteRate);
    printf("BlockAlugn :: %x\n", header.blockAlign);
    printf("BitsPerSample :: %x\n", header.bitsPerSample);
    printf("Subchunk2Id :: %x%x%x%x\n", header.subchunk2Id[0], header.subchunk2Id[1], header.subchunk2Id[2], header.subchunk2Id[3]);
    printf("Subchunk2Size :: %x\n", header.subchunk2Size);
    printf("\n\n\n\n");
    printf("Sample rate: %d\n", header.sampleRate);
    printf("Channels: %d\n", header.numChannels);
    printf("Bits per sample: %d\n", header.bitsPerSample);
    // Посчитаем длительность воспроизведения в секундах
    float fDurationSeconds = 1.f * header.subchunk2Size / (header.bitsPerSample / 8) / header.numChannels / header.sampleRate;
    int iDurationMinutes = (int)floor(fDurationSeconds) / 60;
    fDurationSeconds = fDurationSeconds - (iDurationMinutes * 60);
    printf("Duration: %02d:%02.f\n", iDurationMinutes, fDurationSeconds);
}
    //checkpoint("args init\n");
    wychod_point:
    MEIN_SETCOLOR(WHITE);
    return 0;
}
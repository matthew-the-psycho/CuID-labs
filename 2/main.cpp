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

vector<int> SYMBOL_COUNTER;
vector<float> FREQ_CONTAINER;
long int CHAR_COUNTER = 0;
float ENTROPY = 0.0;

void INIT_SYMBOL_COUNTER() {
    for(int i = 0x00; i <= 0xFF; i++) {
        SYMBOL_COUNTER.push_back(0);
    }
    return;
}

void FREQ_COUNTER(string filenamenn) {
    INIT_SYMBOL_COUNTER();
    ifstream FS;
    FS.open(filenamenn, ios::binary);
    unsigned char CRUTCH;
    char C2C;
    while(!FS.eof()){
        FS.read(&C2C, 1);
        CRUTCH = (unsigned char)C2C;
        SYMBOL_COUNTER[CRUTCH]++;
    }
    FS.close();
    for(int i = 0x00; i <= 0xFF; i++) {
        CHAR_COUNTER += SYMBOL_COUNTER[i];
    }
    for(int i = 0x00; i <= 0xFF; i++) {
        FREQ_CONTAINER.push_back((float)SYMBOL_COUNTER[i]/CHAR_COUNTER);
    }
    return;
}

void ENTROPY_VIA_FREQS_COUNTER() {
    for(int i = 0; i <= 0xFF; i++) {
        if (FREQ_CONTAINER[i] != 0)
            ENTROPY -= FREQ_CONTAINER[i]*(log(FREQ_CONTAINER[i])/log(2));
    }
    return;
} 

int main(int argc, const char* argv[]) {
    MEIN_SETCOLOR(MAGENTA);
    string file = "";
    if (argc != 2) {
        MEIN_SETCOLOR(RED);
        cout << "argc == " << argc << endl;
        cout << "\nUSAGE: <.exe/.elf> <file to process>\n";
        jmp wychod_point;
    }
    for(int i = 0; argv[1][i] != 0; i++) {
        file += argv[1][i];
    }
    FREQ_COUNTER(file);
    ENTROPY_VIA_FREQS_COUNTER();
    results_point:
    cout << "File has ";
    MEIN_SETCOLOR(GREEN);
    cout << CHAR_COUNTER;
    MEIN_SETCOLOR(MAGENTA);
    cout << " characters;\n";
    cout << "FILE ENTROPY :: ";
    MEIN_SETCOLOR(GREEN);
    cout << ENTROPY << endl;
    MEIN_SETCOLOR(MAGENTA);
    cout << " CHAR ";
    MEIN_SETCOLOR(WHITE);
    cout << "|";
    MEIN_SETCOLOR(MAGENTA);
    cout << " COUNTEDD ";
    MEIN_SETCOLOR(WHITE);
    cout << "|";
    MEIN_SETCOLOR(MAGENTA);
    cout << " FREQNCY";
    MEIN_SETCOLOR(WHITE);
    cout << "\n--------------------------\n";
    for(int i = 0x00; i <= 0xFF; i++) {
        MEIN_SETCOLOR(MAGENTA);
        //printf(" 0x%2x ", i);
        cout << " 0x" << setw(2) << right << hex << setfill('0') << i << setfill(' ') << dec;
        MEIN_SETCOLOR(WHITE);
        cout << " |";
        MEIN_SETCOLOR(MAGENTA);
        //cout << " " << setw(8) << right << SYMBOL_COUNTER[i] << " ";
        printf(" %8d ", SYMBOL_COUNTER[i]);
        MEIN_SETCOLOR(WHITE);
        cout << "|";
        MEIN_SETCOLOR(MAGENTA);
        //cout << " " << setw(7) << right << setprecision(3) << FREQ_CONTAINER[i];
        cout << " " << setprecision(3) << setw(7) << right << FREQ_CONTAINER[i];
        //printf(" %3.3f", FREQ_CONTAINER[i]);
        MEIN_SETCOLOR(WHITE);
        cout << endl;
    }
    wychod_point:
    MEIN_SETCOLOR(WHITE);
    return 0;
}
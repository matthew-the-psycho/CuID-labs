#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>

using namespace std;

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

#define jmp goto

int main () {
    vector<int> Vec;
    Vec.push_back(0);
    for(int i = 0; i < 15; i++) {
        Vec.push_back(Vec[Vec.size() - 1] + 0x01);
    }
    for(int i = 0; i < 0x10; i++) {
        MEIN_SETCOLOR(i % 4);
        cout << Vec[i] << " ";
    }
    MEIN_SETCOLOR(WHITE);
    cout << endl;
    return 0;
}
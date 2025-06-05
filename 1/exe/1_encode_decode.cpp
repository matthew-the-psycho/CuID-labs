#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>

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

string aa = "abcdefghijklmnopqrstuvwxyz";
string AA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

string bb = "";
string BB = "";

void string_b_init(int key) {
    bb = aa;
    BB = AA;
    for (int i = 0; i < 26; i++) {
        int pos = (i + 26 + key) % 26;
        bb[i] = aa[pos];
        BB[i] = AA[pos];
    }
    return;
}

string GET_FNO(string FNI) {
    string FNO = "";
    for(int i = 0; FNI[i] != '.'; i++) {
        FNO += FNI[i];
    }
    FNO += ".out";
    return FNO;
}

void checkpoint(const char* line) {
    MEIN_SETCOLOR(GREEN);
    cout << line;
    MEIN_SETCOLOR(MAGENTA);
    return;
}

void ENCODE(string filename, int key) {
    string_b_init(key);
    string filenameout = GET_FNO(filename);
    ifstream in;
    ofstream out;
    in.open(filename, ios::in);
    out.open(filenameout, ios::trunc);
    while(!in.eof()) {
        string buf = "";
        string bufout = "";
        getline(in, buf);
        for(int i = 0; i < buf.length(); i++) {
            if (aa.find(buf[i]) != -1) {
                bufout += bb[aa.find(buf[i])];
            } 
            else if (AA.find(buf[i]) != -1) {
                bufout += BB[AA.find(buf[i])];
            }
            else {
                bufout += buf[i];
            }
        }
        out << bufout << endl;
    }
    in.close();
    out.close();
    return;
}

void DECODE(string filename, int key) {
    string_b_init(-key);
    string filenameout = GET_FNO(filename);
    ifstream in;
    ofstream out;
    in.open(filename, ios::in);
    out.open(filenameout, ios::trunc);
    while(!in.eof()) {
        string buf = "";
        string bufout = "";
        getline(in, buf);
        for(int i = 0; i < buf.length(); i++) {
            if (aa.find(buf[i]) != -1) {
                bufout += bb[aa.find(buf[i])];
            } 
            else if (AA.find(buf[i]) != -1) {
                bufout += BB[AA.find(buf[i])];
            }
            else {
                bufout += buf[i];
            }
        }
        out << bufout << endl;
    }
    in.close();
    out.close();
    return;
}

int main(int argc, const char* argv[]) {
    MEIN_SETCOLOR(MAGENTA);
    if (argc != 3) {
        MEIN_SETCOLOR(RED);
        cout << "argc == " << argc << endl;
        cout << "\nUSAGE: <.exe/.elf> <file to decode/encode> <key::0:25>\n";
        jmp wychod_point;
    }
{
    //checkpoint("\nargc good\n");
    string file_namenn = "";
    string key_s = "";
    for (int i = 0; argv[1][i] != 0; i++) {
        file_namenn += argv[1][i];
    }
    for (int i = 0; argv[2][i] != 0; i++) {
        key_s += argv[2][i];
    }
    //checkpoint("\nfilename good\n");
    int key = stoi(key_s);
    key = key % 26;
    //checkpoint("\nKey gott\n");
    action_point: 
    cout << "\nChoose the action to arbeit:\n";
    cout << "1) to encode;\n";
    cout << "2) to decode;\n";
    int action;
    cout << "\nChosen:\t";
    cin >> action;
    switch(action) {
        case 1:
            ENCODE(file_namenn, key);
            break;
        case 2:
            DECODE(file_namenn, key);
            break;
        default:
            cout << "\nERROR HAPPENED!\n";
            jmp action_point;
    }
}
    wychod_point:
    MEIN_SETCOLOR(WHITE);
    return 0;
}
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

void checkpoint(const char* line) {
    MEIN_SETCOLOR(GREEN);
    cout << line;
    MEIN_SETCOLOR(MAGENTA);
    return;
}

string CHECK_ENCODE(string decoded_line) {
    string bufout = "";
    for(int i = 0; i < decoded_line.length(); i++) {
        if (aa.find(decoded_line[i]) != -1) {
            bufout += bb[aa.find(decoded_line[i])];
        } 
        else if (AA.find(decoded_line[i]) != -1) {
            bufout += BB[AA.find(decoded_line[i])];
        }
        else {
            bufout += decoded_line[i];
        }
    }
    return bufout;
}

int KEY_FIND(string dec, string enc) {
    int i = 0;
    string_b_init(i);
    string encoded_str = CHECK_ENCODE(dec);
    //checkpoint("\nKEY_FIND_While\n"); 
    while(enc != encoded_str) {
        i++;
        string_b_init(i);
        encoded_str = CHECK_ENCODE(dec);
    }
    //checkpoint("\nKEY_FIND_While_Gone\n");
    return i;
}

float FILE_PROCESSER(string FND, string FNE) {
    ifstream fnd, fne;
    fnd.open(FND, ios::in);
    fne.open(FNE, ios::in);
    vector<int> Vec;
    //checkpoint("\nWhile\n");
    while (!(fne.eof() || fnd.eof())) {
        string decoded, encoded;
        getline(fnd, decoded);
        getline(fne, encoded);
        int flag = -1;
        for(int i = 0; i < 26; i++) {
            if (decoded.find(aa[i]) != -1)
                flag = 1;
            if (decoded.find(AA[i]) != -1)
                flag = 1;
        }
        //cout << RED << "Flag is " << flag << MAGENTA << endl;
        int CRUTCH;
        if (flag != -1) {
            CRUTCH = KEY_FIND(decoded, encoded);
            //checkpoint("KF\n");
            Vec.push_back(CRUTCH);
            //checkpoint("V_PB_KF\n");
        } else {
            int CRUTCH2CRUTCH = Vec.size() - 1;
            //checkpoint("C2C_GOTT\n");
            CRUTCH = Vec[CRUTCH2CRUTCH];
            //checkpoint("C_GOTT\n");
            Vec.push_back(CRUTCH);
            //checkpoint("V_PB_LE\n");
        }
    }
    //checkpoint("\nWhile gone\n");
    float avg = 0;
    for(int i = 0; i < Vec.size(); i++) {
        avg += (float)Vec[i] / Vec.size();
    }
    fnd.close();
    fne.close();
    MEIN_SETCOLOR(GREEN);
    cout << "CODE DISTANCE";
    MEIN_SETCOLOR(MAGENTA); 
    cout << " VECTOR SET ::\n";
    for(int i = 0; i < Vec.size(); i++) {
        MEIN_SETCOLOR(MAGENTA);
        cout << "Line\t";
        MEIN_SETCOLOR(RED); 
        cout << i + 1; 
        MEIN_SETCOLOR(MAGENTA);
        cout << ":" << '\t' << Vec[i] << endl;
    }
    MEIN_SETCOLOR(MAGENTA);
    cout << endl;
    return avg;
}

int main(int argc, const char* argv[]) {
    MEIN_SETCOLOR(MAGENTA);
    if (argc != 3) {
        MEIN_SETCOLOR(RED);
        cout << "argc == " << argc << endl;
        cout << "\nUSAGE: <.exe/.elf> <file of decode> <file of encode>\n";
        jmp wychod_point;
    }
    //checkpoint("\nargc\n");
{
    string file_namenn = "";
    string file_outten = "";
    for (int i = 0; argv[1][i] != 0; i++) {
        file_namenn += argv[1][i];
    }
    for (int i = 0; argv[2][i] != 0; i++) {
        file_outten += argv[2][i];
    }
    //checkpoint("\nfilenames\n");
    float average_key = FILE_PROCESSER(file_namenn, file_outten);
    MEIN_SETCOLOR(MAGENTA);
    cout << "\nAverage key ";
    MEIN_SETCOLOR(GREEN);
    cout << "DISTANCE ";
    MEIN_SETCOLOR(MAGENTA);
    cout << "is ";
    MEIN_SETCOLOR(GREEN);
    printf("%2.3f\n", average_key);
    MEIN_SETCOLOR(MAGENTA);
}
    wychod_point:
    MEIN_SETCOLOR(WHITE);
    return 0;
}
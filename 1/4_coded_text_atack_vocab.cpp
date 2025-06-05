#include <iostream>
#include <fstream>
#include <iomanip>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

#define jmp goto
#define FNV "vocab.vcb"

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

string aa = "abcdefghijklmnopqrstuvwxyz";
string AA = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

string bb = "";
string BB = "";

vector<string> VCB;

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

void VOCAB_INIT() {
    ifstream VOCB;
    VOCB.open(FNV, ios::in);
    while(!VOCB.eof()) {
        string buf = "";
        getline(VOCB, buf);
        VCB.push_back(buf);
    }
    VOCB.close();
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

int VCB_CHECK(string DCD_cont, string VCB_w) {
    int hardness = VCB_w.length();
    int variants = pow(2, hardness);
    string* A = new string[variants];
    int position = -1;
    for(int i = 0; i < VCB_w.length(); i++) {
        for(int j = 0; j < variants; j++) {
            if (j / (int)pow(2, i) % 2) {
                A[j] += VCB_w[i];
            } else {
                A[j] += (char)toupper(VCB_w[i]);
            }
        }
    }
    for(int i = 0; i < variants; i++) {
        //cout << "\033[38;5;155m" << A[i] << " " << WHITE;
        if (DCD_cont.find(A[i])) {
            position = DCD_cont.find(A[i]);
        }
    }
    //cout << endl;
    delete[] A;
    return position;
}

void FILE_PROCESSER(string FNE, int voc_flag) {
    setlocale(LC_ALL, "ru_RU");
    ifstream fne;
    ofstream fnd;
    string FND = FNE + ".dcd.log";
    fnd.open(FND, ios::trunc);
    fne.open(FNE, ios::in);
    //checkpoint("\nWhile\n");
    string ENC_container = "";
    while (!fne.eof()) {
        string encoded;
        getline(fne, encoded);
        ENC_container += encoded;
        ENC_container += '\n';
    }
    if (voc_flag) {
        fnd << "VOCABULAR MODE :: ENABLED!\n\n";
        int flag_voc_success = 0;
        for(int i = -25; i < 0; i++) {
            string DCD_container;
            string_b_init(i);
            DCD_container = CHECK_ENCODE(ENC_container);
            int check_container = 0;
            vector<int> GeWorkst;
            for(int j = 0; j < VCB.size(); j++) {
                if (VCB_CHECK(DCD_container, VCB[j]) != -1) {
                    GeWorkst.push_back(1);
                } else {
                    GeWorkst.push_back(0);
                }
                check_container += GeWorkst[j];
            }
            if (check_container > 0) {
                fnd << "Found " << check_container << " words:\n";
                for(int j = 0; j < GeWorkst.size(); j++) {
                    if (GeWorkst[j] == 1) {
                        fnd << VCB[j] << ", ";
                    }
                }
                fnd << "\nKEY == " << i << " ::\n";
                fnd << DCD_container << endl;
                flag_voc_success++;
            }
        }
        if (!flag_voc_success) {
            fnd << "vocab.vcb doesn't have enough words to true'ly decode ::\n";
            fnd << "    SWITCHING IN BROOT_FORCE_METHOD!\n";
            jmp point_voc_disable;
        }
    } else {
        point_voc_disable:
        fnd << "VOCABULAR MODE :: DISABLED\n\n";
        for(int i = -25; i < 0; i++) {
            fnd << "Key == " << i << " :: \n";
            string_b_init(i);
            fnd << CHECK_ENCODE(ENC_container) << endl << endl;
        }
    }
    //checkpoint("\nWhile gone\n");
    fnd.close();
    fne.close();
}

int main(int argc, const char* argv[]) {
    MEIN_SETCOLOR(MAGENTA);
    if (argc < 2 && argc > 3) {
        MEIN_SETCOLOR(RED);
        cout << "argc == " << argc << endl;
        cout << "\nUSAGE: <.exe/.elf> <file of encode> <parameter --voc>\n";
        jmp wychod_point;
    }
    //checkpoint("\nargc\n");
{
    string file_namenn = "";
    for (int i = 0; argv[1][i] != 0; i++) {
        file_namenn += argv[1][i];
    }
    if (argc == 3) {
        string file_voc = "";
        for (int i = 0; argv[2][i] != 0; i++) {
            file_voc += argv[2][i];
        }
        if (file_voc == "--voc") {
            VOCAB_INIT();
            FILE_PROCESSER(file_namenn, 1);
        } else {
            MEIN_SETCOLOR(RED);
            cout << "ERROR! :: parameter usage!\n";
            jmp wychod_point;
        }
    } else if (argc == 2) {
        FILE_PROCESSER(file_namenn, 0);
    }
    //checkpoint("\nfilenames\n");
}
    wychod_point:
    MEIN_SETCOLOR(WHITE);
    return 0;
}
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

void SECRETKEY_ENLOAD() {
    BASE_NUM = 3;
    MOD_BASE = 0x11;
    return;
}

unsigned int ENCRYPTOR(unsigned int OwnCharacter, unsigned int BaseCharacter) {
    unsigned int PubCharacter;
    unsigned int eax = 1;
    unsigned int base_eax = BaseCharacter;
    unsigned int flag_used;
    //printf("\nStarted by :: \nBase :: %d; Pow :: %d; mod :: %d\n", BaseCharacter, OwnCharacter, MOD_BASE);
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
        
        //printf("    pow :: %d; base_eax :: %d; eax :: %d; used :: %d\n", i, base_eax, eax, flag_used);
    }
    PubCharacter = eax;
    //printf("Base :: %d; Pow :: %d; Res :: %d\n", BaseCharacter, OwnCharacter, PubCharacter);
    return PubCharacter;
}


void INIT_MOD_BASE(int MB_num) {
    MOD_BASE = (unsigned int) MB_num;
    return;
}

float DOOM_REV_SQRT(float x) {
    float xhalf = 0.5 * x;
    int i = *(int*)&x;
    i = 0x5F3759DF - (i >> 1);
    float y = *(float*)&i;
    y = y * (1.5 - (xhalf * y * y));
    return y;
}

float DOOM_SQRT_CRUTCH(float x) {
    return 1.0 / DOOM_REV_SQRT(x);
}

int DOOM_SQRT(int x) {
    return (int)DOOM_SQRT_CRUTCH((float)x);
}

void MAIN_PROCESSER(int Y, int A, int MOD) {
    INIT_MOD_BASE(MOD);
    if (RKT == 0) {
        unsigned int i = 1;
        while (ENCRYPTOR(i, (unsigned int)A) != (unsigned int)Y) {
            i++;
            printf("NEXT_Check power %d;\n", i);
        }
        printf("Straight found method\nX = %d\n", i);
    } else {
        int cz1, cz2;
        if (((DOOM_SQRT((int)MOD_BASE) + 1) * DOOM_SQRT((int)MOD_BASE)) <= MOD_BASE) {
            if (((DOOM_SQRT((int)MOD_BASE) + 1) * (DOOM_SQRT((int)MOD_BASE + 2))) <= MOD_BASE) {
                cz1 = DOOM_SQRT((int)MOD_BASE) + 3;
                cz2 = DOOM_SQRT((int)MOD_BASE) + 2;
                checkpoint("+3 +2 methodik\n");
            } else {
                cz1 = DOOM_SQRT((int)MOD_BASE) + 2;
                cz2 = DOOM_SQRT((int)MOD_BASE) + 1;
                checkpoint("+2 +1 methodik\n");
            }
        } else {
            cz1 = DOOM_SQRT((int)MOD_BASE) + 1;
            cz2 = DOOM_SQRT((int)MOD_BASE);
            checkpoint("+1 +0 methodik\n");
        }
        int m = cz1, k = cz2;
        int i, j;
        for (j = 0; j < m; j++) {
            for(i = 1; i <= k; i++) {
                unsigned int Arg[2] = {ENCRYPTOR(1, ENCRYPTOR((unsigned int)j, (unsigned int)A)*ENCRYPTOR(1, (unsigned int)Y)), ENCRYPTOR((unsigned int)i * m, (unsigned int)A)};
                printf("Step :: %d, i == %d, j == %d; Arg[0] == %d, Arg[1] == %d;\n", j * k + i, i, j, Arg[0], Arg[1]);
                if (Arg[0] == Arg[1]) {
                    jmp found_pnt;
                }
            }
        }
        found_pnt:
        int x = i * m - j;
        printf("MOD_BASE == %d :: SQRT(Low) == %d\n", MOD_BASE, DOOM_SQRT(MOD_BASE));
        printf("Method Giant Method Toddler (Shanks Method) ::\n");
        printf("IT WAS :: m == %d, k == %d\n", m, k);
        printf("X = i * m - j = %d * %d - %d = %d\n", i, m, j, x);
    }
    return;
}

int STR2INT(string input) {
    int res = 0;
    string VOC = "0123456789";
    for(int i = 0x0; i < input.length(); i++) {
        if (VOC.find(input[i]) != -1) {
            res *= 0xA;
            res += VOC.find(input[i]);
        } else {
            return -1;
        }
    }
    return res;
}

int main(int argc, const char* argv[]) {
    MEIN_SETCOLOR(MAGENTA);
    string y_base = "";
    string a_base = "";
    string mod_base = "";
    string mgmt_flag = "";
    unsigned char rediska_flag = 0b00000000; 
    TRACER_MUL = 0;
    if (argc < 4 || argc > 5) {
        MEIN_SETCOLOR(RED);
        cout << "argc == " << argc << endl;
        cout << "\nUSAGE:: <.exe/.elf> <y_base> <a_base> <MOD_base> <--mgmt-key / for Shanks method>\n";
        jmp wychod_point;
    }
    //checkpoint("First Errors\n");
    if (argc == 4) {
        for(int i = 0; argv[1][i] != 0; i++) {
            y_base += argv[1][i];
        }
        for(int i = 0; argv[2][i] != 0; i++) {
            a_base += argv[2][i];
        }
        for(int i = 0; argv[3][i] != 0; i++) {
            mod_base += argv[3][i];
        }
    } else {
        for(int i = 0; argv[1][i] != 0; i++) {
            y_base += argv[1][i];
        }
        for(int i = 0; argv[2][i] != 0; i++) {
            a_base += argv[2][i];
        }
        for(int i = 0; argv[3][i] != 0; i++) {
            mod_base += argv[3][i];
        }
        for(int i = 0; argv[4][i] != 0; i++) {
            mgmt_flag += argv[4][i];
        }
    }
    //checkpoint("args init\n");
    int y, a, MB;
    if (STR2INT(a_base) == -1) {
        rediska_flag |= 0b00000001;
    } else {
        a = STR2INT(a_base);
    }
    if (STR2INT(mod_base) == -1) {
        rediska_flag |= 0b00000100;
    } else {
        MB = STR2INT(mod_base);
    }
    if (STR2INT(y_base) == -1) {
        rediska_flag |= 0b00001000;
    } else {
        y = STR2INT(y_base);
    }
    //checkpoint("regiment flag\n");
    if (argc == 5 && mgmt_flag != "--mgmt-key") {
        rediska_flag |= 0b00000010;
    }
    //checkpoint("regen flag\n");
    //cout << "redis_flag is " << (int)rediska_flag << endl;
    if (rediska_flag > 0b00000000) {
        MEIN_SETCOLOR(RED);
        cout << "Caused Problemenn\n";
        if (rediska_flag & 0b00001101) {
            cout << "argv[0::2] must be only decimal digits!\n";
        }
        if (rediska_flag & 0b00000010) {
            cout << "argv[3] can be only as --mgmt-key\n";
        }
    } else {
    //    cout << "First 3 args initialized!\n";
    }
    //checkpoint("problems out\n");
    if (argc == 5 && mgmt_flag == "--mgmt-key") {
        RKT++;
    }
    MAIN_PROCESSER(y, a, MB);
    cout << endl << "Resulted MULTIPLY counted is " << TRACER_MUL << endl;
    wychod_point:
    MEIN_SETCOLOR(WHITE);
    return 0;
}
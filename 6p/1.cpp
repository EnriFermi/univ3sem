#include <iostream>


using namespace std;
enum state
{
    E,
    AC,
    BD,
    SFB,
    ASF,
    SF,
    S
};
class Scanner
{
    char c_curr;
    state CS;
    FILE *fp; // processing file

    void gc()
    {
        c_curr = fgetc(fp);
    }

public:
    bool check_correct();
    Scanner(const char *program)
    {
        fp = fopen(program, "r");
        CS = E;
        gc();
    }
};

bool Scanner::check_correct(){
    bool correct = true;
    while (c_curr != '\n' && correct){
        switch (CS)
        {
        case E: // Initial state
            if (c_curr == 'a') {
                CS = AC;
            } else if(c_curr == 'b'){
                CS = BD;
            } else{
                correct = false;
            }
            break;
        case AC:
            if (c_curr == 'a') {
                CS = SFB;
            } else{
                correct = false;
            }
            break;
        case BD:
            if (c_curr == 'b') {
                CS = ASF;
            } else{
                correct = false;
            }
            break;
        case SFB:
            if (c_curr == 'b') {
                CS = SF;
            }else if(c_curr == 'c'){
                CS = S;
            } else{
                correct = false;
            }
            break;
        case ASF:
            if (c_curr == 'a') {
                CS = SF;
            }else if(c_curr == 'c'){
                CS = S;
            } else{
                correct = false;
            }
            break;
        case SF:
            if (c_curr == 'c') {
                CS = S;
            } else{
                correct = false;
            }
            break;
        case S:
            correct = false;
            break;
        }
        // cout << c_curr << endl;
        gc();
    }
    return correct;
}
int main(){

    int x;
    1 || 2 + 3 || 4;
    
}

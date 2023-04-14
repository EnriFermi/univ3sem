#include <iostream>
#include <cstring>
#include <ctime>
using namespace std;
struct p_1
{
    int i;
    virtual void print()=0;
    p_1(int i): i(i){};
};
struct s_1: public p_1 
{
    s_1(int i): p_1(i){};
    void print(){
        cout << "S_1 " << this->i << endl;
    }
};
struct s_2: public p_1
{
    s_2(int i): p_1(i){};
    void print(){
        cout << "S_2 " << this->i << endl;
    }
};
class test
{
        p_1 ** mas;
        int size;
    public:
        void add(p_1 *p){
            if(size < 10){
                mas[size] = p;
                size++;
            }
        }
        void print(){
            for(int j = 0; j<size; j++){
                mas[j]->print();
            }
        }
        test():size(0){mas = new p_1*[10];};
        ~test(){delete[] mas;};
};



int main()
{
    test t = test();
    s_1 s1 = s_1(2);
    s_2 s2 = s_2(3);
    p_1 *p1 = &s1;
    p_1 *p2 = &s2;
    t.add(p1);
    t.add(p2);
    t.print();
}
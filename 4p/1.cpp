#include <iostream>
#include <cstring>
#include <ctime>
#include <new>
using namespace std;
class A{
    private:
        int a;
        ~A(){};
        A(int i=0): a(i){}
        A(A& r){
            this->a = r.a;
        }
    public:
        static A* created;
        void print(){
            cout << a << endl;
        }
        static A* f(int i){
            if(created == nullptr){
                created = new A(i);
            }
            return created;
        }
        static void d(A* d){
            delete d;
        }
};
A* A::created = nullptr;

class B{
    int b;
    ~B(){}
    B(int i=0): b(i){}
    public:
        static B* f(int i){
            return new B(i);
        }
        static void d(B* d){
            delete d;
        }
        void print(){
            cout << b << endl;
        }
};
class I{
    public:
    int i;
    I(){
        cout << "cr" << endl;
    }
    ~I(){
        cout << "dl" << endl;
    }
};


class D{
    public: int f1(){
        int i;
        int j;
        int& operator()( int a, int b );

        return (i,j) [i,j] (i,j);
    }
    
};
int& D::operator()(int row, int col)
{
    return ;
}
 
int main()
{
    // int N = 10;
    // //int a[N];
    // I *a = (I*) ::operator new(N*sizeof(I));
    // I* mas = new (a) I[N];
    // for(int i=0; i<N; i++){
    //     mas[i].~I();
    // }
    // ::operator delete(mas);
    

}



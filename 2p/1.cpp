#include <iostream>
#include <cstring>
#include <string>
using namespace std;

class Complex
{
private:
    int re;
    int im;

public:
    static int counter;
    Complex(int re, int im): re(re), im(im){counter++;};

    ~Complex()
    {
        cout << "~Complex()" << endl;
        counter--;
    }

    Complex &operator=(const Complex &s)
    {
        if (this != &s)
        {
            this->re = s.re;
            this->im = s.im;
        }
        return *this;
    }
    const Complex operator+(const Complex& right){
        return Complex(right.re + this->re, right.im + this->im);
    }
    const Complex operator-(const Complex& right){
        return Complex(re-right.re, im - right.im);
    }
    const Complex operator*(const Complex& right){
        return Complex(right.re*re - right.im*im, right.im*re + right.re*im);
    }
    friend bool operator==(const Complex& right, const Complex& left){
        return (right.im == left.im)&&(right.re == left.re);
    }
    int get_re()
    {
        return this->re;
    }
    int get_im()
    {
        return this->im;
    }
};
int Complex::counter = 0;


class Animal{  
    protected:
        int time;
        string name;
        int limit;
        int type;
    public:
        void tick(){
            time++;
            if(is_hungry()){
                cout <<  "I m " << name << " , feed me!!!" << endl;
            }
        }
        bool is_hungry(){
            return time >= limit;
        }
        void feed(int food_type){
            if(type == food_type){
                time = 0;
            } else {
                cout <<  "I m " << name << " , wrong food!!!" << endl;
            }
            
        }
};

class Wolf:public Animal{
    public:
        Wolf(){
            type = 1;
            time = 0;
            name = "Wolf";
            limit = 2;
        };
};
class Lion:public Animal{
    public:
        Lion(){
            type = 1;
            time = 0;
            name = "Lion";
            limit = 2;
        };
};
class Tiger:public Animal{

    public:
        Tiger(){
            type = 1;
            time = 0;
            name = "Tiger";
            limit = 3;
        };
};
class Giraffe:public Animal{

    public:
        Giraffe(){
            type = 0;
            time = 0;
            name = "Giraffe";
            limit = 4;
        };
};
class Deer:public Animal{

    public:
        Deer(){
            type = 0;
            time = 0;
            name = "Deer";
            limit = 3;
        };
};
int main()
{
    Wolf w = Wolf();
    w.tick();
    w.tick();
    if(w.is_hungry()){
        w.feed(0);
    }
}
#include <iostream>
#include <cmath>
using namespace std;

class Arr
{
    int *arr;
    int size;

public:
    Arr(int s, int date = 0) : size(s)
    {
        cout << "Create" << endl;
        arr = new int[s];
        for (int i = 0; i < size; arr[i++] = date);
    }
    Arr &operator=(const Arr &s)
    {
        cout << "Operator =" << endl;
        if (this != &s)
        {
            this->size = s.size;
            if(this->arr != NULL){
                delete[] this->arr;
            }
            this->arr = new int[size];
            for (int i = 0; i < size; this->arr[i++] = s.arr[i]);
        }
        return *this;
    }
    Arr& operator=(Arr&& s){
        cout << "Move operator" << endl;
        if (this != &s)
        {
            if(this->arr != NULL){
                delete[] this->arr;
            }
            this->size = s.size;
            this->arr = s.arr;
            s.size = 0;
            s.arr = NULL;
            
        }
        return *this;
    }
    int get_size() const{
        return size;
    }
    friend Arr operator|(const Arr& left, const Arr& right){
        cout << "Operator |" << endl;
        return Arr(min(left.size, right.size), left.arr[0]|right.arr[0]);
    }
    int & operator[](int i){
        cout << "Operator []" << endl;
        return this->arr[i];
    }
    friend Arr& operator|=(Arr& left,const Arr& right){
        cout << "Operator |=" << endl;
        for(int i=0; i<left.size; left.arr[i++] = left.arr[i]|right.arr[i]);
        return left;
    }
    friend const Arr operator-(const Arr& left){
        cout << "Operator -" << endl;
        return Arr(left.size, (-1)*left.arr[0]);
    }
    Arr(Arr&& right): size(right.size), arr(right.arr){
        cout << "Move constructor" << endl;
    }

    ~Arr() {
        cout << "Delete" << endl;
        if(arr != NULL){
            delete[] arr; 
        }
    }
};
ostream& operator<<(ostream &out, const Arr& right)
{

    return out << right.get_size();
}

int main()
{
    Arr a1(15), a2(20, 5), a3(30, 3);
    const Arr a4(5);
    cout << a1 << a2 << a3 << a4;
    a2 = a2 | 2; // объект a2 здесь не меняется
    a2 |= 1;     // а здесь меняется
    a3 = (-a2) | (-a4);
    a3[5] = 10;
    a1 = a4 | a3;
    cout << a1 << a2 << a3 << a4;
    return 0;
}
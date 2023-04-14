#include <iostream>
using namespace std;
class Arr {
    int * arr;
    int size;
public:
    void decrease(int a, int i){
        arr[i] |= a;
    }
    void neg(){
        for (int i = 0; i < this->size; i++)
            arr[i] = -arr[i];
    }
    int get_size() const{
        return size;
    }
    int get_arri(int i) const{
        return this->arr[i];
    }
    Arr(int s, int date=0) : size(s){
        arr= new int[s];
        for(int i=0;i<size;arr[i++]=date);
    }
    ~Arr () { delete [] arr;}
    const Arr operator|(const int i){
        this->arr[i] = 0;
        return *this;
    }
    friend ostream &operator<<(ostream &os, const Arr& a){
        int s = a.get_size();
        for (int i = 0; i < s; i++) os << a.get_arri(i) <<" ";
        os << '\n';
        return os << "size is:"<< s << '\n';
    }
    const Arr operator|=(const int i){
        this->arr[i] = 0;
        return *this;
    };
    const Arr &operator-(){
        this->neg();
        return *this;
    }
    Arr &operator=(const Arr &a){
        if (this != &a)
        {
            this->size = a.size;
            if(this->arr != NULL)
                delete[] this->arr;
            this->arr = new int[size];
            for (int i = 0; i < size; this->arr[i++] = a.arr[i]);
        }
        return *this;
    }
    friend const Arr operator|(const Arr& left, const Arr& right){
            return Arr(min(left.size, right.size), left.arr[0]|right.arr[0]);
    }

};

int  main()  {
        Arr  a1(15),  a2(20,5),  a3(30,3);    const Arr a4(5);
        cout << a1 <<a2<<a3<<a4;
        a2 = a2 | 2;  // объект a2 здесь не меняется
//        a2 |=1;  // а здесь меняется
//        a3= (- a2) | (-a4);
//        a3[5]=10;
//        a1= a4 | a3;
        cout<<a1 <<a2<<a3<<a4;
 return 0;
}
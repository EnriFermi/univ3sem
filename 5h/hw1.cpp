// ДЗ-4. Шаблоны. Срок сдачи — до 01.04.23

// Написать класс-функцию (с перегруженным operator()), для выполнения некоторых действий над передаваемым ей контейнером. Действия можете придумать любые (например: поменять местами элементы, реверсировать и т. п.), напишите комментарий, что делаете.

// Функция должна быть применима к list, vector, string, хорошо, если и к массивам встроенных типов.

// Количество параметров в функции - по необходимости.
#include <iostream>
#include <list>
#include <vector>
#include <type_traits>

using namespace std;

class Func{
    public:
    Func(){}

    template <typename L, typename E>
    void operator()(L &l, int index, E elem){ // Присваивание i ого элемента
        int counter = 0;

        for (auto iter = l.begin(); iter != l.end(); ++iter)
        {
            if (counter == index)
            {
                *iter = elem;
                break;
            }
            counter++;
        }
    }
    template <typename E>
    void operator()(E* l, int index, E elem){ // Присваивание i ого элемента
        int counter = 0;
        l[index] = elem;
    }
};
template<typename L1>
void print(const L1 & l)
{
    for (auto i : l) {
        cout << i;
    }
    cout << endl;
}


int main(){

    // Func f; // объект-функция
    
    // list<int> L={1,2,3,4,5};

    // f(L, 1, 9); // как-то меняет список

    // print(L);

    // vector<int> V={1,2,3,4,5};

    // f(V, 1, 9);

    // print(V);    
    // string s1="abcdef";

    // f(s1, 1, '9');

    // print(s1);

    // int M[5]={1,2,3,4,5};
    // f(M, 1, 9);
    
    // print(M);
    int p = 10;
    int * d = new int(p);
    cout << d << &p << endl;
    cout << (new int(p))[0]<< endl;
}

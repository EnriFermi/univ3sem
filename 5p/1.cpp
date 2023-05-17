#include <iostream>
#include <list>
#include <vector>

using namespace std;

// template<typename L>
// void straight(const L & l)
// {
//     for (auto elem: l)
//     {
//         cout << elem << endl;
//     }
// }

// template<typename L1>
// void reverse(const L1 & l)
// {
//     for (typename L1::const_reverse_iterator iter = l.rbegin(); iter != l.rend(); ++iter)
//     {
//         cout << *iter << endl;
//     }
// }

// template<typename T>
// void ins(const list<T> & l, vector<T> & v)
// {
//     typename vector<T>::const_iterator v_iter = v.cbegin();
//     v_iter +=2;
//     typename list<T>::const_iterator l_iter = l.cbegin();
//     l_iter ++;

//     while((v_iter <= v.cend())&& (l_iter != l.cend())){
//         v_iter = v.insert(v_iter , *l_iter);
//         v_iter +=3;
//         l_iter ++;
//         if(l_iter == l.cend()){
//             break;
//         }
//         l_iter ++;
//     }
//     if((v_iter <= v.cend()) && (l_iter == l.cend())){
//         while(v_iter <= v.cend()){
//             v_iter = v.insert( v_iter , 0);
//             v_iter +=3;
//         }
//     }
// }

// template<typename L>

// void del(L & l)
// {
//     int counter = 1;
//     for (auto it = l.begin(); it != l.end();)
//     {
//         if (counter % 3 == 0)
//             it = l.erase(it);
//         else
//             it++;
//             counter ++;
//     }
// }
 

#include <iostream>
using namespace std;
class A {
    public:
    const int i;
public:
     A():i(1) {cout << 1<< endl; };
    //A& operator=(const A& ra) = default; //{return *this;}
    
};
 
int main() {
    int x=0, u=0;
    x = u+1 = 1;
}

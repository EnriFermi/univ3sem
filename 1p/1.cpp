#include <iostream>
#include <cstring>

using namespace std;

class Str
{
private:
    char *str;
    int len;

public:
    Str() {
        cout << "str()" << endl;
        this->len = 0;
        this->str = new char[1];
        this->str[0] = 0;
    };
    Str(const char *s)
    {
        cout << "str(const char *s)" << endl;
        this->len = strlen(s);
        this->str = new char[len + 1];
        strcpy(str, s);
    }
    Str(const Str &s)
    {
        cout << "str(const Str &s)" << endl;
        this->len = s.len;
        this->str = new char[len + 1];
        strcpy(str, s.str);
    }

    ~Str()
    {
        cout << "~Str()" << endl;
        delete[] str;
    }

    Str &operator=(const Str &s)
    {
        cout << "str=" << endl;
        if (this != &s)
        {
            this->len = s.len;
            if(this->str != NULL){
                delete[] this->str;
            }
            this->str = new char[len + 1];
            strcpy(str, s.str);
        }
        return *this;
    }
    char *get_s()
    {
        return this->str;
    }
    int get_len()
    {
        return this->len;
    }
};
class Man{
    Str name;
    int age;
    public:
    Man() {
        cout << "Man()" << endl;
        this->age = 0;
        this->name = Str();
    };
    Man(Str n, int a){
        cout << "Man(Str n, int a)" << endl;
        this->age = a;
        this->name = Str(n);
    }
    //Man(Str n, int a): name(n), age(a){};

    Man(const Man &m)
    {
        cout << "Man(const Man &m)" << endl;
        this->age = m.age;
        this->name = Str(m.name);
    }

    ~Man()
    {
        cout << "~Man" << endl;
        cout << "man died :(" << endl;
    }

    Man &operator=(const Man &m)
    {
        cout << "man=" << endl;
        if (this != &m)
        {
            this->age = m.age;
            this->name = m.name;
        }
        return *this;
    }
    Man(Man&& m) :  age(m.age)
    {
        name = move(m.name);
        cout << "move" << std::endl;
    }
    
    Str get_name()
    {
        return this->name;
    }
    int get_age()
    {
        return this->age;
    }
};
Man f(){
    return Man();
}
int main()
{
    Str s("Johnes");
    cout << s.get_s() << endl;
    Man m0;
    m0 = f();
}
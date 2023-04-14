#include <iostream>
#include <cstring>
#include <ctime>
using namespace std;
class bank_account
{
private:
    static int number_generator;
    int number;
    char *owner;
    int creation_data;
    int balance;
    bank_account(int balance, char *owner);

public:
    int const get_number();
    char *const get_owner();
    int const get_creation_data();
    int const get_balance();
    void set_balance(int balance);
    void set_owner(char *owner);
    bank_account(char *owner);
    bank_account(const bank_account& p);
    bank_account( bank_account&& p): number(number),
    owner(p.owner), creation_data(p.creation_data), balance(balance){};
    void print();
    ~bank_account();
    bank_account &operator=(const bank_account &p)
    {
        if (this != &p)
        {
            this->balance = p.balance;
            this->creation_data = p.creation_data;
            if(this->owner != NULL){
                free(this->owner);
            }
            this->owner = strdup(p.owner);
        }
        return *this;
    }
    bank_account &operator=(bank_account &&p)
    {
        if (this != &p)
        {

            this->balance = p.balance;
            this->creation_data = p.creation_data;
            if(this->owner != NULL){
                free(this->owner);
            }
            this->owner = p.owner;
            p.owner = NULL;
        }
        return *this;
    }
    const bank_account operator+(const bank_account& right){
        return bank_account(right.balance + balance, right.owner);
    }
};
int bank_account::number_generator = 1;
//TODO Constructors
bank_account::bank_account(int balance, char *owner)
{
    this->number = number_generator++;
    this->owner = strdup(owner);
    this->creation_data = time(0);
    this->balance = balance;
};
bank_account::bank_account(const bank_account& p){
    this->number = number_generator++;
    this->owner = strdup(p.owner);
    this->creation_data = p.creation_data;
    this->balance = p.balance;
}
bank_account::bank_account(char *owner)
{
    this->number = number_generator++;
    this->owner = strdup(owner);
    this->creation_data = time(0);
    this->balance = 0;
};
bank_account::~bank_account()
{
    if(this->owner != NULL){
        free(this->owner);
    }
    
};
//TODO Setters)
int const bank_account::get_number(){
    return this->number;
};
char *const bank_account::get_owner(){
    return this->owner;
};
int const bank_account::get_creation_data(){
    return this->creation_data;
};
int const bank_account::get_balance(){
    return this->balance;
};
void bank_account::set_balance(int balance){
    this->balance = balance;
};
void bank_account::set_owner(char *owner){
    free(this->owner);
    this->owner = strdup(owner);
};
//TODO print
void bank_account::print(){
    cout << this->number << " " << this->owner << " " 
    << this->creation_data << " " << this->balance << "\n";
}

class list{
    private:
        class node{
            public:
                bank_account elem;
                node* next;
                node* prev;
                node(const bank_account& elem, node *prev, node * next): elem(elem), next(next), prev(prev){};
                ~node(){
                }


        };
        node* first;
        node* last;
        int lsize;
    public:
        list(){
            this->first = NULL;
            this->last = NULL;
            this->lsize = 0;
        }
        ~list(){
            node *p = this->first, *q;
            while (p != NULL){
                q = p->next;
                delete p;
                p = q;
            }
        }
        void push_front(const bank_account& elem);
        void push_back(const bank_account& elem);
        bank_account front();
        bank_account back();
        void pop_front();
        void pop_back();
        void insert(const bank_account& elem, int p);
        void erase(int p);
        bool empty();
        int size();
        void print();
};

void list::push_front(const bank_account& elem){
    if((this->first == this->last)&&(this->first == NULL)){
        node* n = new node(elem, NULL, NULL);
        this->first = n;
        this->last = n;
    } else {
        node *n = new node(elem, NULL, this->first);
        this->first->prev = n;
        this->first = n;
    }
    this->lsize++;
}
void list::push_back(const bank_account& elem){
    if((this->first == this->last)&&(this->first == NULL)){
        node* n = new node(elem, NULL, NULL);
        this->first = n;
        this->last = n;
    } else {
        node *n = new node(elem, this->last, NULL);
        this->last->next = n;
        this->last = n;
    }
    this->lsize++;
}
bank_account list::front(){
    return this->first->elem;
}
bank_account list::back(){
    return this->last->elem;
}

void list::pop_front(){
    if(this->first != NULL){
        if(this->first == this->last){
            delete this->first;
            this->first = NULL;
            this->last = NULL;
        } else {
            node *p = this->first->next;
            delete this->first;
            this->first = p;
            this->first->prev = NULL;
        }
        this->lsize--;
    }
    
}

void list::pop_back(){
    if(this->first != NULL){
        if(this->first == this->last){
            delete this->last;
            this->first = NULL;
            this->last = NULL;
        } else {
            node *p = this->last->prev;
            delete this->last;
            this->last = p;
            this->last->next = NULL;
        }
        this->lsize--;
    }
}
void list::insert(const bank_account &elem, int n){
    node * p = this->first;
    int counter = 0;
    while(counter < n && counter <= this->lsize){
        p = p->next;
        counter++;
    }
    if(counter == this->lsize){
        this->push_back(elem);
    } else if(counter == 0){
        this->push_front(elem);
    } else {
        node *q = new node(elem, p->prev, p);
        p->prev->next = q;
        p->prev = q;
        this->lsize++;
    }
}
void list::erase(int n){
    node * p = this->first;
    int counter = 0;
    while(counter < n && counter < this->lsize){
        p = p->next;
        counter++;
    }
    if(counter == this->lsize-1){
        this->pop_back();
    } else if(counter == 0){
        this->pop_front();
    } else {
        node *q = p->prev;
        p->prev->next = p->next;
        p->next->prev = q;
        delete p;
        this->lsize--;
    }
}
bool list::empty(){
    return this->first == NULL;
}
int list::size(){
    return this->lsize;
}
void list::print(){
    node * p = this->first;
    int counter = 0;
    while(p != NULL){
        p->elem.print();
        p = p->next;
        counter++;
    }
}

int main()
{
    char *s = strdup("Hello");
    bank_account a(s);
    s[0] = 'e';
    bank_account c(a);
    bank_account b(s);
    s[0] = 'F';
    c.set_owner(s);
    list l;
    l.push_front(a);
    l.push_back(c);
    l.insert(b, 1);
    l.print();
    cout << endl;
    l.erase(2);
    l.print();
    cout << endl;
    free(s);
}



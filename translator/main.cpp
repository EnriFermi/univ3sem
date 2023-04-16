#include <iostream>
#include <cstring>
#include <queue>
using namespace std;

//! FILL!
// TODO Lexem enum
enum type_of_lex
{
    LEX_NULL,
    // words
    LEX_PROGRAM,
    LEX_INT,
    LEX_STRING,
    LEX_BOOLEAN,
    LEX_IF,
    LEX_ELSE,
    LEX_WHILE,
    LEX_READ,
    LEX_WRITE,
    LEX_FOR,
    LEX_BREAK,
    LEX_GOTO,
    LEX_TRUE,
    LEX_FALSE,
    // symbols
    LEX_LFP,
    LEX_RFP,
    LEX_SEMICOLON,
    LEX_COMMA,
    LEX_ASSIGN,
    LEX_PLUS,
    LEX_MINUS,
    LEX_LP,
    LEX_RP,
    LEX_COLON,
    LEX_LSS,
    LEX_LEQ,
    LEX_GTR,
    LEX_GEQ,
    LEX_NOT,
    LEX_NEQ,
    LEX_EQ,
    LEX_OR,
    LEX_TIMES,
    LEX_SLASH,
    LEX_AND,
    LEX_DQU,
    // num
    LEX_NUM,
    LEX_ID,
    LEX_STR,
    POLIZ_LABEL,
    POLIZ_ADDRESS,
    POLIZ_GO,
    POLIZ_FGO
};

// TODO Lexem class
class Lex
{
    type_of_lex t_lex;
    void *v_lex;

public:
    Lex(type_of_lex t = LEX_NULL, void *v = NULL)
    {
        t_lex = t;
        v_lex = v;
    }
    type_of_lex get_type() { return t_lex; }
    void *get_value() { return v_lex; }
    friend ostream &operator<<(ostream &s, Lex l)
    {
        s << '(' << l.t_lex /*<<"," << ((char*)l.v_lex)[0]*/ << ");";
        return s;
    }
};

// TODO Identifier class
class Ident
{
    char *name;
    bool declare;
    type_of_lex type;
    bool assign;
    void *value;

public:
    Ident()
    {
        declare = false;
        assign = false;
    }
    char *get_name()
    {
        return name;
    }
    void put_name(const char *n)
    {
        name = new char[strlen(n) + 1];
        strcpy(name, n);
    }
    bool get_declare()
    {
        return declare;
    }
    void put_declare()
    {
        declare = true;
    }
    type_of_lex get_type()
    {
        return type;
    }
    void put_type(type_of_lex t)
    {
        type = t;
    }
    bool get_assign()
    {
        return assign;
    }
    void put_assign()
    {
        assign = true;
    }
    void *get_value()
    {
        return value;
    }
    void put_value(void *v)
    {
        value = v;
    }
};

// TODO Ident table
class tabl_ident
{
    Ident *p;
    int size;
    int top;

public:
    tabl_ident(int max_size)
    {
        p = new Ident[size = max_size];
        top = 1;
    }
    ~tabl_ident()
    {
        delete[] p;
    }
    Ident &operator[](int k)
    {
        return p[k];
    }
    int put(const char *buf);
};
int tabl_ident::put(const char *buf)
{
    for (int j = 1; j < top; ++j)
        if (!strcmp(buf, p[j].get_name()))
            return j;
    p[top].put_name(buf);
    ++top;
    return top - 1;
}

// TODO main lexical recognition class
enum state
{
    H,              // Initial
    IDENT,          // Char
    NUMB,           // Int
    STRING,         // String
    D_COM,          // /**/ or /
    ALE_NEQ_NEG_EQ, //<,>,<=,>=,=,==,!=,!
    DELIM,          // +,-,*,___,___,;,(,),=
    ORAND,          // or or and
    COM             // comments
};
class Scanner
{

    static char *TW[];          // Appropriated service words table (const)
    static type_of_lex words[]; // Service words table
    static char *TD[];          // Appropriated discriminative words table (const)
    static type_of_lex dlms[];  // Discriminative words table
    state CS;
    FILE *fp; // processing file
    char c;
    char buf[80];
    int buf_top;

    void clear()
    {
        buf_top = 0;
        for (int j = 0; j < 80; ++j)
            buf[j] = '\0';
    }
    void add()
    {
        buf[buf_top++] = c;
    }
    int look(const char *buf, char **list)
    {
        int i = 0;
        while (list[i])
        {
            if (!strcmp(buf, list[i]))
                return i;
            ++i;
        }
        return 0;
    }
    void gc()
    {
        c = fgetc(fp);
    }

public:
    Lex get_lex();
    Scanner(const char *program)
    {
        fp = fopen(program, "r");
        CS = H;
        clear();
        gc();
    }
};
// TODO Appropriated service words table (const)
char *Scanner::TW[] =
    {
        "",
        "program",
        "int",
        "string",
        "bool",
        "if",
        "else",
        "while",
        "read",
        "write",
        "for",
        "break",
        "goto",
        "false",
        "true",
        NULL};

// TODO Appropriated discriminative words table (const)
char *Scanner::TD[] =
    {
        "",
        "{",
        "}",
        ";",
        ",",
        "=",
        "+",
        "-",
        "(",
        ")",
        ":",
        "<",
        "<=",
        ">",
        ">=",
        "!",
        "!=",
        "==",
        "||",
        "*",
        "/",
        "&&",
        "\"",
        NULL

};
// TODO Variables table
tabl_ident TID(100);

type_of_lex Scanner::words[] =
    {
        LEX_NULL,
        LEX_PROGRAM,
        LEX_INT,
        LEX_STRING,
        LEX_BOOLEAN,
        LEX_IF,
        LEX_ELSE,
        LEX_WHILE,
        LEX_READ,
        LEX_WRITE,
        LEX_FOR,
        LEX_BREAK,
        LEX_GOTO,
        LEX_FALSE,
        LEX_TRUE,
        LEX_NULL};
type_of_lex Scanner::dlms[] =
    {
        LEX_NULL,
        LEX_LFP,
        LEX_RFP,
        LEX_SEMICOLON,
        LEX_COMMA,
        LEX_ASSIGN,
        LEX_PLUS,
        LEX_MINUS,
        LEX_LP,
        LEX_RP,
        LEX_COLON,
        LEX_LSS,
        LEX_LEQ,
        LEX_GTR,
        LEX_GEQ,
        LEX_NOT,
        LEX_NEQ,
        LEX_EQ,
        LEX_OR,
        LEX_TIMES,
        LEX_SLASH,
        LEX_AND,
        LEX_DQU,
        LEX_NULL};

Lex Scanner::get_lex()
{
    int d, j;
    char p;
    CS = H;
    do
    {
        switch (CS)
        {
        case H: // Initial state
            if (c == EOF)
                return Lex(LEX_NULL, 0);
            else if (c == ' ' || c == '\n' || c == '\r' || c == '\t') // Ignorable symbols
                gc();
            else if (c == '"')
            {
                clear();
                gc();
                CS = STRING;
            }
            else if (c == '|' || c == '&')
            { // or or Ident(begins with o)
                clear();
                add();
                gc();
                CS = ORAND;
            }
            else if (isalpha(c)) // Ident begin(not begins with a or o)
            {
                clear();
                add();
                gc();
                CS = IDENT;
            }
            else if (isdigit(c)) // Number begin
            {
                d = c - '0';
                gc();
                CS = NUMB;
            }
            else if (c == '/') // Comment begin or derivation
            {
                clear();
                add();
                gc();
                CS = D_COM;
            }
            else if (c == '<' || c == '>' || c == '!' || c == '=') // <, >, >=, <=, /!=, !(not), =, =
            {
                clear();
                add();
                gc();
                CS = ALE_NEQ_NEG_EQ;
            }
            else // other one letter Lexems
                CS = DELIM;
            break;
        case STRING:
            if (c != '"')
            {
                add();
            }
            else
            {
                char *value = new char[strlen(buf) + 1];
                strcpy(value, buf);
                gc();
                return Lex(LEX_STR, (void *)value);
            }
            gc();
            break;
        case ORAND:
            add();
            if (j = look(buf, TD))
            {
                gc();
                return Lex(dlms[j], (void *)new int(j));
            }
            else
                throw c;
            break;
        case IDENT:
            if (isalpha(c) || isdigit(c))
            {
                add();
                gc();
            }
            else if (j = look(buf, TW))
                return Lex(words[j], (void *)new int(j));
            else
            {
                j = TID.put(buf);
                return Lex(LEX_ID, (void *)new int(j));
            }
            break;
        case NUMB:
            if (isdigit(c))
            {
                d = d * 10 + (c - '0');
                gc();
            }
            else
            {
                return Lex(LEX_NUM, (void *)new int(d));
            }

            break;
        case D_COM:
            if (c == '*')
            {
                gc();
                p = c; // initialising
                CS = COM;
            }
            else if (j = look(buf, TD))
                return Lex(dlms[j], (void *)new int(j));
            else
                throw c;
            break;
        case COM:
            if (c == '/' && p == '*') //! DANGER LOGIC
            {
                gc();
                CS = H;
            }
            else if (c == EOF)
                throw c;
            else
            {
                p = c;
                gc();
            }
            break;
        case ALE_NEQ_NEG_EQ:
            if (c == '=')
            {
                add();
                gc();
                j = look(buf, TD);
                return Lex(dlms[j], (void *)new int(j));
            }
            else
            {
                j = look(buf, TD);
                return Lex(dlms[j], (void *)new int(j));
            }
            break;
        case DELIM:
            clear();
            add();
            if (j = look(buf, TD))
            {
                gc();
                return Lex(dlms[j], (void *)new int(j));
            }
            else
                throw c;
            break;
        }
        // end switch
    } while (true);
}

// TODO Stack class
template <class T, int max_size>
class Stack
{
    T s[max_size];
    int top;

public:
    Stack() { top = 0; }
    void reset() { top = 0; }
    void push(T i);
    T pop();
    bool is_empty() { return top == 0; }
    bool is_full() { return top == max_size; }
};

template <class T, int max_size>
void Stack<T, max_size>::push(T i)
{
    if (!is_full())
    {
        s[top] = i;
        ++top;
    }
    else
        throw "Stack_is_full";
}
template <class T, int max_size>
T Stack<T, max_size>::pop()
{
    if (!is_empty())
    {
        --top;
        return s[top];
    }
    else
        throw "Stack_is_empty";
}
// TODO Poliz class
class Poliz
{
    Lex *p;
    int size;
    int free;

public:
    Poliz(int max_size)
    {
        p = new Lex[size = max_size];
        free = 0;
    };
    ~Poliz() { delete[] p; };
    void put_lex(Lex l)
    {
        p[free] = l;
        ++free;
    };
    void put_lex(Lex l, int place) { p[place] = l; };
    void blank() { ++free; };
    int get_free() { return free; };
    Lex &operator[](int index)
    {
        if (index > size)
            throw "POLIZ:out of array";
        else if (index > free)
            throw "POLIZ:indefinite element of array";
        else
            return p[index];
    };
    void print()
    {
        for (int i = 0; i < free; ++i)
            cout << p[i];
    };
};

// TODO Syntax parser
class Parser
{
    struct var_info
    {
        int index_in_TID;
        void *initial_value;
        var_info(int i, void *val) : index_in_TID(i), initial_value(val){};
        var_info(){};
    };
    Lex curr_lex; // текущая лексема
    type_of_lex c_type;
    type_of_lex v_type;
    int c_val;
    Scanner scan;
    Stack<var_info, 100> st_var;
    Stack<type_of_lex, 100> st_lex;
    queue<Lex> q_lex;
    bool is_q_read = false; //read from queue or from scanner
    bool is_desc = false;
    bool is_oper = false;
    void P(); // процедуры РС-метода
    void D1();
    void D();
    void T();
    void V();
    void O1();
    void O();
    void E();
    void E1();
    ////void T();
    void F();
    void dec(type_of_lex type);
    void check_id();
    void check_op();
    void check_not();
    void eq_type();
    void eq_bool();
    void check_id_in_read();
    // семантичиеские действия
    void gl()
    // получить очередную лексему
    {
        is_q_read = !q_lex.empty(); // if empty switch to normal regime
        if(is_q_read){
            curr_lex = q_lex.front();
            q_lex.pop();
        } else {
            curr_lex = scan.get_lex();
        }
        c_type = curr_lex.get_type();
        c_val = ((int *)curr_lex.get_value())[0]; // could be some random int
    }

public:
    Poliz prog = Poliz(1000); // LATER
    // внутреннее представление программы
    Parser(const char *program) : scan(program) /*,prog(1000)*/ {}
    void analyze(); // анализатор с действиями
};
// main function
void Parser::analyze()
{
    gl();
    P();
    prog.print();
    cout << endl
         << "Yes!!!" << endl;
}
//? Don't know
void Parser::dec(type_of_lex type)
{
    var_info info;
    int i;
    while (!st_var.is_empty())
    {
        info = st_var.pop();
        i = info.index_in_TID;
        if (TID[i].get_declare())
            throw "twice";
        else
        {
            TID[i].put_declare();
            TID[i].put_type(type);
            TID[i].put_value(info.initial_value); // if no initialization value is NULL
        }
    }
}

void Parser::check_id()
{
    if (TID[c_val].get_declare())
        st_lex.push(TID[c_val].get_type());
    else
        throw "not declared";
}
void Parser::check_op() // Checks operation types
{
    type_of_lex t1, t2, op, t = LEX_INT, r = LEX_BOOLEAN;
    t2 = st_lex.pop();
    op = st_lex.pop();
    t1 = st_lex.pop();
    if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH)
        r = LEX_INT;
    if (op == LEX_OR || op == LEX_AND)
        t = LEX_BOOLEAN;
    if (t1 == t2 && t1 == t)
        st_lex.push(r);
    else
        throw "wrong types are in operation";
}

void Parser::check_not()
{
    if (st_lex.pop() != LEX_BOOLEAN)
        throw "wrong type is in not";
    else
    {
        st_lex.push(LEX_BOOLEAN);
    }
}
void Parser::eq_type()
{
    if (st_lex.pop() != st_lex.pop())
        throw "wrong types are in :=";
}

void Parser::eq_bool()
{
    if (st_lex.pop() != LEX_BOOLEAN)
        throw "expression is not boolean";
}

void Parser::check_id_in_read()
{
    if (!TID[c_val].get_declare())
        throw "not declared";
}

// TODO Program parser
//?DONE
void Parser::P()
{
    if (c_type == LEX_PROGRAM)
        gl();
    else
        throw curr_lex;
    if (c_type == LEX_LFP)
        gl();
    else
        throw curr_lex;
    D1();
    O1();
    if (c_type != LEX_RFP)
        throw curr_lex;
}
// TODO Descriptions
//?DONE
void Parser::D1()
{
    do
    {
        D();
        if (is_desc)
            if (c_type == LEX_SEMICOLON)
                gl();
            else
                throw curr_lex;

    } while (is_desc);
}
// TODO Description
//! IF not lexem, dont move!!!!
void Parser::D()
{
    st_var.reset();
    if (c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOLEAN)
    { // type defining
        v_type = c_type;
        is_desc = true;
    }
    else
    {
        is_desc = false;
        return; // no descriptions (probably)
    }
    gl();
    V();
    while (c_type == LEX_COMMA)
    {
        gl();
        V();
    }
    dec(v_type); // saving what we initialized
}

// TODO Variable parser
void Parser::V()
{
    bool need_minus = false;
    if (c_type != LEX_ID)
        throw curr_lex;
    else
    {
        int idx = c_val;
        void *val = NULL;
        int *buffer;
        gl();
        if (c_type == LEX_ASSIGN)
        { // const initialization
            gl();

            if (c_type == LEX_MINUS)
            { // check if sign exist exist
                need_minus = true;
                gl();
            }
            else if (c_type == LEX_PLUS)
                gl();

            buffer = (int *)curr_lex.get_value();          // get buffer fro future
            if (c_type == LEX_STR && v_type == LEX_STRING) // STRING
                val = curr_lex.get_value();
            else if (c_type == LEX_NUM && v_type == LEX_INT)
            { // INT
                if (need_minus)
                    buffer[0] = buffer[0] * (-1);
                val = curr_lex.get_value();
            }
            else if ((c_type == LEX_TRUE || c_type == LEX_FALSE) && (v_type == LEX_INT || v_type == LEX_BOOLEAN))
            {                               // BOOLEAN
                buffer[0] = buffer[0] - 13; // change values from 13, 14(indicies of words) to 0, 1
                val = curr_lex.get_value();
            }
            else
                throw curr_lex;
            gl();
        }
        // cout <<  ((int *) val)[0] << endl;
        st_var.push(var_info(idx, val));
    }
}
// New comment for git
// One more new comment for git

// TODO Operators parser
void Parser::O1()
{
    do
    {
        O();
    } while (is_desc);
}
// TODO Operator parser
//? DONE
void Parser::O()
{
    int pl0, pl1, pl2, pl3;
    if (c_type == LEX_IF) // IF operator
    {
        gl();
        if (c_type != LEX_LP)
            throw curr_lex;
        gl();
        E();
        //!eq_bool();
        pl2 = prog.get_free();        // get begin IF operator
        prog.blank();                 // create blank space
        prog.put_lex(Lex(POLIZ_FGO)); //! ???
        if (c_type == LEX_RP)
        {
            gl();
            O();
            pl3 = prog.get_free();
            prog.blank();
            prog.put_lex(Lex(POLIZ_GO));
            prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(prog.get_free())), pl2);
            if (c_type == LEX_ELSE)
            {
                gl();
                O();
                prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(prog.get_free())), pl3);
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;
    }
    else if (c_type == LEX_WHILE) // WHILE operator
    {
        pl0 = prog.get_free();
        gl();
        if(c_type != LEX_LP)
            throw curr_lex;
        gl();
        E();
        //!eq_bool();
        pl1 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        if (c_type == LEX_RP)
        {
            gl();
            O();
            prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(pl0)));
            prog.put_lex(Lex(POLIZ_GO));
            prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(prog.get_free())), pl1);
        }
        else
            throw curr_lex;
    }
    else if (c_type == LEX_FOR) //! FUCK FOR CYCLES
    { // FOR operator
         //! for saving changing action
        gl();
        if(c_type != LEX_LP)
            throw curr_lex;
        
        gl();
        E(); //Initialising
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
        
        pl0 = prog.get_free(); //Condition address saving
        gl();
        E();
        //!eq_bool();
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
        
        gl();
        while(c_type != LEX_RP){ // Save changing action for later insertion
            q_lex.push(curr_lex); //Dangerous assign
            gl();
        }

        pl1 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        if (c_type == LEX_RP)
        {
            gl();
            O();
            q_lex.push(curr_lex); //save first after cycle lex
            is_q_read = true;
            E();
            prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(pl0)));
            prog.put_lex(Lex(POLIZ_GO));
            prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(prog.get_free())), pl1);
        }
        else
            throw curr_lex;
    }
    /*else if (c_type == LEX_READ) // READ operator
    {
        gl();
        if (c_type == LEX_LPAREN)
        {
            gl();
            if (c_type == LEX_ID)
            {
                check_id_in_read();
                prog.put_lex(Lex(POLIZ_ADDRESS, c_val));
                gl();
            }
            else
                throw curr_lex;
            if (c_type == LEX_RPAREN)
            {
                gl();
                prog.put_lex(Lex(LEX_READ));
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;
    }
    else if (c_type == LEX_WRITE) // WRITE operator
    {
        gl();
        if (c_type == LEX_LPAREN)
        {
            gl();
            E();
            if (c_type == LEX_RPAREN)
            {
                gl();
                prog.put_lex(Lex(LEX_WRITE));
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;
    }*/
    else if (c_type == LEX_ID) // ASSIGN or //! LABELED operator
    {
        //!check_id();
        prog.put_lex(Lex(POLIZ_ADDRESS, (void*) new int(c_val)));
        gl();
        if (c_type == LEX_ASSIGN)
        {
            gl();
            E();
            //!eq_type();
            prog.put_lex(Lex(LEX_ASSIGN));
            if(c_type != LEX_SEMICOLON)
                throw curr_lex;
            gl();
        }
        else
            throw curr_lex;
    }
    else if (c_type == LEX_LFP)//? DONE
    { // COMBINED operator
        if(c_type != LEX_LFP)
            throw curr_lex;
        gl();
        O1();
        if(c_type != LEX_RFP)
            throw curr_lex;
        gl();
    }
    /*else if (c_type == LEX_GOTO)
    { // GOTO operator

    }
    else if (c_type == LEX_BREAK)
    { // BREAK operator

    }*/
    else{
        is_desc = false;
    }
}
void Parser::E()
{
    while(c_type != LEX_SLASH){
        gl();
    }
    prog.put_lex(Lex(LEX_NULL));
    gl();
}
//     // check_id();
//     // prog.put_lex(Lex(POLIZ_ADDRESS, (void *)new int(c_val)));
//     // gl();
//     // if (c_type == LEX_ASSIGN)
//     // {
//     //     gl();
//     //     E();
//     //     eq_type();
//     //     prog.put_lex(Lex(LEX_ASSIGN));
//     //     if (c_type != LEX_SEMICOLON)
//     //         throw curr_lex;
//     //     gl();
//     // }
//     // else
//     //     throw curr_lex;


    // E1();
    // if (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
    //     c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ)
    // {
    //     st_lex.push(c_type);
    //     gl();
    //     E1();
    //     check_op();
    // }
// }
// void Parser::E1()
// {
//     T();
//     while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR)
//     {
//         st_lex.push(c_type);
//         gl();
//         T();
//         check_op();
//     }
// }
// void Parser::T()
// {
//     F();
//     while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND)
//     {
//         st_lex.push(c_type);
//         gl();
//         F();
//         check_op();
//     }
// }
// void Parser::F()
// {
//     if (c_type == LEX_ID)
//     {
//         check_id();
//         prog.put_lex(Lex(LEX_ID, c_val));
//         gl();
//     }
//     else if (c_type == LEX_NUM)
//     {
//         st_lex.push(LEX_INT);
//         prog.put_lex(curr_lex);
//         gl();
//     }
//     else if (c_type == LEX_TRUE)
//     {
//         st_lex.push(LEX_BOOL);
//         prog.put_lex(Lex(LEX_TRUE, 1));
//         gl();
//     }
//     else if (c_type == LEX_FALSE)
//     {
//         st_lex.push(LEX_BOOL);
//         prog.put_lex(Lex(LEX_FALSE, 0));
//         gl();
//     }
//     else if (c_type == LEX_NOT)
//     {
//         gl();
//         F();
//         check_not();
//     }
//     else if (c_type == LEX_LPAREN)
//     {
//         gl();
//         E();
//         if (c_type == LEX_RPAREN)
//             gl();
//         else
//             throw curr_lex;
//     }
//     else
//         throw curr_lex;
// }

/*
//TODO POLIZ Executer
class Executer
{
    Lex pc_el;
public:
    void execute(Poliz &prog);
};

void Executer::execute(Poliz &prog)
{
    Stack<int, 100> args;
    int i, j, index = 0, size = prog.get_free();
    while (index < size)
    {
        pc_el = prog[index];
        switch (pc_el.get_type())
        {
        case LEX_TRUE:
        case LEX_FALSE:
        case LEX_NUM:
        case POLIZ_ADDRESS:
        case POLIZ_LABEL:
            args.push(pc_el.get_value());
            break;
        case LEX_ID:
            i = pc_el.get_value();
            if (TID[i].get_assign())
            {
                args.push(TID[i].get_value());
                break;
            }
            else
                throw "POLIZ: indefinite identifier";
        case LEX_NOT:
            args.push(!args.pop());
            break;
        case LEX_OR:
            i = args.pop();
            args.push(args.pop() || i);
            break;
        case LEX_AND:
            i = args.pop();
            args.push(args.pop() && i);
            break;
        case POLIZ_GO:
            index = args.pop() - 1;
            break;
        case POLIZ_FGO:
            i = args.pop();
            if (!args.pop())
                index = i - 1;
            break;
        case LEX_WRITE:
            cout << args.pop() << endl;
            break;
        case LEX_READ:
        {
            int k;
            i = args.pop();
            if (TID[i].get_type() == LEX_INT)
            {
                cout << "Input int value for";
                cout << TID[i].get_name() << endl;
                cin >> k;
            }
            else
            {
                char j[20];
            rep:
cout << "Input boolean value";
cout << "(true or false) for";
cout << TID[i].get_name() << endl;
cin >> j;
if (!strcmp(j, "true"))
    k = 1;
else if (!strcmp(j, "false"))
    k = 0;
else
{
    cout << "Error in input:true/false";
    cout << endl;
    goto rep;
}
            }
            TID[i].put_value(k);
            TID[i].put_assign();
            break;
        }
        case LEX_PLUS:
            args.push(args.pop() + args.pop());
            break;
        case LEX_TIMES:
            args.push(args.pop() * args.pop());
            break;
        case LEX_MINUS:
            i = args.pop();
            args.push(args.pop() - i);
            break;
        case LEX_SLASH:
            i = args.pop();
            if (!i)
            {
args.push(args.pop() / i);
break;
            }
            else
throw "POLIZ:divide by zero";
        case LEX_EQ:
            args.push(args.pop() == args.pop());
            break;
        case LEX_LSS:
            i = args.pop();
            args.push(args.pop() < i);
            break;
        case LEX_GTR:
            i = args.pop();
            args.push(args.pop() > i);
            break;
        case LEX_LEQ:
            i = args.pop();
            args.push(args.pop() <= i);
            break;
        case LEX_GEQ:
            i = args.pop();
            args.push(args.pop() >= i);
            break;
        case LEX_NEQ:
            i = args.pop();
            args.push(args.pop() != i);
            break;
        case LEX_ASSIGN:
            i = args.pop();
            j = args.pop();
            TID[j].put_value(i);
            TID[j].put_assign();
            break;
        default:
            throw "POLIZ: unexpected elem";
        }
        // end of switch
        ++index;
    };
    // end of while
    cout << "Finish of executing!!!" << endl;
}
class Interpretator
{
    Parser pars;
    Executer E;

public:
    Interpretator(char *program) : pars(program){};
    void interpretation();
};
void Interpretator::interpretation()
{
    pars.analyze();
    E.execute(pars.prog);
}
*/



int main()
{
    Parser p = Parser("test.txt");
    p.analyze();
    for (int i = 0; i < 100; i++)
    {
        if (TID[i].get_name() != NULL)
        {
            cout << TID[i].get_name() << " ";
        }
        if (TID[i].get_value() != NULL)
        {
            cout << ((int *)TID[i].get_value())[0];
        }
        cout << endl;
    }
}
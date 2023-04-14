#include <iostream>
#include <cstring>
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
        "boolean",
        "if",
        "else",
        "while",
        "read",
        "write",
        "for",
        "break",
        "goto",
        "true",
        "false",
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
        LEX_TRUE,
        LEX_FALSE,
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
                CS = COM;
            }
            else if (j = look(buf, TD))
                return Lex(dlms[j], (void *)new int(j));
            else
                throw c;
            break;
        case COM:
            p = c;                  // initialising
            if (c == '/', p == '*') //! DANGER LOGIC
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

// TODO Syntax parser
class Parser
{
    struct var_info{
        int index_in_TID;
        void * initial_value;
        var_info(int i, void * val): index_in_TID(i), initial_value(val){};
        var_info(){};
    };
    Lex curr_lex; // текущая лексема
    type_of_lex c_type;
    int c_val;
    Scanner scan;
    Stack<var_info, 100> st_var;
    Stack<type_of_lex, 100> st_lex;
    bool is_desc = false;
    bool is_oper = false;
    void P(); // процедуры РС-метода
    void D1();
    void D();
    void T();
    void V();
    void O1();
    void S();
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
        curr_lex = scan.get_lex();
        c_type = curr_lex.get_type();
        c_val = ((int*) curr_lex.get_value())[0] ; //could be some random int
    }

public:
    //Poliz prog; // LATER
    // внутреннее представление программы
    Parser(const char *program) : scan(program)/*,prog(1000)*/ {}
    void analyze(); // анализатор с действиями
};
//main function
void Parser::analyze()
{
    gl();
    P();
    //prog.print();
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
void Parser::check_op()
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
    // B();
    // if (c_type != LEX_RFP)
    //     throw curr_lex;
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
    type_of_lex v_type;
    if(c_type == LEX_INT || c_type == LEX_STRING || c_type == LEX_BOOLEAN){ //type defining
        v_type = c_type;
        is_desc = true;
    }else{
        is_desc = false;
        return; //no descriptions (probably)
    }
    gl();
    V();
    while (c_type == LEX_COMMA)
    {
        gl();
        V();
    } 
    dec(v_type); //saving what we initialized
}
    // if (c_type != LEX_ID)
    //     throw curr_lex;
    // else
    // {
    //     st_int.push(c_val);
    //     gl();
    //     while (c_type == LEX_COMMA)
    //     {
    //         gl();
    //         if (c_type != LEX_ID)
    //             throw curr_lex;
    //         else
    //         {
    //             st_int.push(c_val);
    //             gl();
    //         }
    //     }
    //     if (c_type != LEX_COLON)
    //         throw curr_lex;
    //     else
    //     {
    //         gl();
    //         if (c_type == LEX_INT)
    //         {
    //             dec(LEX_INT);
    //             gl();
    //         }
    //         else if (c_type == LEX_BOOL)
    //         {
    //             dec(LEX_BOOL);
    //             gl();
    //         }
    //         else
    //             throw curr_lex;
    //     }
    // }

// TODO Variable parser
void Parser::V()
{
    if (c_type != LEX_ID)
        throw curr_lex;
    else
    {
        int idx = c_val;
        void * val = NULL;
        gl();
        if(c_type == LEX_ASSIGN){ //const initialization
            gl();
            if(c_type == LEX_NUM || c_type == LEX_STR || c_type == LEX_TRUE || c_type == LEX_FALSE) //check if init value is correct type
                val = curr_lex.get_value();
            else
                throw curr_lex;
            gl();
        }
        //cout <<  ((int *) val)[0] << endl;
        st_var.push(var_info(idx,val));
    }
}
//New comment for git
//One more new comment for git
/*
//Operators parser
void Parser::O1()
{
    if (c_type == LEX_BEGIN)
    {
        gl();
        S();
        while (c_type == LEX_SEMICOLON)
        {
            gl();
            S();
        }
        if (c_type == LEX_END)
            gl();
        else
            throw curr_lex;
    }
    else
        throw curr_lex;
}
void Parser::S()
{
    int pl0, pl1, pl2, pl3;
    if (c_type == LEX_IF)
    {
        gl();
        E();
        eq_bool();
        pl2 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        if (c_type == LEX_THEN)
        {
            gl();
            S();
            pl3 = prog.get_free();
            prog.blank();
            prog.put_lex(Lex(POLIZ_GO));
            prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl2);
            if (c_type == LEX_ELSE)
            {
                gl();
                S();
                prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl3);
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;
    } // end if
    else if (c_type == LEX_WHILE)
    {
        pl0 = prog.get_free();
        gl();
        E();
        eq_bool();
        pl1 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        if (c_type == LEX_DO)
        {
            gl();
            S();
            prog.put_lex(Lex(POLIZ_LABEL, pl0));
            prog.put_lex(Lex(POLIZ_GO));
            prog.put_lex(Lex(POLIZ_LABEL, prog.get_free()), pl1);
        }
        else
            throw curr_lex;
    } // end while
    else if (c_type == LEX_READ)
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
    } // end read
    else if (c_type == LEX_WRITE)
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
    } // end write
    else if (c_type == LEX_ID)
    {
        check_id();
        prog.put_lex(Lex(POLIZ_ADDRESS, c_val));
        gl();
        if (c_type == LEX_ASSIGN)
        {
            gl();
            E();
            eq_type();
            prog.put_lex(Lex(LEX_ASSIGN));
        }
        else
            throw curr_lex;
    } // assign-end
    else
        B();
}
void Parser::E()
{
    E1();
    if (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
        c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ)
    {
        st_lex.push(c_type);
        gl();
        E1();
        check_op();
    }
}
void Parser::E1()
{
    T();
    while (c_type == LEX_PLUS || c_type == LEX_MINUS || c_type == LEX_OR)
    {
        st_lex.push(c_type);
        gl();
        T();
        check_op();
    }
}
void Parser::T()
{
    F();
    while (c_type == LEX_TIMES || c_type == LEX_SLASH || c_type == LEX_AND)
    {
        st_lex.push(c_type);
        gl();
        F();
        check_op();
    }
}
void Parser::F()
{
    if (c_type == LEX_ID)
    {
        check_id();
        prog.put_lex(Lex(LEX_ID, c_val));
        gl();
    }
    else if (c_type == LEX_NUM)
    {
        st_lex.push(LEX_INT);
        prog.put_lex(curr_lex);
        gl();
    }
    else if (c_type == LEX_TRUE)
    {
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_TRUE, 1));
        gl();
    }
    else if (c_type == LEX_FALSE)
    {
        st_lex.push(LEX_BOOL);
        prog.put_lex(Lex(LEX_FALSE, 0));
        gl();
    }
    else if (c_type == LEX_NOT)
    {
        gl();
        F();
        check_not();
    }
    else if (c_type == LEX_LPAREN)
    {
        gl();
        E();
        if (c_type == LEX_RPAREN)
            gl();
        else
            throw curr_lex;
    }
    else
        throw curr_lex;
}
*/
int main()
{
    Parser p = Parser("test.txt");
    p.analyze();
    for(int i=0; i<100; i++){
        if(TID[i].get_name() != NULL){
            cout << TID[i].get_name() << " ";
        }
        if(TID[i].get_value() != NULL){
            cout << ((int*) TID[i].get_value())[0];
        } 
        cout << endl;
    }
}
#include <iostream>
#include <cstring>
#include <string>
#include <map>
#include <queue>
#include <stack>
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
    POLIZ_FGO,
    LEX_LINK
};
//More understandable enum output
ostream& operator<<(std::ostream& out, const type_of_lex value){
    static map<type_of_lex, std::string> strings;
    if (strings.size() == 0){
#define INSERT_ELEMENT(p) strings[p] = #p
        INSERT_ELEMENT(LEX_NULL);     
        INSERT_ELEMENT(LEX_PROGRAM);
        INSERT_ELEMENT(LEX_INT);
        INSERT_ELEMENT(LEX_STRING);
        INSERT_ELEMENT(LEX_BOOLEAN);
        INSERT_ELEMENT(LEX_IF);
        INSERT_ELEMENT(LEX_ELSE);
        INSERT_ELEMENT(LEX_WHILE);
        INSERT_ELEMENT(LEX_READ);
        INSERT_ELEMENT(LEX_WRITE);
        INSERT_ELEMENT(LEX_FOR);
        INSERT_ELEMENT(LEX_BREAK);
        INSERT_ELEMENT(LEX_GOTO);
        INSERT_ELEMENT(LEX_TRUE);
        INSERT_ELEMENT(LEX_FALSE);
        INSERT_ELEMENT(LEX_LFP);
        INSERT_ELEMENT(LEX_RFP);
        INSERT_ELEMENT(LEX_SEMICOLON);
        INSERT_ELEMENT(LEX_COMMA);
        INSERT_ELEMENT(LEX_ASSIGN);
        INSERT_ELEMENT(LEX_PLUS);
        INSERT_ELEMENT(LEX_MINUS);
        INSERT_ELEMENT(LEX_LP);
        INSERT_ELEMENT(LEX_RP);
        INSERT_ELEMENT(LEX_COLON);
        INSERT_ELEMENT(LEX_LSS);
        INSERT_ELEMENT(LEX_LEQ);
        INSERT_ELEMENT(LEX_GTR);
        INSERT_ELEMENT(LEX_GEQ);
        INSERT_ELEMENT(LEX_NOT);
        INSERT_ELEMENT(LEX_NEQ);
        INSERT_ELEMENT(LEX_EQ);
        INSERT_ELEMENT(LEX_OR);
        INSERT_ELEMENT(LEX_TIMES);
        INSERT_ELEMENT(LEX_SLASH);
        INSERT_ELEMENT(LEX_AND);
        INSERT_ELEMENT(LEX_DQU);
        INSERT_ELEMENT(LEX_NUM);
        INSERT_ELEMENT(LEX_ID);
        INSERT_ELEMENT(LEX_STR);
        INSERT_ELEMENT(POLIZ_LABEL);
        INSERT_ELEMENT(POLIZ_ADDRESS);
        INSERT_ELEMENT(POLIZ_GO);
        INSERT_ELEMENT(POLIZ_FGO);
        INSERT_ELEMENT(LEX_LINK);
#undef INSERT_ELEMENT
    }
    return out << strings[value];
}



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
        s << '(' << l.t_lex /*<<"," << ((char*)l.v_lex)[0]*/<< " " << ((l.v_lex != NULL)?((int*) l.v_lex)[0]:0) << "); ";
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
#define CBUF_SIZE 80
class Scanner
{

    static char *TW[];          // Appropriated service words table (const)
    static type_of_lex words[]; // Service words table
    static char *TD[];          // Appropriated discriminative words table (const)
    static type_of_lex dlms[];  // Discriminative words table
    state CS;
    FILE *fp; // processing file
    char c;
    char *buf;
    int buf_top;
    int buf_size=0;
    void clear()
    {
        buf_top = 0;
        for (int j = 0; j < buf_size; ++j)
            buf[j] = '\0';
    }
    void add()
    {
        if(buf_top  > buf_size){
            buf_size += CBUF_SIZE;
            buf =(char *) realloc(buf, buf_size);
        }
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
        buf_size = CBUF_SIZE;
        buf =(char *) malloc(CBUF_SIZE);
        clear();
        gc();
    }
    ~Scanner(){
        free(buf);
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
                string *value = new string(buf); //!Dangerous conversion
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
            if (c == '/' && p == '*')
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

// // TODO Stack class
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
            cout << i  << " " << p[i];
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
    Stack<var_info, 100> s_goto_id;
    stack<int> s_break;
    queue<Lex> q_buffer; //! DELETE
    stack<queue<Lex>> sq_buffer;
    stack<Lex> s_for; //buffer for lex (used for FOR cycle)
    stack<Lex> s_buffer; //buffer for other purposes
    Lex buffer; //buffer just for fun
    bool is_s_read = false; //read from stack
    bool is_q_read = false; //read from queue or from scanner
    bool is_b_read = false; //read from 1 level buffer
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
    void RV();
    void RV1();
    void RV2();
    void RV3();
    void RV4();
    void RV5();
    void RV6();
    ////void T();
    void F();
    void dec(type_of_lex type);
    void check_id();
    void check_break();
    void check_op();
    void check_not();
    void check_unar_minus();
    void eq_type();
    void eq_bool();
    void check_id_in_read();
    void sq_push(){
        q_buffer = sq_buffer.top();
        sq_buffer.pop();
    }
    // семантичиеские действия
    void gl()
    // получить очередную лексему
    {
        
        if(is_b_read){
            curr_lex = buffer;
            is_b_read = false;
        }else if(is_s_read && (is_s_read = !s_buffer.empty())){
            curr_lex = s_buffer.top();
            s_buffer.pop();
            is_s_read = false;
        }else if(is_q_read &&(is_q_read = !q_buffer.empty())){
            curr_lex = q_buffer.front();
            q_buffer.pop(); //! Danger
        }else{
            curr_lex = scan.get_lex();
        }
        c_type = curr_lex.get_type();
        if(curr_lex.get_value()!=NULL)
            c_val = ((int *)curr_lex.get_value())[0]; // could be some random int
        else
            c_val = 0;
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
    while(!s_goto_id.is_empty()){ //goto initialisation
        var_info var = s_goto_id.pop();
        Ident t = TID[((int*) var.initial_value)[0]];
        if(t.get_declare())
            prog.put_lex(Lex(   POLIZ_LABEL,   (void*) new int(     ((int*) t.get_value())[0]      )    ), var.index_in_TID);
        else
            throw "use of undeclared label";
    } 
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
        {is_q_read = false;
            TID[i].put_declare();
            TID[i].put_type(type);
            TID[i].put_value(info.initial_value); // if no initialization value is NULL
            if(!info.initial_value)
                TID[i].put_assign(); //assign if initialised
        }
    }
}
void Parser::check_break(){
    if(!s_break.empty())
        throw "break operator outside of cycle";
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
    if(t1==LEX_STRING) //string
        if(t2 == LEX_STRING && (op == LEX_PLUS || op == LEX_EQ || op == LEX_LSS ||
         op == LEX_GTR || op == LEX_LEQ || op == LEX_GEQ || op == LEX_NEQ))
            st_lex.push(LEX_STRING);
        else
            throw "wrong types are in operation";
    else { //int and boolean
        if (op == LEX_PLUS || op == LEX_MINUS || op == LEX_TIMES || op == LEX_SLASH)
            r = LEX_INT;
        if (op == LEX_OR || op == LEX_AND)
            t = LEX_BOOLEAN;
        if (t1 == t2 && t1 == t)
            st_lex.push(r);
        else
            throw "wrong types are in operation";
    }

    
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
void Parser::check_unar_minus()
{
    if (st_lex.pop() != LEX_INT)
        throw "wrong type is in unar minus";
    else
    {
        st_lex.push(LEX_INT);
    }
}
void Parser::eq_type()
{
    type_of_lex t;
    if ((t = st_lex.pop()) != st_lex.pop())
        throw "wrong types are in =";
    st_lex.push(t);
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
    check_break();
    if (c_type != LEX_RFP)
        throw curr_lex;
}
// TODO Descriptions
//?DONE
void Parser::D1()
{
    bool is_desc_higher_level = is_desc;
    do
    {
        D();
        if (is_desc)
            if (c_type == LEX_SEMICOLON)
                gl();
            else
                throw curr_lex;

    } while (is_desc);
    is_desc = is_desc_higher_level;
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




// TODO Operators parser
void Parser::O1()
{
    bool is_desc_higher_level = is_desc;
    do
    {
        O();
    } while (is_desc);
    is_desc = is_desc_higher_level;
}



// TODO Operator parser
//? DONE
void Parser::O()
{
    int pl0, pl1, pl2, pl3;
    if (c_type == LEX_IF) // IF operator
    {
        is_desc = true;
        gl();
        if (c_type != LEX_LP)
            throw curr_lex;
        gl();
        E();
        eq_bool();
        pl2 = prog.get_free();        // get begin IF operator
        prog.blank();                 // create blank space
        prog.put_lex(Lex(POLIZ_FGO));
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
        is_desc = true;
        pl0 = prog.get_free();
        gl();
        if(c_type != LEX_LP)
            throw curr_lex;
        gl();
        E();
        eq_bool();
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
            while(!s_break.empty()){ //set break operators
                prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(prog.get_free())), s_break.top());
                s_break.pop();
            }
            
        }
        else
            throw curr_lex;
    }
    else if (c_type == LEX_FOR) //! FUCK FOR CYCLES
    { // FOR operator
         //! for saving changing action
        is_desc = true;
        
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
        eq_bool();
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
        
        gl();
        if(!q_buffer.empty())
            throw curr_lex;
        while(c_type != LEX_RP){ // Save changing action for later insertion
            q_buffer.push(curr_lex); //Dangerous assign
            gl();
        }
        q_buffer.push(Lex(LEX_SEMICOLON,  (void *)new int(17))); // for correct assignment parser working (17 is num of semicolon in enum)

        sq_buffer.push(q_buffer); //Push to stack queue

        while(!q_buffer.empty()) //Make queue empty
            q_buffer.pop();


        pl1 = prog.get_free();
        prog.blank();
        prog.put_lex(Lex(POLIZ_FGO));
        if (c_type == LEX_RP)
        {
            gl();
            O();
            s_for.push(curr_lex); //save first after cycle lex
            sq_push();
            is_q_read = true;
            gl();
            E();
            if(c_type!= LEX_SEMICOLON)
                throw curr_lex;
            is_q_read = false; // for save test
            is_b_read = true;
            buffer = s_for.top();
            s_for.pop();
            gl();

            prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(pl0)));
            prog.put_lex(Lex(POLIZ_GO));
            prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(prog.get_free())), pl1);
            while(!s_break.empty()){ //set break operators
                prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(prog.get_free())), s_break.top());
                s_break.pop();
            }
        }
        else
            throw curr_lex;
    }
    else if (c_type == LEX_READ) // READ operator
    {
        is_desc = true;
        gl();
        if (c_type == LEX_LP)
        {
            gl();
            if (c_type == LEX_ID)
            {
                check_id_in_read();
                prog.put_lex(Lex(POLIZ_ADDRESS, (void *)new int(c_val)));
                gl();
            }
            else
                throw curr_lex;
            if (c_type == LEX_RP)
            {
                gl();
                prog.put_lex(Lex(LEX_READ));
            }
            else
                throw curr_lex;
        }
        else
            throw curr_lex;
        if(c_type!=LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }
    else if (c_type == LEX_WRITE) // WRITE operator
    {
        is_desc = true;
        gl();
        if (c_type == LEX_LP)
        {
            gl();
            E();
            while(c_type != LEX_RP && c_type != LEX_NULL/*if error*/){
                gl();
                E();
                prog.put_lex(Lex(LEX_WRITE));
            }
            if (c_type != LEX_RP)
            {
                throw curr_lex;
            }
            gl();
        }
        else
            throw curr_lex;
        if(c_type!=LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }
    else if (c_type == LEX_ID || c_type == LEX_MINUS) // expression operator or LABELED operator //?DONE
    {
        is_desc = true;
        buffer = curr_lex;
        gl();
        if(c_type == LEX_COLON){ // LABELED operator
            is_b_read = true;
            gl(); //read from buffer
            int idx = c_val;
            if (TID[idx].get_declare())
                throw "twice";
            else
            {
                TID[idx].put_declare();
                TID[idx].put_type(LEX_LINK);
                TID[idx].put_value((void*) new int(prog.get_free())); // if no initialization value is NULL //! Could be +-1 
            }
            gl();
            O();
            return;
        }
        //EXPRESSION operator //!!!! Pizdez
        s_buffer.push(curr_lex);
        is_b_read = true;
        gl();//read from buffer
        is_s_read = true;
        E();
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }
    else if (c_type == LEX_LFP)//? DONE
    { // COMBINED operator
        is_desc = true;
        if(c_type != LEX_LFP)
            throw curr_lex;
        gl();
        O1();
        if(c_type != LEX_RFP)
            throw curr_lex;
        gl();
    }
    else if (c_type == LEX_GOTO)
    { // GOTO operator
        is_desc = true;
        gl();
        if(c_type != LEX_ID)
            throw curr_lex;
        int idx = c_val;
        if(TID[idx].get_declare()){
            prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(((int *)TID[idx].get_value())[0]))); // duplicate because of after freeing
            prog.put_lex(Lex(POLIZ_GO));
        } else {
            var_info k = var_info(prog.get_free(), curr_lex.get_value());
            prog.blank();
            prog.put_lex(Lex(POLIZ_GO));
            s_goto_id.push(k);
        }
        gl();
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }
    else if (c_type == LEX_BREAK)
    { // BREAK operator
        is_desc = true;
        s_break.push(prog.get_free());
        prog.blank();
        prog.put_lex(Lex(POLIZ_GO));
        gl();
        if(c_type != LEX_SEMICOLON)
            throw curr_lex;
        gl();
    }
    else{
        is_desc = false;
    }
}
//? DONE?
void Parser::E()
{
    buffer = curr_lex;
    gl();
    is_b_read = true;
    if(c_type == LEX_ASSIGN){ //ID=E
        gl();//read from buffer
        check_id();
        prog.put_lex(Lex(POLIZ_ADDRESS, (void *)new int(c_val)));
        gl();
        E();
        eq_type();
        prog.put_lex(Lex(LEX_ASSIGN));
    } else { //RV
        if(!s_buffer.empty())
            throw "E: buffer is not empty";
        s_buffer.push(curr_lex); // save value to stack
        is_s_read = true;
        gl(); //read from buffer
        RV();
    }
    // while(c_type != LEX_SLASH){
    //     gl();
    // }
    // prog.put_lex(Lex(LEX_ID));
    // gl();
}
//TODO OR statement
//? DONE
void Parser::RV()
{
    RV1();
    int pl0, pl1;
    if(c_type == LEX_OR){ //RV1||RV
        st_lex.push(c_type);
        pl0 = prog.get_free();
        prog.blank();// adress of the second statement
        prog.put_lex(Lex(POLIZ_FGO)); //! ???
        prog.put_lex(Lex(LEX_TRUE, (void *)new int(1)));
        pl1 = prog.get_free();
        prog.blank();// adress of the end of OR statement
        prog.put_lex(Lex(POLIZ_GO));
        prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(prog.get_free())), pl0);
        gl();
        RV();
        check_op();
        prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(prog.get_free())), pl1);
    }
}

//TODO AND statement
//? DONE
void Parser::RV1()
{
    RV2();
    int pl0, pl1;
    if(c_type == LEX_AND){ //RV2&&RV1
        st_lex.push(c_type);
        pl0 = prog.get_free();       
        prog.blank();// adress of 0
        prog.put_lex(Lex(POLIZ_FGO)); //! ???
        gl();
        RV1();
        check_op();
        pl1 = prog.get_free();
        prog.blank();// adress of the end of AND statement
        prog.put_lex(Lex(POLIZ_GO));
        prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(prog.get_free())), pl0);
        prog.put_lex(Lex(LEX_FALSE, (void *)new int(0)));
        prog.put_lex(Lex(POLIZ_LABEL, (void *)new int(prog.get_free())), pl1);
    }
}
void Parser::RV2()
{
    RV3();
    if(c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR || 
        c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ){ //RV3<RV2 ...
        st_lex.push(c_type); //push for operand check
        type_of_lex b_type = c_type;
        gl();
        RV2();
        check_op();
        prog.put_lex(Lex(b_type));

    }
}
void Parser::RV3()
{
    RV4();
    if(c_type == LEX_PLUS || c_type == LEX_MINUS)
    {

        st_lex.push(c_type);
        type_of_lex b_type = c_type;
        gl();
        RV3();
        check_op();
        prog.put_lex(Lex(b_type));
    }
}
void Parser::RV4()
{
    RV5();
    if(c_type == LEX_TIMES || c_type == LEX_SLASH)
    {

        st_lex.push(c_type);
        type_of_lex b_type = c_type;
        gl();
        RV4();
        check_op();
        prog.put_lex(Lex(b_type));
    }
}
void Parser::RV5()
{
    if (c_type == LEX_NOT)
    {
        gl();
        RV6();
        prog.put_lex(Lex(LEX_NOT));
        check_not();
    }
    else if (c_type == LEX_MINUS)
    {
        gl();
        RV6();
        prog.put_lex(Lex(LEX_NUM, (void *)new int(-1)));
        prog.put_lex(Lex(LEX_TIMES));
        check_unar_minus();
        
    }
    else{
        RV6();
    }
}
void Parser::RV6()
{
    if (c_type == LEX_ID)
    {
        check_id();
        prog.put_lex(curr_lex);
        gl();
    }
    else if (c_type == LEX_NUM)
    {
        st_lex.push(LEX_INT);
        prog.put_lex(curr_lex);
        gl();
    }
    else if (c_type == LEX_STR)
    {
        st_lex.push(LEX_STRING);
        prog.put_lex(curr_lex);
        gl();
    }
    else if (c_type == LEX_TRUE)
    {
        st_lex.push(LEX_BOOLEAN);
        prog.put_lex(Lex(LEX_TRUE, (void *)new int(1)));
        gl();
    }
    else if (c_type == LEX_FALSE)
    {
        st_lex.push(LEX_BOOLEAN);
        prog.put_lex(Lex(LEX_FALSE, (void *)new int(0)));
        gl();
    }
    else if (c_type == LEX_LP)
    {
        gl();
        E();
        if (c_type == LEX_RP)
            gl();
        else
            throw curr_lex;
    }
    else
        throw curr_lex;
}

//     check_id();
//     prog.put_lex(Lex(POLIZ_ADDRESS, (void *)new int(c_val)));
//     gl();
//     if (c_type == LEX_ASSIGN)
//     {
//         gl();
//         E();
//         eq_type();
//         prog.put_lex(Lex(LEX_ASSIGN));
//         if (c_type != LEX_SEMICOLON)
//             throw curr_lex;
//         gl();
//     }
//     else
//         throw curr_lex;

//     E1();
//     if (c_type == LEX_EQ || c_type == LEX_LSS || c_type == LEX_GTR ||
//         c_type == LEX_LEQ || c_type == LEX_GEQ || c_type == LEX_NEQ)
//     {
//         st_lex.push(c_type);
//         gl();
//         E1();
//         check_op();
//     }
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
//         st_lex.push(LEX_BOOLEAN);
//         prog.put_lex(Lex(LEX_TRUE, 1));
//         gl();
//     }
//     else if (c_type == LEX_FALSE)
//     {
//         st_lex.push(LEX_BOOLEAN);
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


//TODO POLIZ Executer
class Executer
{
    Lex pc_el;
public:
    void execute(Poliz &prog);
};

void Executer::execute(Poliz &prog)
{
    stack<void*> args;
    int i, j, index = 0, size = prog.get_free(), *val, *val2;
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
            i = ((int*)pc_el.get_value())[0];
            if (TID[i].get_assign())
            {
                args.push(TID[i].get_value());
                break;
            }
            else
                throw "POLIZ: indefinite identifier";
        case LEX_NOT:
            val =(int*) args.top();
            args.pop();
            val[0] = !val[0];
            args.push(val);
            break;
        case POLIZ_GO:
            val = (int*) args.top();
            args.pop();
            index = val[0] - 1;
            free(val);
            break;
        case POLIZ_FGO:
            val = (int*) args.top();
            args.pop();
            if (!val[0])
                index = val[0] - 1;
            free(val);
            break;
        case LEX_WRITE:
            val = (int*) args.top();
            args.pop();
            cout << val[0] << endl;
            free(val);
            break;
        case LEX_READ:
        {
            val = (int*) args.top();
            args.pop();
            void * k;
            i = val[0];
            if (TID[i].get_type() == LEX_INT)
            {
                k = new int;
                cout << "Input int value for";
                cout << TID[i].get_name() << endl;
                cin >> *(int*)k;
            }
            else if(TID[i].get_type() == LEX_BOOLEAN)
            {
                char j[20];
                k = new int;
            rep:
                cout << "Input boolean value";
                cout << "(true or false) for";
                cout << TID[i].get_name() << endl;
                cin >> j;
                if (!strcmp(j, "true"))
                    *(int*)k = 1;
                else if (!strcmp(j, "false"))
                    *(int*)k = 0;
                else
                {
                    cout << "Error in input:true/false";
                    cout << endl;
                    goto rep;
                }
            } else {
                k = new string;
                cout << "Input string value";
                cout << TID[i].get_name() << endl;
                cin >> *(string*)k;
                
            }
            free(TID[i].get_value());
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




int main()
{
    // Scanner s= Scanner("test.txt");
    // Lex l = s.get_lex();
    // while(l.get_type() != LEX_NULL){
    //     //cout << l.get_type() << endl;
    //     l = s.get_lex();
    // }

    Parser p= Parser("test.txt");
    p.analyze();
    cout << endl;
    for (int j=1; j<100; j++){
        cout << TID[j].get_name()<< " " << TID[j].get_type() << " " << TID[j].get_value() << endl;
    }
}
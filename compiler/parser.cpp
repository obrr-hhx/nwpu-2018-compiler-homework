#include "parser.h"
#include "token.h"
#include "lexer.h"
#include "error.h"
#include "symbol.h"
#include "symtab.h"

//语法分析器
Parser::Parser(Lexer &lexer, SymTab &symtab)
    :lexer(lexer), symtab(symtab)
{}

void Parser::analyse(){
    move();
    program();
}

void Parser::move(){
    look = lexer.tokenize();
}

bool Parser::match(Tag need){
    if(look->tag == need){
        move();
        return true;
    }else{
        return false;
    }
}

#define SYNERROR(code,t) Error::synError(code, t)

//错误修复
#define _(T) ||look->tag == T
#define F(C) look->tag == C
/*
若终结符匹配失败，则检查当前词法记号是否是之后待匹配的非终结符下的词法记号
如果是的话，则代表是词法记号丢失，否则，是词法记号匹配错误
*/
void Parser::recovery(bool cond, SynError lost, SynError wrong){
    if(cond){// 在给定的follow集中
        SYNERROR(lost, look);
    }else{
        SYNERROR(wrong, look);
        move();
    }
}

//类型
#define TYPE_FIRST F(KW_INT)_(KW_CHAR)_(KW_VOID)
//表达式
#define EXPR_FIRST F(LPAREN)_(NUM)_(CH)_(STR)_(ID)_(NOT)_(SUB)_(LEA)_(MUL)_(INC)_(DEC)
//左值运算
#define LVAL_OPR F(ASSIGN)_(OR)_(AND)_(GT)_(GE)_(EQU)_(NEQU)_(ADD)_(SUB)_(MUL)_(DIV)_(MOD)_(INC)
//右值运算
#define RVAL_OPR F(OR)_(AND)_(GT)_(GE)_(LT)_(LE)_(EQU)_(NEQU)_(ADD)_(SUB)_(MUL)_(DIV)_(MOD)
//语句
#define STATEMENT_FIRST F(EXPR_FIRST)_(SEMICON)_(KW_WHILE)_(KW_FOR)_(KW_DO)_(KW_IF)_(KW_SWITCH)_(KW_RETURN)_(KW_BREAK)_(KW_CONTINUE)

// <program> -> <segment><program> | ε
void Parser::program(){
    if(F(END)){
        return ;
    }else{
        segment();
        program();
    }
}
// <segment> -> KW_EXTERN <type><def> | <type><def>
void Parser::segment(){
    bool ext = match(KW_EXTERN); // 记录声明属性
    Tag t = type();
    def(ext, t);
}
// <type> -> KW_INT | KW_VOID | KW_CHAR
Tag Parser::type(){
    Tag tmp = KW_INT;// 初始化为int
    if(TYPE_FIRST){
        tmp = look->tag;
        move();
    }else{// 报错
        recovery(F(ID)_(MUL), TYPE_LOST, TYPE_WRONG);
    }
    return tmp;
}
// <def> -> ID <idtail> | MUL ID <init><deflist>
void Parser::def(bool ext, Tag t){
    string name = "";
    if(match(MUL)){// 指针
        if(F(ID)){
            name = (((Id*)look)->name);
            move();
        }else{
            recovery(F(SEMICON)_(COMMA)_(ASSIGN),ID_LOST, ID_WRONG);
        }
        symtab.addVar(init(ext,t,true,name));
        deflist(ext,t);
    }else{
        if(F(ID)){
            name = ((Id*)look)->name;
            move();
        }else{
            recovery(F(SEMICON)_(COMMA)_(ASSIGN)_(LPAREN)_(LBRACK), ID_LOST, ID_WRONG);
        }
        idtail(ext, t, false, name);
    }
}
// <idtail> -> <varrdef><deflist> | LPAREN <para> RPAREN <funtail>
void Parser::idtail(bool ext, Tag t, bool ptr, string name){
    if(match(LPAREN)){// 函数
        symtab.enter();
        vector<Var*> paralist; // 创建参数列表
        para(paralist);
        if(!match(RPAREN)) recovery(F(LBRACE)_(SEMICON), RPAREN_LOST, RBRACE_WRONG);
        Fun* fun = new Fun(ext, t, name, paralist);// 新建这个函数
        funtail(fun);
        symtab.leave();
    }else{
        symtab.addVar(varrdef(ext,t,false,name));
        deflist(ext,t);
    }
}

// <para> -> <type><paradata><paralist> | ε
void Parser::para(vector<Var*> &list){
    if(match(RPAREN)) return;
    Tag t = type();
    Var* v = paradata(t);
    symtab.addVar(v);// 保存参数到符号表
    list.push_back(v); // 将参数存入参数列表中
    paralist(list);
}

// <paradata> -> MUL ID | ID <paradatatail>
Var* Parser::paradata(Tag t){
    string name = "";
    if(match(MUL)){// 和 * 匹配，说明是指针变量
        bool ext = false;
        bool ptr = true;
        if(F(ID)){
            name = ((Id*)look)->name;
            move();
        }else{
            recovery(F(COMMA)_(RPAREN), ID_LOST, ID_WRONG);
        }
        return new Var(symtab.getScopePath(), ext, t, ptr, name);
    }else if(F(ID)){// 普通变量或者是数组变量
        name = ((Id*)look)->name;
        move();
        return paradatatail(t, name);
    }else{// 没有和ID匹配上，报错，并且返回一个name为空的变量(毕竟总得返回一个变量对象)
        recovery(F(RPAREN)_(COMMA)_(LBRACK), ID_LOST, ID_WRONG);
        return new Var(symtab.getScopePath(), false, t, false, name);// 
    }
}
// <paradatatail> -> LBRACK NUM RBRACK | ε
Var* Parser::paradatatail(Tag t, string name){
    if(match(LBRACK)){// 匹配到数组左括号 [
        int len = 1;// 参数数组可以忽略长度
        if(F(NUM)){
            len = ((Num*)look)->value;
            move();
        }
        if(!match(RBRACK)){
            recovery(F(COMMA)_(RPAREN), RBRACK_LOST, RBRACK_WRONG);
        }
        return new Var(symtab.getScopePath(), false, t, name, len);
    }else{
        return new Var(symtab.getScopePath(), false, t, false, name);
    }
}
// <paralist> -> COMMA <type><paradata><paralist> | ε
void Parser::paralist(vector<Var*> &list){
    if(match(COMMA)){
        Tag t = type();
        Var* v = paradata(t);
        symtab.addVar(v);
        list.push_back(v);
        paralist(list);
    }
}
// <varrdef> ->  LBRACK NUM RBRACK | <init>
Var* Parser::varrdef(bool ext, Tag t, bool ptr, string name){
    if(match(LBRACK)){//数组
        int len = 0;
        if(F(NUM)){
            len = ((Num*)look)->value;
            move();
        }else{
            recovery(F(RBRACK), NUM_LOST, NUM_WRONG);
        }
        if(!match(RBRACK)) recovery(F(COMMA)_(SEMICON), RBRACK_LOST, RBRACK_WRONG);
        return new Var(symtab.getScopePath(), ext, t, name, len);
    }
    else
        return init(ext, t, ptr, name);
}
// <deflist> -> COMMA \<defdata> <deflist> | SEMICON
void Parser::deflist(bool ext, Tag t){
    if(match(COMMA)){
        symtab.addVar(defdata(ext,t));
        deflist(ext,t);
    }
    else if(match(SEMICON)) return;
    else{
        if(F(MUL)_(ID)){// 逗号出错
            recovery(true, COMMA_LOST, COMMA_WRONG);
            symtab.addVar(defdata(ext, t));
            deflist(ext, t);
        }else{// 分号出错
            recovery(TYPE_FIRST || STATEMENT_FIRST || F(KW_EXTERN)_(RBRACE), SEMICON_LOST, SEMICON_WRONG);
        }
    }
}

// <defdata> -> ID <varrdef> | MUL ID <init>
Var* Parser::defdata(bool ext, Tag t){
    string name = "";
    if(F(ID)){
        name = ((Id*)look)->name;
        move();
        return varrdef(ext,t,false,name);
    }else if(match(MUL)){
        if(F(ID)){
            name = ((Id*)look)->name;
            move();
            return varrdef(ext,t,true,name);
        }else{
            recovery(F(COMMA)_(ASSIGN)_(SEMICON), ID_LOST, ID_WRONG);
        }
        return init(ext,t,true,name);
    }else{
        recovery(F(COMMA)_(ASSIGN)_(SEMICON)_(LBRACK), ID_LOST, ID_WRONG);
        return varrdef(ext,t,false,name);
    }
}
// <init> -> ASSIGN <expr> | ε
Var* Parser::init(bool ext, Tag t, bool ptr, string name){
    Var* initVar = NULL;
    if(match(ASSIGN)){
        // initVar = expr();
    }
    return new Var(symtab.getScopePath(), ext, t, ptr, name, initVar);//新的变量或指针
}
// <funtail> -> SEMICON | <block>
void Parser::funtail(Fun* f){
    if(match(SEMICON)){// 函数声明
        symtab.decFun(f);
    }else{// 函数定义
        // symtab.defFun(f);
        block();
        // symtab.endDefFun();// 结束函数定义
    }
}
// <block> -> LBRACE <subprogram> RBRACE
void Parser::block(){
    if(!match(LBRACE)) recovery(TYPE_FIRST || STATEMENT_FIRST || F(RBRACE), LBRACE_LOST, LBRACE_WRONG);
    subprogram();
    if(!match(RBRACE)) recovery(TYPE_FIRST || STATEMENT_FIRST || F(KW_EXTERN)_(KW_ELSE)_(KW_CASE)_(KW_DEFAULT), RBRACE_LOST, RBRACE_WRONG);
}
// <subprogram> -> <localdef><subprogram> | <statement><subprogram> | ε
void Parser::subprogram(){
    if(TYPE_FIRST){// 局部变量
        localdef();
        subprogram();
    }else if(STATEMENT_FIRST){// 简简单单小语句
        statement();
        subprogram();
    }
}
// <localdef> -> <type><defdata><deflist>
void Parser::localdef(){
    Tag t = type();
    Var* v = defdata(false, t);
    symtab.addVar(v);
    deflist(false, t);
}

// <altexpr> -> <expr> | ε
Var* Parser::altexpr(){
    
}

// <expr> -> <assexpr>

// <assexpr> -> <orexpr><asstail>

// <asstail> -> ASSIGN <orexpr><asstail> | ε

// <orexpr> -> <andexpr><ortail>

// <ortail> -> OR <andexpr><ortail> | ε

// <andexpr> -> <cmpexpr><andtail>

// <andtail> -> AND <cmpexpr><andtail> | ε

// <cmpexpr> -> <aloexpr><cmptail>

// <cmptail> -> <cmps><aloexpr><cmptail> | ε

// <cmps> -> GT | GE | LT | LE | EQU | NEQU

// <aloexpr> -> <item><alotail>

// <alotail> -> <adds><item><alotail> | ε

// <adds> -> ADD | SUB

// <item> -> <factor><itemtail>

// <itemtail> -> <muls><factor><itemtail> | ε

// <muls> -> MUL | DIV | MOD

// <factor> -> <lop><factor> | <val>

// <lop> -> NOT | SUB | LEA | MUL | INCR | DECR

// <val> -> <elem><rop>

// <rop> -> INCR | DECR ｜ε

// <elem> -> ID <idexpr> | LPAREN <expr> RPAREN | <lieral>

// <idexpr> -> LBRACK <expr> RBRACK | LPAREN <realarg> RPAREN | ε

// <realarg> -> <arg><arglist> | ε

// <arglist> - > COMMA <arg><arglist> | ε

// <arg> -> <expr>

// <literal> -> NUM | CH | STR
Var* Parser::literal(){
    Var* v = NULL;
    if(F(NUM)_(CH)_(STR)){
        v = new Var(look);
        if(F(STR)){
            symtab.addStr(v);
        }else{
            symtab.addVar(v);
        }
        move();
    }else{
        recovery(RVAL_OPR, LITERAL_LOST, LITERAL_WRONG);
    }
    return v;
}
// // <statement> -> <altexpr> SEMICON | 
//                <whilestat> | 
//                <forstat> | 
//                <dowhilestat> | 
//                <ifstat> | 
//                <switchstat> | 
//                KW_BREAK SEMICON | 
//                KW_CONTINUE SEMICON | 
//                KW_RETURN <altexpr> SEMICON
void Parser::statement(){
    
}

// <whilestat> -> KW_WHILE LPAREN <altexpr> RPAREN <block>
void Parser::whilestat(){
    symtab.enter();
    match(KW_WHILE);
    if(!match(LPAREN)) recovery(EXPR_FIRST || F(RPAREN), LPAREN_LOST, LPAREN_WRONG);
    altexpr();
    if(!match(RPAREN)) recovery(F(LBRACE), RPAREN_LOST, RPAREN_WRONG);
    block();
    symtab.leave();
}
// <forstat> -> KW_FOR LPAREN <forinit><altexpr> SEMICON <altexpr> RPAREN <block>
// <forinit> -> <localdef> | <altexpr> SEMICON

// <ifstat> -> KW_IF LPAREN <expr> RPAREN <block><elsestat>
// <elsestat> -> KW_ELSE <block> | ε

// <switchstat> -> KW_SWITCH LPAREN <expr> RPAREN LBRACE <casestat> RBRACE
// <casestat> -> KW_CASE <caselabel> COLON <subprogram><casestat> | KW_DEFAULT COLON <subprogram>
// <caselable> -> <literal>

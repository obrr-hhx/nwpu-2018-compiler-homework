#include "parser.h"
#include "token.h"
#include "lexer.h"
#include "error.h"
#include "symbol.h"
#include "symtab.h"
#include "genir.h"
#include "intercode.h"

//语法分析器
Parser::Parser(Lexer &lexer, SymTab &symtab, GenIR &ir)
    :lexer(lexer), symtab(symtab), ir(ir)
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
        symtab.defFun(f);
        block();
        symtab.endDefFun();// 结束函数定义
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
    if(EXPR_FIRST) return expr();
    else return Var::getVoid();
}

// <expr> -> <assexpr>
Var* Parser::expr(){
    return assexpr();
}

// <assexpr> -> <orexpr><asstail>
Var* Parser::assexpr(){
    Var* lvar = orexpr();
    return asstail(lvar);
}
// <asstail> -> ASSIGN <orexpr><asstail> | ε
Var* Parser::asstail(Var* lvar){
    if(match(ASSIGN)){

    }
}

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
    InterInst* _while, *_exit; // 标签
    ir.genWhileHead(_while, _exit); // while循环头部
    match(KW_WHILE);
    if(!match(LPAREN)) recovery(EXPR_FIRST || F(RPAREN), LPAREN_LOST, LPAREN_WRONG);
    Var* cond = altexpr();
    ir.genWhileCond(cond, _exit); // while条件
    if(!match(RPAREN)) recovery(F(LBRACE), RPAREN_LOST, RPAREN_WRONG);
    block();
    ir.genWhileTail(_while, _exit); // while尾部
    symtab.leave();
}
// <forstat> -> KW_FOR LPAREN <forinit><altexpr> SEMICON <altexpr> RPAREN <block>
// <forinit> -> <localdef> | <altexpr> SEMICON

// <dowhile> -> KW_DO <block> KW_WHILE LPAREN <altexpr> RPAREN SEMICON
void Parser::dowhilestat(){
    symtab.enter();
    InterInst *_do, *_exit;
    ir.genDoWhileHead(_do, _exit);
    match(KW_DO);
    block();
    if(!match(KW_WHILE)) recovery(F(LPAREN), WHILE_LOST, WHILE_WRONG);
    if(!match(LPAREN)) recovery(EXPR_FIRST||F(RPAREN), LPAREN_LOST, LPAREN_WRONG);
    symtab.leave();
    Var* cond = altexpr();
    if(!match(RPAREN)) recovery(F(SEMICON), RPAREN_LOST, RPAREN_WRONG);
    if(!match(SEMICON)) recovery(TYPE_FIRST||STATEMENT_FIRST||F(RBRACE), SEMICON_LOST, SEMICON_WRONG);
    ir.genDoWhileTail(cond, _do, _exit);
}

// <ifstat> -> KW_IF LPAREN <expr> RPAREN <block><elsestat>
void Parser::ifstat(){
    symtab.enter();
    InterInst *_else, *_exit; // 标签
    match(KW_IF);
    if(!match(LPAREN))
        recovery(EXPR_FIRST, LPAREN_LOST, LPAREN_WRONG);
    Var *cond = expr();
    ir.genIfHead(cond, _else); // 生成if头部
    if(!match(RPAREN))
        recovery(F(LBRACE), RPAREN_LOST, RPAREN_WRONG);
    block();
    symtab.leave();
    if(F(KW_ELSE)){
        ir.genElseHead(_else,_exit); // 生成else头部
        elsestat();
        ir.genElseTail(_exit); // 生成else尾部
    }else{
        // 无else
        ir.genIfTail(_else); // 生成if尾部
    }
}

// <elsestat> -> KW_ELSE <block> | ε
void Parser::elsestat(){
    match(KW_ELSE);
    symtab.enter();
    if(F(LBRACE)) block();
    else statement();
    symtab.leave();
}

// <switchstat> -> KW_SWITCH LPAREN <expr> RPAREN LBRACE <casestat> RBRACE
void Parser::swicthstat(){
    symtab.enter();
    InterInst *_exit;
    ir.genSwitchHead(_exit);
    match(KW_SWITCH);
    if(!match(LPAREN)) recovery(EXPR_FIRST, LPAREN_LOST, LPAREN_WRONG);
    Var *cond = expr();
    if(cond->isRef()) cond = ir.genAssign(cond);
    if(!match(RPAREN)) recovery(F(LBRACE), RPAREN_LOST, RPAREN_WRONG);
    if(!match(LBRACE)) recovery(F(KW_CASE)_(KW_DEFAULT), LBRACE_LOST, LBRACE_WRONG);
    casestat(cond);
    if(!match(RBRACE)) recovery(TYPE_FIRST || STATEMENT_FIRST, RBRACE_LOST, RBRACE_WRONG);
    ir.genSwitchTail(_exit);
    symtab.leave();
}

// <casestat> -> KW_CASE <caselabel> COLON <subprogram><casestat> | KW_DEFAULT COLON <subprogram>
void Parser::casestat(Var *cond){
    if(match(KW_CASE)){
        InterInst *_case_exit;
        Var *lb = caselabel();
        ir.genCaseHead(cond, lb, _case_exit);
        if(match(COLON)) recovery(TYPE_FIRST||STATEMENT_FIRST, COLON_LOST, COLON_WRONG);
        symtab.enter();
        subprogram();
        symtab.leave();
        ir.genCaseTail(_case_exit);
        casestat(cond);
    }else if(match(KW_DEFAULT)){
        if(!match(COLON)) recovery(TYPE_FIRST||STATEMENT_FIRST, COLON_LOST, COLON_WRONG);
        symtab.enter();
        subprogram();
        symtab.leave();
    }
}

// <caselable> -> <literal>

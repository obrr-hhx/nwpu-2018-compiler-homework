#pragma once
#include "common.h"

//语法分析器

class Parser
{
    //文法开始
    void program();
    void segment();
    Tag type();

    //变量定义结构
    void def(bool ext, Tag t);
    void idtail(bool ext, Tag t, bool ptr, string name);
    Var* defdata(bool ext, Tag t);
    void deflist(bool ext, Tag t);
    Var* varrdef(bool ext, Tag t, bool ptr, string name);
    Var* init(bool ext, Tag t, bool ptr, string name);

    //函数
    void para(vector<Var*> &list);
    Var* paradata(Tag t);
    void paralist(vector<Var*> &list);
    Var* paradatatail(Tag t, string name);
    void block();
    void subprogram();
    void localdef();
    void funtail(Fun* f);

    //语句
    void statement();
    void whilestat();
    void dowhilestat();
    void forstat();
    void forinit();
    void ifstat();
    void elsestat();
    void swicthstat();
    void casestat(Var* cond);
    Var* caselabel();

    //表达式
    Var* altexpr();
    Var* expr();
    Var* assexpr();
    Var* asstail(Var* lval);
    Var* orexpr();
    Var* ortail(Var* lval);
    Var* andexper();
    Var* andtail(Var* lval);
    Var* cmpexpr();
    Var* cmptail(Var* lval);
    Tag cmps();
    Var* aloexpr();
    Var* alotail(Var* lval);
    Tag adds();
    Var* item();
	Var* itemtail(Var*lval);
	Tag muls();
	Var* factor();
	Tag lop();
	Var* val();
	Tag rop();
	Var* elem();
	Var* literal();
	Var* idexpr(string name);
	void realarg(vector<Var*> &args);
	void arglist(vector<Var*> &args);
	Var* arg();

    Lexer &lexer;//词法分析器
    Token* look;//查看超前的字母

    //语法分析与错误修复
    void move();//向前移一位
    bool match(Tag t);//匹配成功则向前移一位
    void recovery(bool cond, SynError lost, SynError wrong);//错误修复

public:
    Parser(Lexer &lexer);

    void analyse();// 语法分析
}
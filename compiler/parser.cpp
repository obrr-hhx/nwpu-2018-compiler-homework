#include "parser.h"
#include "token.h"
#include "lexer.h"
#include "error.h"
#include "symbol"

//语法分析器
Parser::Parser(Lexer &lexer)
    :lexer(lexer)
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
#define STATEMENT_FIRST (EXPR_FIRST)_(SEMICON)_(KW_WHILE)_(KW_FOR)_(KW_DO)_(KW_IF)_(KW_SWITCH)_(KW_RETURN)_(KW_BREAK)_(KW_CONTINUE)

// <program> -> <segment><program> | ε
void Parser::program(){
    if(F(END)){
        return ;
    }else{
        segment();
        program();
    }
}
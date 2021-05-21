#pragma once
#include<stdio.h>
#include <vector>
#include <list>
#include <string>
#include<iostream>
using namespace std;
/*
类型声明
*/

// 词法记号声明
enum Tag
{
	ERR,																					//错误，异常
	END,																					//文件结束标记
	ID,																						//标识符
	KW_INT,KW_CHAR,KW_VOID,												//数据类型
	KW_EXTERN,																		//extern
	NUM,CH,STR,																		//字面量
	NOT,LEA,																			//单目运算 ! - & *
	ADD,SUB,MUL,DIV,MOD,													//算术运算符
	INC,DEC,
	GT,GE,LT,LE,EQU,NEQU,													//比较运算符
	AND,OR,																				//逻辑运算
	LPAREN,RPAREN,																//()
	LBRACK,RBRACK,																//[]
	LBRACE,RBRACE,																//{}
	COMMA,COLON,SEMICON,													//逗号,冒号,分号
	ASSIGN,																				//赋值
	KW_IF,KW_ELSE,																//if-else
	KW_SWITCH,KW_CASE,KW_DEFAULT,									//swicth-case-deault
	KW_WHILE,KW_DO,KW_FOR,												//循环
	KW_BREAK,KW_CONTINUE,KW_RETURN								//break,continue,return
};

// 词法错误码
enum LexError
{
	STR_NO_R_QUTION,		//字符串没有右引号
	NUM_BIN_TYPE,				//2进制数没有实体数据
	NUM_HEX_TYPE,				//16进制数没有实体数据
	CHAR_NO_R_QUTION,		//字符没有右引号
	CHAR_NO_DATA,				//字符没有数据
	OR_NO_PAIR,					//||只有一个|
	COMMENT_NO_END,			//多行注释没有正常结束
	TOKEN_NO_EXIST			//不存在的词法记号
};

extern const char * tokenName[];

class Token;
class Scanner;
class Error;
class Lexer;
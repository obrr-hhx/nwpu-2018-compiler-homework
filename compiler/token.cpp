#include"token.h"
#include<sstream>
#include"error.h"

/**********************************************
 *                 词法记号名称
***********************************************/
const char * tokenName[]=
{
	"error",//错误，异常，结束标记等
	"end_of_file",//文件结束
	"ident",//标识符
	"int","char","void",//数据类型
	"extern",//extern
	"number","character","string",//字面量
	"!","&",//单目运算 ! - & *
	"+","-","*","/","%",//算术运算符
	"++","--",
	">",">=","<","<=","==","!=",//比较运算符
	"&&","||",//逻辑运算
	"(",")",//()
	"[","]",//[]
	"{","}",//{}
	",",":",";",//逗号,冒号,分号
	"=",//赋值
	"if","else",//if-else
	"switch","case","default",//swicth-case-deault
	"while","do","for",//循环
	"break","continue","return"//break,continue,return
};

// 词法记号
Token::Token(Tag t):tag(t)
{}

string Token::tostring(){
    return tokenName[tag];
};

Token::~Token()
{}

// 标识符
Id::Id(string n):Token(ID),name(n)
{}

string Id::tostring(){
    return string("[")+Token::tostring()+"]"+name;
};

// 数字
Num::Num(int val):Token(NUM),value(val)
{}

string Num::tostring(){
	stringstream ss;
	ss<<value;
	return string("[")+Token::tostring()+"]:"+ss.str();
}

// 字符串
Str::Str(string str):Token(STR),str(str)
{}

string Str::tostring(){
	return string("[")+Token::tostring()+"]:"+str;
}

// 字符
Chara::Chara(char c):Token(CH),ch(c){}

string Chara::tostring(){
	stringstream ss;
	ss<<ch;
	return string("[")+Token::tostring()+"]:"+ss.str();
}
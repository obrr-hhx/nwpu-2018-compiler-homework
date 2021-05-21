#pragma once

#include"common.h"

/********************************************************
 *                     词法记号
 * ****************************************************/
class Token{
    public:
        Tag tag;
        Token(Tag t);
        virtual string tostring();
        virtual ~Token();
};

// 标识符
class Id:public Token{
    public:
        string name;
        Id(string n);
        virtual string tostring();
};

// 字符串
class Str:public Token{
    public:
        string str;
        Str(string str);
        virtual string tostring();
};

// 数字
class Num:public Token{
    public:
        int value;
        Num(int val);
        virtual string tostring();
};

// 字符
class Chara:public Token{
    public:
        char ch;
        Chara(char c);
        virtual string tostring();
};
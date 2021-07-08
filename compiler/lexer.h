#pragma once
#include "common.h"
#include <unordered_map>

/********************************************************
 *                   扫描器
 * ******************************************************/

class Scanner{
    const char * fileName;
    FILE * file;

    static const int BUFFLEN = 80;// 缓冲长度
    char line[BUFFLEN];
    int lineLen;// 当前行长度
    int charAtLine;// 字符在当前行的位置
    char oldCh;

    int lineNum;// 行号

    public:
        Scanner(const char* name);
        ~Scanner();

        int scan();// 扫描
        void showChar(char ch);

        FILE* getFile();// 得到文件
        const char* getFileName();// 得到文件名
        int getLineNum();// 得到行号


};

/*********************************************
 *              关键字表
 * ******************************************/

class Keywords{
    unordered_map<string,Tag> keywords;
    public:
        Keywords();
        Tag getTag(string name);// 检查是否为保留字
};

/*********************************************
 *             词法分析器
 * *****************************************/

class Lexer{
    static Keywords keywords;// 关键字列表

    Scanner &scanner;// 扫描器
    char ch;// 读取的字符
    bool scan(char need = 0);// 匹配扫描
    Token* token;

public:
    Lexer(Scanner &sc);// 词法分析器构造
    ~Lexer();
    Token* tokenize();// 有限自动机匹配
    static string varietyTable[48];
};

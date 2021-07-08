#pragma once
#include "common.h"

// 编译器参数
class Args
{
public:
    static bool showChar;// 显示字符
    static bool showToken; // 显示词法记号
    static bool showSymTab; // 显示符号表
    static bool showIr; // 显示中间代码
    static bool showOpIr; // 显示优化后的中间代码
    static bool genArm; // 显示arm汇编
    static bool opt; // 是否执行优化
    static bool showHelp; // 显示帮助信息
    static bool showVersion; // 显示版本信息
    static bool removeFile; // 删除文件
};

// 编译器类
class Compiler
{
public:
    void compile(char* file);
};
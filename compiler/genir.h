#pragma once
#include "common.h"

// 中间代码生成器
class GenIR{
    static int labelNum;// 标签号码，用来产生唯一的标签
    SymTab &symtab;// 符号表
public:
    GenIR(SymTab& tab);
    ~GenIR();
    static string genLb();// 产生唯一的标签 
    static bool typeCheck(Var* v1, Var* v2);// 检查类型是否可以转换
};
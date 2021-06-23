#pragma once
#include "common.h"
#include <unordered_map>

//符号表
class SymTab{
    vector<string> varlist; // 记录变量添加的顺序
    vector<string> funlist; // 记录函数添加的顺序

    unordered_map<string, vector<Var*> *> varTab; // 变量表
    unordered_map<string, Var*> strTab; // 字符串常量表
    unordered_map<string, Fun*> funTab; // 函数表

    Fun* curFun; // 当前分析的函数
    int scopeId; // 作用域编号
    vector<int> scopePath; // 记录作用域的路径

    // 中间代码生成器
    GenIR *ir;

public:
    SymTab();
    ~SymTab();

    // 特殊变量
    static Var* voidVar;
    static Var* zero;
    static Var* one;
    static Var* four;

    //符号表作用域管理
    void enter(); // 进入局部作用域
    void leave(); // 离开局部作用域

    //变量管理
    void addVar(Var* v); // 添加一个变量
    void addStr(Var* v); // 添加一个字符串常量
    Var* getVar(string name); // 获取一个变量
    vector<Var*> getGlbVars(); // 获取所有全局变量

    //函数管理
    void decFun(Fun* f); // 声明函数
    void defFun(Fun* f); // 定义函数
    void endDefFun(); // 结束定义函数
    Fun* getFun(string name, vector<Var*>& args); // 获取函数
    Fun* getCurFun(); // 获取当前函数
    void addInst(InterInst* inst); // 添加一条中间代码

    void setIR(GenIR*ir); // 设置中间代码生成器  

    // 输出信息
    void toString();
    void printInterCode(); // 输出中间代码

    vector<int>& getScopePath();//获取scopePath
};
#pragma once
#include"common.h"

// 四元式类，定义了中间代码的格式
class InterInst{
    string label; // 标签, 空为正常指令
    Operator op; // 操作符
    Var *result; // 结果
    Var* arg1; // 参数1
    Var* arg2; // 参数2
    Fun* fun; // 函数
    InterInst *target; // 跳转目标

    bool first; // 是否是首指令

    void init();

public:
    //构造
	InterInst (Operator op,Var *rs,Var *arg1,Var *arg2=NULL);//一般运算指令
	InterInst (Operator op,Fun *fun,Var *rs=NULL);//函数调用指令,ENTRY,EXIT
	InterInst (Operator op,Var *arg1=NULL);//参数进栈指令,NOP
	InterInst ();//产生唯一标号
	InterInst (Operator op,InterInst *tar,Var *arg1=NULL,Var *arg2=NULL);//条件跳转指令,return

    void toString();
};

// 中间代码
class InterCode{
    vector<InterInst*> code; // 中间代码
public:
    ~InterCode(); // 销毁中间代码

    void addInst(InterInst * inst); // 添加中间代码

    void toString(); // 输出中间代码
    vector<InterInst*>& getCode(); // 获取中间代码序列
};
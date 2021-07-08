#pragma once
#include "common.h"

/*
    MIPS汇编
*/

struct MIPS{
    bool dead;
    string opcode;
    string result;
    string reg1;
    string reg2;
    string addition;

    MIPS(string op, string rs="", string a1="",string a2="",string add="");
    void replace(string op, string rs="", string a1="",string a2="",string add="");
    void setDead();
    string outPut();
};

class InstMips{
    list<MIPS*> code;
    string toStr(int num); // 整数变成字符串

    void lw_imm(string rsReg, int num);
    void lw_lb(string rsReg, string name);
    void leaStack(string rsReg, int off); // 加载栈内变量地址
public:
    // 基址存址 lw t0, 100(fp)
    void lw_base(string rsReg, string base, int disp, bool isChar);
    // 基址寻址 sw t0, 100(fp)
    void sw_base(string srcReg, string base, int disp, string tmpReg, bool isChar);

    /*
        label
        comment
        common instruction
    */
    void label(string name);
    void comment(string comment);
    void inst(string op, string rs); // 0 operand
    void inst(string op, string rs, string reg1); // 1 oprand
    void inst(string op, string rs, string reg1, string reg2); // 2 oprands

    // variables operation
    void init_var(Var* var, string initReg, string tmpReg); // initialize the variable
    void lw_var(string rsReg,Var* var); // load variable to register
    void lea_var(string rsReg, Var* var); // load address of variable to register
    void str_var(string srcReg, Var* var, string addrReg); // store register to variable

    // call procedure
    void call_fun(Fun* fun, string tmpReg);

    // stack operation
    void allocStack(Fun*fun, string tmpReg);// allocate stack
    void lw_args(Fun* fun); // load parameters of function to register

    // logic mathematical operation
    void logic_and(string rsReg, string reg1, string reg2);// and
    void logic_or(string rsReg, string reg1, string reg2);// or
    void logic_not(string rsReg, string reg1); // not
    // void cmp(string cmp, string cmpnot, string rsReg, string reg1, string reg2);
    void cmp(string op, string rsReg, string reg1, string reg2);// compare

    // nop
    void nop();
    void syscall();

    void outPut(FILE*file); // ouput assembly code
    list<MIPS*>& getCode();
};
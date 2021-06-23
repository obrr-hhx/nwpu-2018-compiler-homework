#pragma once
#include "common.h"

// 中间代码生成器
class GenIR{
    static int labelNum;// 标签号码，用来产生唯一的标签
    SymTab &symtab;// 符号表

    //break continue辅助标签列表
	vector< InterInst* > heads;
	vector< InterInst* > tails;
	void push(InterInst*head,InterInst*tail);//添加一个作用域
	void pop();//删除一个作用域

    // 传递参数
    void genPara(Var* val);

    //双目运算
	Var* genAssign(Var*lval,Var*rval);//赋值语句
	
	Var* genOr(Var*lval,Var*rval);//或运算语句
	Var* genAnd(Var*lval,Var*rval);//与运算语句
	Var* genGt(Var*lval,Var*rval);//大于语句
	Var* genGe(Var*lval,Var*rval);//大于等于语句
	Var* genLt(Var*lval,Var*rval);//小于语句
	Var* genLe(Var*lval,Var*rval);//小于等于语句
	Var* genEqu(Var*lval,Var*rval);//等于语句
	Var* genNequ(Var*lval,Var*rval);//不等于语句
	Var* genAdd(Var*lval,Var*rval);//加法语句
	Var* genSub(Var*lval,Var*rval);//减法语句
	Var* genMul(Var*lval,Var*rval);//乘法语句
	Var* genDiv(Var*lval,Var*rval);//除法语句
	Var* genMod(Var*lval,Var*rval);//取模
	
	//单目运算
	Var* genNot(Var*val);//非
	Var* genMinus(Var*val);//负
	Var* genIncL(Var*val);//左自加语句
	Var* genDecL(Var*val);//右自减语句
	Var* genLea(Var*val);//取址语句
	Var* genPtr(Var*val);//指针取值语句	
	Var* genIncR(Var*val);//右自加语句
	Var* genDecR(Var*val);//右自减

public:
    GenIR(SymTab& tab);

    Var* genAssign(Var* val); // 变量拷贝赋值，指针左值引用和变量赋值

    // 产生符号和语句
    Var* genArray(Var* array, Var* index); // 产生数组语句
    Var* genOneOpRight(Var* var, Tag opt); // 右单目运算语句
    Var* genOneOpLeft(Tag opt, Var* val); // 左单目运算
    Var* genTwoOp(Var* lvar, Tag opt, Var* rvar); // 双目运算语句
    Var* genCall(Fun* fun, vector<Var*> &args); // 函数调用

    // 产生符合语句
    void genIfHead(Var* cond, InterInst* &_else);
    void genIfTail(InterInst* &_else);
    void genElseHead(InterInst* _else, InterInst* &_exit);
    void genElseTail(InterInst*& _exit);
    void genSwitchHead(InterInst*& _exit);
    void genSwitchTail(InterInst* _exit);
    void genCaseHead(Var* cond, Var* lb, InterInst*& _case_exit);
    void genCaseTail(InterInst* _case_exit);
    void genWhileHead(InterInst * &_while, InterInst* &_exit);
    void genWhileCond(Var* cond, InterInst* _exit);
    void genWhileTail(InterInst* &_while, InterInst* &_exit);
    void genDoWhileHead(InterInst* &_do, InterInst* &_exit);
    void genDoWhileTail(Var* cond, InterInst* _do, InterInst* _exit);
    void genForHead(InterInst* &_for, InterInst* &_exit);
    void genForCondBegin(Var* cond, InterInst* &_step, InterInst* &_block, InterInst* _exit);
    void genForCondEnd(InterInst* _for, InterInst* _block);
    void genForTail(InterInst* &_step, InterInst* &_exit);

    // 产生特殊语句
    void genBreak();
    void genContiue();
    void genReturn(Var* ret); // 产生返回语句
    bool genVarInit(Var* var); // 产生变量初始化语句
    void genFunHead(Fun* fun); // 产生函数入口代码
    void genFunTail(Fun* fun); // 产生函数出口代码

    static string genLb();// 产生唯一的标签 
    static bool typeCheck(Var* v1, Var* v2);// 检查类型是否可以转换
};
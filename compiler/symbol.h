#pragma once

#include "common.h"
#include "intercode.h"

//变量结构体

class Var{
    bool literal; // 是否是常量
    vector<int> scopePath; // 作用域路径
    bool externed; // 是否是extern声明
    Tag type; // 变量类型
    string name; // 变量名
    bool isPtr; // is pointer?
    bool isArr; // is array?
    int arrayLen; // array's length, if this var isn't array, the lentgh is 0
    bool isLeft; // 是否是左值
    Var* initData; // 初始数值
    bool isInit; // 是否初始化
    union{
        int intVal; // int初值
        char charVal; // char初值
    };
    string strVal; // 字符串常量初值
    string ptrVal; // 字符指针初值
    Var* ptr; // 变量的指针变量
    int size; // 变量的大小
    int offset; // 变量的栈帧偏移

    //内部使用函数
    void setExtern(bool ext);//设置extern
    void setType(Tag t);//设置类型
    void setPtr(bool ptr);//设置指针
    void setName(string n);//设置名字
    void setArray(int len);//设定数组
    void clear();//清除关键字段信息

public:
    Var(vector<int>&sp,bool ext,Tag t,bool ptr,string name,Var*init=NULL);//变量
    Var(vector<int>&sp,bool ext,Tag t,string name,int len);// 数组
    Var(Token*lt);//设定常量
	Var(int val);//整数变量
	Var(vector<int>&sp,Tag t,bool ptr);//临时变量
	Var(vector<int>&sp,Var*v);//拷贝变量
	Var();//void变量

    static Var* getVoid();// 获取void变量
    static Var* getStep(Var* v);// 获取步长变量
    static Var* getTrue();// 获取true变量

    bool setInit(); // 设置初始化
    string getName();
    vector<int> &getPath();//获取scopePath
    int getSize();// 获取变量大小
    void setOffset(int off);
    string getStrVal();
    void setLeft(bool lf);//设置变量的左值属性
    Tag getType();
    Var* getInitData();
    bool getLeft();// 获取变量左值属性
    void setPointer(Var* var);// 设置指针变量
    Var* getPointer();// 获取指向变量的指针
    bool getArray(); // 获取是否为数组
    bool getPtr(); // 获取是否为指针

    void toString(); // 输出变量信息
    void value(); // 输出变量的中间代码形式

    bool isBase();// 是否为基本类型
    bool isRef(); // 是否为引用类型
    bool isVoid();// 是否为void类型变量
    bool isLiteral(); // 是否为基本类型常量
};

//函数结构
class Fun{
    //函数信息
    bool externed;
    Tag type;
    string name;
    vector<Var*> paraVar; // 参数形参列表

    //临时变量地址分配
    int maxDepth; // 栈的最大深度
    int curEsp; // 当前栈指针位置
    bool relocated; // 栈帧重定位标记

    //作用域管理
    vector<int> scopeEsp; // 作用域栈指针位置

    InterCode interCode; // 目标代码
    InterInst* returnPoint; // 返回点

public:
    //构造函数与析构函数
	Fun (bool ext,Tag t,string n,vector<Var*>&paraList);
	~Fun();

    //声明定义与使用
    bool match(Fun*f);//声明定义匹配
    bool match(vector<Var*>&args);//行参实参匹配
    void define(Fun*def);//将函数声明转换为定义，需要拷贝参数列表，设定extern

    // 作用域管理
    void enterScope();
    void leaveScope();
    void locate(Var* var);// 定位局部变量栈帧偏移

    // 中间代码
    void addInst(InterInst* inst); // 添加一条中间代码
    void setReturnPoint(InterInst* inst);
    InterInst* getReturnPoint();

    //外部调用接口
	bool getExtern();//获取extern
	void setExtern(bool ext);//设置extern
	Tag getType();//获取函数类型
	string& getName();//获取名字
	// bool isRelocated();//栈帧重定位了？
	vector<Var*>& getParaVar();//获取参数列表，用于为参数生成加载代码
	void toString();//输出信息
	void printInterCode();//输出中间代码
	void printOptCode();//输出优化后的中间代码
	void genAsm(FILE*file);//输出汇编代码
};
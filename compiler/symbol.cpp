#include "symbol.h"
#include "error.h"
#include "token.h"
#include "symtab.h"
#include "genir.h"

#define SEMERROR(code, name) Error::semError(code,name)

// 常量对象
Var::Var(Token*lt){
    clear();
    literal = true; // 常量标记
    setLeft(false); // 设置为不能为左值
    switch (lt->tag)
    {
    case NUM:
        setType(KW_INT);
        name="<int>"; // 类型作为其名字
        intVal = ((Num*)lt)->value; // 变量的值
        break;
    case CH:
        setType(KW_CHAR);
        name = "<char>";
        intVal = 0;
        charVal = ((Chara*)lt)->ch;
        break;
    case STR:
        setType(KW_CHAR);
        // name = Gencode::genLb(); //唯一生成一个新的名字
        strVal = ((Str*)lt)->str;
        setArray(strVal.size()+1); // 字符串作为字符数组储存
    }
}

// 变量, 指针
Var::Var(vector<int>&sp,bool ext,Tag t,bool ptr,string name,Var*init){
    clear();
    scopePath = sp;
    setExtern(ext);
    setType(t);
    setPtr(ptr);
    setName(name);
    initData = init;
}

// 数组
Var::Var(vector<int>&sp,bool ext,Tag t,string name,int len){
    clear();
    setName(name);
    scopePath = sp;
    setExtern(ext);
    setType(t);
    setArray(len);
}

// 整数变量
Var::Var(int val){
    clear();
    setName("<int>");// 特殊变量名字
    literal = true;
    setLeft(false);
    setType(KW_INT);
    intVal = val;
}

// void变量
Var::Var(){
    clear();
    setName("<void>");
    setLeft(false);
    intVal = 0;
    literal = 0;
    type = KW_VOID;
    isPtr = true;
}

// 清除信息，重新初始化
void Var::clear(){
    scopePath.push_back(-1);// 默认全局作用域
	externed=false;
	isPtr=false;
	isArr=false;
	isLeft=true;//默认变量是可以作为左值的
    isInit=false;
	literal=false;
	size=0;
	offset=0;
	ptr=NULL;//没有指向当前变量的指针变量
	// index=-1;//无效索引
	initData=NULL;
}

// 输出信息
void Var::toString(){
    if(externed) printf("extern ");
    // 输出类型
    printf("%s", tokenName[type]);
    // 输出指针
    if(isPtr) printf("*");
    // 输出变量名
    printf(" %s", name.c_str());
    // 输出数组
    if(isArr) printf("[%d]", arrayLen);
    // 输出初始值
    if(isInit){
        printf(" = ");
        switch (type)
        {
        case KW_INT:
            printf("%d", intVal);
            break;
        case KW_CHAR:
            if(isPtr){// string
                printf("\"%s\"", ptrVal.c_str());
            }else{
                printf("%c", charVal);
            } 
            break;
        }
    }
    printf("; size=%d, scope=\\", size);
    for(int i=0; i < scopePath.size(); i++){
        printf("%d\\",scopePath[i]);
    }
}

void Var::setExtern(bool ext){
    externed = ext;
}

void Var::setType(Tag t){
    type = t;
}

void Var::setLeft(bool lf){
    isLeft = lf;
}

void Var::setPtr(bool ptr){
    isPtr = ptr;
}

void Var::setName(string n){
    name = n;
}

void Var::setArray(int len){
    if(len < 0){
        SEMERROR(ARRAY_LEN_INVALID, name);
    }else{
        isArr = true;
        isLeft = false; // 数组不能是左值
        arrayLen = len;
        if(!externed) size *= len;
    }

}

// 获取变量名
string Var::getName(){
    return name;
}

// 获取变量类型
Tag Var::getType(){
    return type;
}

// 获取变量大小
int Var::getSize(){
    return size;
}

// 设置偏移量
void Var::setOffset(int off){
    offset = off;
}

// 获取字符串常量值
string Var::getStrVal(){
    return strVal;
}

//获取scopePath
vector<int>& Var::getPath(){
    return scopePath;
}

// 是否为基本类型
bool Var::isBase(){
    return !isArr && !isPtr;
}

//=================================================================================================//
// 函数对象
Fun::Fun(bool ext, Tag t, string n,vector<Var*>&paraList){
    externed = ext;
    type = t;
    name = n;
    paraVar = paraList;
    curEsp = 0;
    maxDepth = 0;
    // 计算参数相对于栈帧基址的偏移，从8字节开始，参数传递固定为4字节，偏移都为正值
    for(int i = 0, argOff = 8; i < paraVar.size(); i++, argOff+=4){
        paraVar[i]->setOffset(argOff);
    }
}

Fun::~Fun(){

}

#define SEMWARN(code,name) Error::semWarn(code,name)

// 函数匹配检测
bool Fun::match(Fun* f){
    if(name != f->getName()) return false;// 函数名是否一致
    if(paraVar.size() != f->paraVar.size()) return false; // 函数参数长度是否一致
    int len = paraVar.size();
    for(int i = 0; i < len; i++){
        if(GenIR::typeCheck(paraVar[i], f->paraVar[i])){// 判断参数类型是否一致，包含可以相互转换的类型，比如int*和int[]，只能大致判断是否是基本类型
            if(paraVar[i]->getType() != f->paraVar[i]->getType()){// 准确判断类型是否一致
                SEMWARN(FUN_RET_CONFLICT,name);// 不一致报语义警告
            }
        }else{
            return false;
        }
    }
    if(type != f->getType()){// 函数返回类型判断
        SEMWARN(FUN_RET_CONFLICT, name);// 函数返回值冲突
    }
    return true;
}

// 设置extern
void Fun::setExtern(bool ext){
    externed = ext;
}

// 获取extern状态
bool Fun::getExtern(){
    return externed;
}

// 获取函数名
string& Fun::getName(){
    return name;
}

// 获取参数列表
vector<Var*>& Fun::getParaVar(){
    return paraVar;
}

// 获取函数类型
Tag Fun::getType(){
    return type;
}

//进入一个新的作用域
void Fun::enterScope(){
    scopeEsp.push_back(0);
}

//离开一个作用域
void Fun::leaveScope(){
    maxDepth = (curEsp>maxDepth) ? curEsp : maxDepth;
    curEsp -= scopeEsp.back();
    scopeEsp.pop_back();
}

// 定位局部变量栈帧偏移
void Fun::locate(Var* var){
    int size = var->getSize();
    size += (4-size%4)%4;// 按照4字节整数倍来分配局部变量
    scopeEsp.back() += size; // 累加作用域大小
    curEsp += size; // 累加栈指针位置
    var->setOffset(-curEsp); // 局部变量偏移为负数
}
#include "genir.h"
#include "symbol.h"
#include "symtab.h"
#include<sstream>
#include "error.h"

//打印语义错误
#define SEMERROR(code) Error::semError(code)

int GenIR::labelNum = 0;

GenIR::GenIR(SymTab& tab):symtab(tab){
    symtab.setIR(this);//构建符号表与代码生成器的一一关系
	labelNum=0;
	push(NULL,NULL);//初始化作用域 
}

void GenIR::push(InterInst*head,InterInst*tail){
    heads.push_back(head);
    tails.push_back(tail);
}

void GenIR::pop(){
    heads.pop_back();
    tails.pop_back();
}

// 产生变量初始化语句
bool GenIR::genVarInit(Var* var){
    if(var->getName()[0] == '<') return false; // 整形常量和字符常量的变量对象，不需要被初始化
    symtab.addInst(new InterInst(OP_DEC, var));
    if(var->setInit()){// 非常量初始化，生成赋值语句代码
        genTwoOp(var, ASSIGN, var->getInitData()); // 双操作数运算表达式代码生成
    }
    return true;
}

// 或
Var* GenIR::genOr(Var* lval, Var* rval){
    Var* tmp = new Var(symtab.getScopePath(), KW_INT, false);
    symtab.addVar(tmp);
    // tmp = lval || rval
    symtab.addInst(new InterInst(OP_OR, tmp, lval, rval));
    return tmp;
}

// 与
Var* GenIR::genAnd(Var* lval, Var* rval){
    Var* tmp = new Var(symtab.getScopePath(), KW_INT, false);
    symtab.addVar(tmp);
    // tmp = lval && rval
    symtab.addInst(new InterInst(OP_AND, tmp, lval, rval));
    return tmp;

}

// 等于
Var* GenIR::genEqu(Var* lval, Var* rval){
    Var* tmp = new Var(symtab.getScopePath(), KW_INT, false);
    symtab.addVar(tmp);
    // tmp = lval==rval
    symtab.addInst(new InterInst(OP_EQU, tmp, lval, rval));
    return tmp;
}

//不等于
Var* GenIR::genNequ(Var* lval, Var* rval){
    Var* tmp = new Var(symtab.getScopePath(), KW_INT, false);
    symtab.addVar(tmp);
    // tmp = lval!=rval
    symtab.addInst(new InterInst(OP_NE, tmp, lval, rval));
    return tmp;
}

// 大于
Var* GenIR::genGt(Var* lval, Var* rval){
    Var* tmp = new Var(symtab.getScopePath(), KW_INT, false);
    symtab.addVar(tmp);
    // tmp = lval>rval
    symtab.addInst(new InterInst(OP_GT, tmp, lval, rval));
    return tmp;
}

// 大于等于
Var* GenIR::genGe(Var* lval, Var* rval){
    Var* tmp = new Var(symtab.getScopePath(), KW_INT, false);
    symtab.addVar(tmp);
    // tmp = lval>=rval
    symtab.addInst(new InterInst(OP_GE, tmp, lval, rval));
    return tmp;
}

// 小于
Var* GenIR::genLt(Var* lval, Var* rval){
    Var* tmp = new Var(symtab.getScopePath(), KW_INT, false);
    symtab.addVar(tmp);
    // tmp = lval<rval
    symtab.addInst(new InterInst(OP_LT, tmp, lval, rval));
    return tmp;
}

// 小于等于
Var* GenIR::genLe(Var* lval, Var* rval){
    Var* tmp = new Var(symtab.getScopePath(), KW_INT, false);
    symtab.addVar(tmp);
    // tmp = lval<=rval
    symtab.addInst(new InterInst(OP_LE, tmp, lval, rval));
    return tmp;
}

// 除
Var* GenIR::genDiv(Var* lval, Var* rval){
    Var* tmp = new Var(symtab.getScopePath(), KW_INT, false);
    symtab.addVar(tmp);
    // tmp = lval/rval
    symtab.addInst(new InterInst(OP_DIV, tmp, lval, rval));
    return tmp;
}

// 取模
Var* GenIR::genMod(Var* lval, Var* rval){
    Var* tmp = new Var(symtab.getScopePath(), KW_INT, false);
    symtab.addVar(tmp);
    // tmp = lval<rval
    symtab.addInst(new InterInst(OP_MOD, tmp, lval, rval));
    return tmp;
}

// 加法运算语句
Var* GenIR::genAdd(Var* lvar, Var* rvar){
    Var* tmp = NULL;

    // 数组和指针只能和基本类型相加
    if(!lvar->isBase() && rvar->isBase()){
        // ？？？ what i need to do
        // ok, now i know what next to do 
        tmp = new Var(symtab.getScopePath(), lvar);
        rvar = genMul(rvar, Var::getStep(lvar));
        // 对于int *p，计算p+1时，其实是计算p+1*sizeof(int)
    }else if(lvar->isBase() && !rvar->isBase()){
        tmp = new Var(symtab.getScopePath(), rvar);
        lvar = genMul(lvar, Var::getStep(lvar));
    }else if(lvar->isBase() && rvar->isBase()){
        tmp = new Var(symtab.getScopePath(), KW_INT, false);
    }else{
        SEMERROR(EXPR_NOT_BASE); // 两个非基本类型的操作数相加，不合法
        return lvar;
    }
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_ADD, tmp, lvar, rvar));
    return tmp;
}

// 减法运算指令
Var* GenIR::genSub(Var* lval, Var* rval){
    Var* tmp = NULL;
    if(!rval->isBase()){ // 右值必须为基本类型
        SEMERROR(EXPR_NOT_BASE);
        return lval;
    }
    if(lval->getArray() || lval->getPtr()){ // 左值为数组或或者指针时
        tmp = new Var(symtab.getScopePath(), lval);
        rval = genMul(rval, Var::getStep(lval));
    }else{//基本类型
		tmp=new Var(symtab.getScopePath(),KW_INT,false);//基本类型
	}
    symtab.addVar(tmp);
    // tmp = lval-rval
    symtab.addInst(new InterInst(OP_SUB, tmp,lval, rval));
    return tmp;
}

// 乘法
Var* GenIR::genMul(Var* lval, Var* rval){
    Var*tmp = new Var(symtab.getScopePath(), KW_INT, false);// 默认为int类型
    symtab.addVar(tmp);
    symtab.addInst(new InterInst(OP_MUL, tmp, lval, rval));
    return tmp;
}

// 产生一个指针指向变量
Var* GenIR::genPtr(Var* var){
    if(var->isBase()){ // 基本类型变量不能取值
        SEMERROR(EXPR_IS_BASE);
        return NULL;
    }
    Var* tmp = new Var(symtab.getScopePath(), var->getType(), false);
    tmp->setLeft(true);
    tmp->setPointer(var);
    symtab.addVar(tmp);
    return tmp;
}

// 赋值语句
Var* GenIR::genAssign(Var* lval, Var* rval){
    // 被赋值对象必须是左值
    if(!lval->getLeft()){
        SEMERROR(EXPR_NOT_LEFT_VAL);
        return rval;
    }
    // 类型检查
    if(!typeCheck(lval,rval)){
        SEMERROR(ASSIGN_TYPE_ERR); // 赋值类型不匹配
        return rval;
    }
    // 考虑右值*p
    if(rval->isRef()){
        if(!lval->isRef()){
            // lval = *(rval->ptr)
            symtab.addInst(new InterInst(OP_GET, lval, rval->getPointer()));
            return lval;
        }else{
            // *(lval->ptr) = *(rval->ptr)
            // 先对右值进行处理
            // rval = *(rval->ptr)
            rval = genAssign(rval);
        }
    }
    if(lval->isRef()){
        // *(lval->ptr) = rval
        symtab.addInst(new InterInst(OP_SET, rval, lval->getPointer()));
    }else{
        // lval = rval
        symtab.addInst(new InterInst(OP_AS, lval, rval));
    }
    return lval;
}

// 拷贝赋值语句
Var* GenIR::genAssign(Var*val){
    Var*tmp = new Var(symtab.getScopePath(), val); // 拷贝变量信息
    symtab.addVar(tmp);
    if(val->isRef()){ // val被指针指向
        // 中间代码 tmp = *(val->ptr)
        symtab.addInst(new InterInst(OP_GET, tmp, val->getPointer()));
    }else{ // val没有被指针指向
        // 中间代码 tmp = val
        symtab.addInst(new InterInst(OP_AS, tmp, val));
    }
    return tmp;
}

// 产生右单目运算语句
Var* GenIR::genOneOpRight(Var* val, Tag opt){
    if(!val) return NULL;
    if(val->isVoid()){
        SEMERROR(EXPR_IS_VOID); // 变量不能声明为void
        return NULL;
    }
    if(!val->getLeft()){// 判断变量是否为左值
        SEMERROR(EXPR_NOT_LEFT_VAL);
        return val;
    }
    if(opt==INC) return genIncR(val); // 右自加语句
    if(opt==DEC) return genDecR(val); // 右自减语句
    return val;
}

// 右自加
Var* GenIR::genIncR(Var* val){ // 后缀自加先返回操作数的值
    Var* tmp = genAssign(val); // 复制，tmp=val
    // val = val+1
    symtab.addInst(new InterInst(OP_ADD, val, val, Var::getStep(val)));
    return tmp;
}

// 右自减
Var* GenIR::genDecR(Var* val){ // 后缀自减先返回操作数的值
    Var* tmp = genAssign(val); // 复制，tmp=val   
    // val = val-1
    symtab.addInst(new InterInst(OP_SUB, val, val, Var::getStep(val)));  
    return tmp;
}

// 前缀单目运算
Var* GenIR::genOneOpLeft(Tag opt, Var* val){
    if(!val) return NULL;
    if(val->isVoid()){
        SEMERROR(EXPR_IS_VOID);
        return NULL;
    }
    if(opt==LEA) return genLea(val); // 取址
    if(opt==MUL) return genPtr(val); // 指针
    if(opt==INC) return genIncL(val); // 左自增
    if(opt==DEC) return genDecL(val); // 左自减
    if(val->isRef()) val = genAssign(val); // 处理(*p)的情况
    if(opt==NOT) return genNot(val); // 非
    if(opt==SUB) return genMinus(val); // 取负
    return val;
}

// 取址
Var* GenIR::genLea(Var* val){
    if(!val->getLeft()){
        SEMERROR(EXPR_NOT_LEFT_VAL); // 不能取地址
        return val;
    }else if(val->isRef()){ // &(*p) = p
        return val->getPointer();
    }else{
        Var* tmp = new Var(symtab.getScopePath(), val->getType(), true); // 产生局部变量
        symtab.addVar(tmp);
        // tmp = &val
        symtab.addInst(new InterInst(OP_LEA, tmp, val));
        return tmp;
    }
}

// 左自增
Var* GenIR::genIncL(Var* val){
    if(!val->getLeft()){
        SEMERROR(EXPR_NOT_LEFT_VAL); // 变量不是左值
        return val;
    }
    if(val->isRef()){ // val是(*p)的情况
        // ++*p
        // t1 = *p
        // t2 = t1+1
        // *p = t2
        Var* t1 = genAssign(val);
        Var* t2 = genAdd(t1, Var::getStep(val));
        genAssign(val, t2);
    }else{
        // ++val
        symtab.addInst(new InterInst(OP_ADD, val, val, Var::getStep(val)));
    }
    return val;
}

// 左自减
Var* GenIR::genDecL(Var* val){
    if(!val->getLeft()){
        SEMERROR(EXPR_NOT_LEFT_VAL); // 变量不是左值
        return val;
    }
    if(val->isRef()){ // val是(*p)的情况
        // --*p
        // t1 = *p
        // t2 = t1-1
        // *p = t2
        Var* t1 = genAssign(val);
        Var* t2 = genSub(t1, Var::getStep(val));
        genAssign(val, t2);
    }else{
        // --val
        symtab.addInst(new InterInst(OP_SUB, val, val, Var::getStep(val)));
    }
    return val;
}

// 取负
Var* GenIR::genMinus(Var* val){
    if(!val->isBase()){
        SEMERROR(EXPR_NOT_BASE);
        return val;
    }
    Var* tmp = new Var(symtab.getScopePath(), KW_INT, false);
    symtab.addVar(tmp);
    // tmp = -val
    symtab.addInst(new InterInst(OP_NEG, tmp, val));
    return tmp;
}

// 取非
Var* GenIR::genNot(Var* val){
    Var* tmp = new Var(symtab.getScopePath(), KW_INT, false);
    symtab.addVar(tmp);
    // tmp = !val
    symtab.addInst(new InterInst(OP_NOT, tmp, val));
    return tmp;
}

// 双目操作语句
Var* GenIR::genTwoOp(Var* lvar, Tag opt, Var* rvar){
    if(!lvar || !rvar) return NULL;
    if(lvar->isVoid() || rvar->isVoid()){
        SEMERROR(EXPR_IS_VOID); // 变量不能是void类型
        return NULL;
    }
    // 赋值单独处理
    if(opt==ASSIGN) return genAssign(lvar,rvar);
    // 先处理(*p)变量
    if(lvar->isRef()) lvar = genAssign(lvar);
    if(rvar->isRef()) rvar = genAssign(rvar);
    if(opt==OR)return genOr(lvar,rvar);//或
	if(opt==AND)return genAnd(lvar,rvar);//与
	if(opt==EQU)return genEqu(lvar,rvar);//等于
	if(opt==NEQU)return genNequ(lvar,rvar);//不等于
	if(opt==ADD)return genAdd(lvar,rvar);//加
	if(opt==SUB)return genSub(lvar,rvar);//减	
	if(!lvar->isBase() || !rvar->isBase())
	{
		SEMERROR(EXPR_NOT_BASE);//不是基本类型
		return lvar;
	}	
	if(opt==GT)return genGt(lvar,rvar);//大于
	if(opt==GE)return genGe(lvar,rvar);//大于等于
	if(opt==LT)return genLt(lvar,rvar);//小于
	if(opt==LE)return genLe(lvar,rvar);//小于等于
	if(opt==MUL)return genMul(lvar,rvar);//乘
	if(opt==DIV)return genDiv(lvar,rvar);//除
	if(opt==MOD)return genMod(lvar,rvar);//取模
	return lvar;
}

// 产生数组语句
Var* GenIR::genArray(Var* array, Var* index){
    if(!array || !index) return NULL;
    if(array->isVoid() || index->isVoid()){
        SEMERROR(EXPR_IS_VOID);
        return NULL;
    }
    if(array->isBase() || !index->isBase()){// 数组不能是基本类型，索引得是基本类型
        SEMERROR(ARR_TYPE_ERR);
        return index;
    }
    return genPtr(genAdd(array, index));
}

// 产生返回语句
void GenIR::genReturn(Var* ret){
    if(!ret) return;
    Fun *fun = symtab.getCurFun();
    if(ret->isVoid()&&fun->getType()!=KW_VOID || ret->isBase()&&fun->getType()==KW_VOID){
        // 返回类型和函数类型不一致
        SEMERROR(RETURN_ERR);
        return;
    }
    InterInst* returnPoint = fun->getReturnPoint(); // 获取返回点
    if(ret->isVoid()){
        symtab.addInst(new InterInst(OP_RET, returnPoint));
    }else{
        if(ret->isRef()) ret = genAssign(ret); // 处理返回值是指针的情况
        symtab.addInst(new InterInst(OP_RETV,returnPoint,ret));
    }
}

// 参数传递
void GenIR::genPara(Var *arg){
    if(arg->isRef()){ // 参数为指针运算的结果
        arg = genAssign(arg);
    }
    InterInst *argInst = new InterInst(OP_ARG, arg); // 此中间代码用于将参数入栈
    symtab.addInst(argInst);
}

// 函数调用
Var* GenIR::genCall(Fun *fun, vector<Var*> &args){
    if(!fun) return NULL;
    for(int i = args.size()-1; i>=0; i--){ // 考虑函数参数压栈，逆向传递参数
        genPara(args[i]);
    }
    if(fun->getType() == KW_VOID){ // 函数类型为void
        symtab.addInst(new InterInst(OP_PROC, fun)); // 生成OP_PORC函数调用指令，返回void变量
        return Var::getVoid();
    }else{
        Var *ret = new Var(symtab.getScopePath(), fun->getType(), false); // 生成和函数类型一致的返回值
        symtab.addInst(new InterInst(OP_CALL, fun, ret)); // 函数OP_CALL调用指令
        symtab.addVar(ret);
        return ret;
    }
}

// 产生函数入口代码
void GenIR::genFunHead(Fun*fun){
    fun->enterScope(); // 进入函数
    symtab.addInst(new InterInst(OP_ENTRY,fun)); // 函数入口
    fun->setReturnPoint(new InterInst); // 创建返回点
}

// 产生函数出口代码
void GenIR::genFunTail(Fun*fun){
    symtab.addInst(fun->getReturnPoint()); // 函数返回点
    symtab.addInst(new InterInst(OP_EXIT, fun)); // 函数出口
    fun->leaveScope(); // 退出函数
}

// 产生if头部
void GenIR::genIfHead(Var* cond, InterInst* &_else){
    _else = new InterInst(); // 产生else标签
    if(cond){
        if(cond->isRef()) cond = genAssign(cond);
        // OP_JF _else, cond
        // if(!cond) goto _else
        symtab.addInst(new InterInst(OP_JF, _else, cond));
    }
}

// 产生if尾部, 在没有else的情况下
void GenIR::genIfTail(InterInst* &_else){
    symtab.addInst(_else);
}

// 产生else头部
void GenIR::genElseHead(InterInst* _else, InterInst* &_exit){
    _exit = new InterInst(); // 产生exit标签
    // goto _exit
    symtab.addInst(new InterInst(OP_JMP, _exit));
    symtab.addInst(_else);
}

// 产生else尾部
void GenIR::genElseTail(InterInst* &_exit){
    symtab.addInst(_exit);
}

// 产生switch头部
void GenIR::genSwitchHead(InterInst*& _exit){
    _exit = new InterInst(); // _exit标签
    push(NULL,_exit); // 不允许continue
}

// 产生switch尾部
void GenIR::genSwitchTail(InterInst* _exit){
    symtab.addInst(_exit);
    pop();
}

// 产生case头部
void GenIR::genCaseHead(Var* cond, Var* lb, InterInst* &_case_exit){
    _case_exit = new InterInst(); // 产生case的exit标签
    // if(cond != lb) goto _case_exit
    if(lb) symtab.addInst(new InterInst(OP_JNE, _case_exit,cond, lb));
}

// 产生case尾部
void GenIR::genCaseTail(InterInst* _case_exit){
    symtab.addInst(_case_exit); // 添加case的exit标签
}

// 产生while头部
void GenIR::genWhileHead(InterInst* &_while, InterInst* &_exit){
    _while = new InterInst(); // 创建while标签
    symtab.addInst(_while); // 添加while标签
    _exit = new InterInst(); // 产生exit标签
    push(_while, _exit); // 进入while
}

// 处理while循环条件
void GenIR::genWhileCond(Var* cond, InterInst* _exit){
    if(cond){
        if(cond->isVoid()) cond = Var::getTrue(); // 处理空表达式
        else if(cond->isRef()) cond = genAssign(cond);
        // if(!cond) goto _exit
        symtab.addInst(new InterInst(OP_JF, _exit, cond));
    }
}

// 产生while尾部
void GenIR::genWhileTail(InterInst* &_while, InterInst* &_exit){
    // goto _while
    symtab.addInst(new InterInst(OP_JMP, _while));
    // 添加_exit标签 
    symtab.addInst(_exit);
    pop(); // 离开while
}

// 产生dowhile头部
void GenIR::genDoWhileHead(InterInst* &_do, InterInst* &_exit){
    _do = new InterInst(); // 产生do标签
    symtab.addInst(_do);
    _exit = new InterInst(); // 产生exit标签
    push(_do, _exit); // 进入dowhile作用域
}

// 产生dowhile尾部
void GenIR::genDoWhileTail(Var* cond, InterInst* _do, InterInst* _exit){
    if(cond){
        if(cond->isVoid()) cond=Var::getTrue();
        else if(cond->isRef()) cond=genAssign(cond);
        // if(cond) goto _do
        symtab.addInst(new InterInst(OP_JT, _do, cond));
    }
    symtab.addInst(_exit);
    pop();
}

/*
    do init
_for:
    do cond
    OP_JF _exit cond
    OP_JMP _block
_step:
    do step
    OP_JMP _for
_block:
    do loop
    OP_JMP _step
_exit:
*/
void GenIR::genForHead(InterInst*& _for, InterInst*& _exit){
    _for = new InterInst();
    _exit = new InterInst();
    symtab.addInst(_for);
}

void GenIR::genForCondBegin(Var* cond, InterInst*& _step, InterInst*& _block, InterInst* _exit){
    _block = new InterInst();
    _step = new InterInst();
    if(cond){
        if(cond->isVoid()) cond = Var::getTrue();
        else if(cond->isRef()) cond = genAssign(cond);
        symtab.addInst(new InterInst(OP_JF, _exit, cond));
        symtab.addInst(new InterInst(OP_JMP,_block));
    }
    symtab.addInst(_step);
    push(_step,_exit);
}

void GenIR::genForCondEnd(InterInst* _for, InterInst* _block){
    symtab.addInst(new InterInst(OP_JMP, _for));
    symtab.addInst(_block);
}

void GenIR::genForTail(InterInst*& _step, InterInst*& _exit){
    symtab.addInst(new InterInst(OP_JMP, _step));
    symtab.addInst(_exit);
    pop();
}

void GenIR::genBreak(){
    InterInst* tail = tails.back();
    if(tail){
        symtab.addInst(new InterInst(OP_JMP, tail));
    }
    else SEMERROR(BREAK_ERR); // break不在循环或者switch-case中
}

void GenIR::genContiue(){
    InterInst* head = heads.back();
    if(head) symtab.addInst(new InterInst(OP_JMP, head));
    else SEMERROR(CONTINUE_ERR);
}

// 产生唯一标签
string GenIR::genLb(){
    labelNum++;
    string lb = ".L";
    stringstream ss;
    ss<<labelNum;
    return lb+ss.str();
}

bool GenIR::typeCheck(Var* v1, Var* v2){
    if(!v2) return false;
    if(v1->isBase() && v2->isBase()) return true;// 都是基本类型
    else if(!v1->isBase() && !v2->isBase()){// 都不是基本类型
        return v1->getType() == v2->getType();
    }
    return false;
}

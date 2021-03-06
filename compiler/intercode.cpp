#include"intercode.h"
#include"symbol.h"
#include"genir.h"

// ======================================中间指令==================================

void InterInst::init(){
    op = OP_NOP;
    this->arg1 = NULL;
    this->arg2 = NULL;
    this->result = NULL;
    this->fun = NULL;
    this->target = NULL;
    first = false;
}

// 一般运算指令
InterInst::InterInst(Operator op,Var *rs,Var *arg1,Var *arg2){
    init();
    this->op = op;
    this->result = rs;
    this->arg1 = arg1;
    this->arg2 = arg2;
}

// 函数调用指令
InterInst::InterInst(Operator op, Fun* fun, Var* rs){
    init();
    this->op = op;
    this->fun = fun;
    this->result = rs;
    this->arg2 = NULL;
}

// 参数进栈指令
InterInst::InterInst(Operator op, Var* arg1){
    init();
    this->op = op;
    this->result = NULL;
    this->arg1 = arg1;
    this->arg2 = NULL;
}

// 产生唯一标号
InterInst::InterInst(){
    init();
    label=GenIR::genLb();
}

// 条件跳转指令
InterInst::InterInst(Operator op,InterInst *tar,Var *arg1,Var *arg2){
    init();
    this->op = op;
    this->target = tar;
    this->arg1 = arg1;
    this->arg2 = arg2;
}

void InterInst::setFirst(){
    first=true;
}

bool InterInst::isFirst(){
    return first;
}

bool InterInst::isJcond(){
    return op>=OP_JT&&op<=OP_JNE;
}

bool InterInst::isJmp(){
    return op==OP_JMP||op==OP_RET||op==OP_RETV;
}

bool InterInst::isLb(){
    return label!="";
}

// 是基本类型表达式运算，可以对指针取值
bool InterInst::isExpr(){
    return (op>=OP_AS&&op<=OP_OR||op==OP_GET);
}

bool InterInst::unknown(){
    return op==OP_SET||op==OP_PROC||op==OP_CALL;
}

Operator InterInst::getOp(){
    return op;
}

InterInst* InterInst::getTarget(){
    return target;
}

Var* InterInst::getResult(){
    return result;
}

void InterInst::setArg1(Var*arg1){
    this->arg1=arg1;
}

// 是否是声明
bool InterInst::isDec(){
    return op==OP_DEC;
}

Var* InterInst::getArg1(){
    return arg1;
}

Var* InterInst::getArg2(){
    return arg2;
}

string InterInst::getLabel(){
    return label;
}

Fun* InterInst::getFun(){
    return fun;
}

// 输出指令
void InterInst::toString(){
    if(label != ""){
        printf("%s:\n", label.c_str());
        return;
    }
    switch (op)
    {
    case OP_DEC:
        printf("dec ");
        arg1->value();
        break;
    case OP_ENTRY:
        printf("entry");
        break;
    case OP_EXIT:
        printf("exit");
        break;
    case OP_AS:
        result->value();
        printf(" = ");
        arg1->value();
        break;
    case OP_ADD:
        result->value();
        printf(" = ");
        arg1->value();
        printf(" + ");
        arg2->value();
        break;
    case OP_SUB:
        result->value();
        printf(" = ");
        arg1->value();
        printf(" - ");
        arg2->value();
        break;
    case OP_MUL:
        result->value();
        printf(" = ");
        arg1->value();
        printf(" * ");
        arg2->value();
        break;
    case OP_DIV:
        result->value();
        printf(" = ");
        arg1->value();
        printf(" / ");
        arg2->value();
        break;
    case OP_MOD:
        result->value();
        printf(" = ");
        arg1->value();
        printf(" %% ");
        arg2->value();
        break;
    case OP_NEG:
        result->value();
        printf(" = -");
        arg1->value();
        break;
    case OP_GT:
        result->value();
        printf(" = ");
        arg1->value();
        printf(" > ");
        arg2->value();
        break;
    case OP_GE:
        result->value();
        printf(" = ");
        arg1->value();
        printf(" >= ");
        arg2->value();
        break;
    case OP_LT:
        result->value();
        printf(" = ");
        arg1->value();
        printf(" < ");
        arg2->value();
        break;
    case OP_LE:
        result->value();
        printf(" = ");
        arg1->value();
        printf(" <= ");
        arg2->value();
        break;
    case OP_EQU:
        result->value();
        printf(" = ");
        arg1->value();
        printf(" == ");
        arg2->value();
        break;
    case OP_NE:
        result->value();
        printf(" = ");
        arg1->value();
        printf(" != ");
        arg2->value();
        break;
    case OP_NOT:
        result->value();
        printf(" = ");
        printf("!");
        arg1->value();
        break;
    case OP_AND:
        result->value();
        printf(" = ");
        arg1->value();
        printf(" && ");
        arg2->value();
        break;
    case OP_OR:
        result->value();
        printf(" = ");
        arg1->value();
        printf(" || ");
        arg2->value();
        break;
    case OP_JMP:
        printf("goto %s", target->label.c_str());
        break;
    case OP_JT:
        printf("if(");
        arg1->value();
        printf(") goto %s", target->label.c_str());
        break;
    case OP_JF:
        printf("if(!");
        arg1->value();
        printf(") goto %s", target->label.c_str());
        break;
    case OP_JNE:
        printf("if(");
        arg1->value();
        printf(" != ");
        arg2->value();
        printf(") goto %s", target->label.c_str());
        break;
    case OP_ARG:
        printf("arg ");
        arg1->value();
        break;
    case OP_PROC:
        printf("%s()", fun->getName().c_str());
        break;
    case OP_CALL:
        result->value();
        printf(" = %s()", fun->getName().c_str());
        break;
    case OP_RETV:
        printf("return ");
        arg1->value();
        printf(" goto %s", target->label.c_str());
        break;
    case OP_RET:
        printf("return goto %s", target->label.c_str());
        break;
    case OP_LEA:
        result->value();
        printf(" = ");
        printf("&");
        arg1->value();
        break;
    case OP_SET:
        printf("*");
        arg1->value();
        printf(" = ");
        result->value();
        break;
    case OP_GET:
        result->value();
        printf(" = ");
        printf("*");
        arg1->value();
        break;
    }
    printf("\n");
}


// ======================================中间代码===================================

// 添加中间代码
void InterCode::addInst(InterInst* inst){
    code.push_back(inst);
}

void InterCode::markFirst(){
    unsigned int len=code.size();
    code[0]->setFirst();
    code[len-1]->setFirst();
    if(len>2) code[1]->setFirst();

    for(unsigned int i=1; i<len-1; i++){
        if(code[i]->isJmp()||code[i]->isJcond()){// 直接跳转，条件跳转目标以及紧跟其后的指令
            code[i]->getTarget()->setFirst();
            code[i+1]->setFirst();
        }
    }
}

// 获取中间代码序列
vector<InterInst*>& InterCode::getCode(){
    return code;
}

// 输出中间代码信息
void InterCode::toString(){
    for(int i = 0; i < code.size(); i++){
        code[i]->toString();
    }
}

// 清除中间代码
InterCode::~InterCode(){
    for(int i = 0; i < code.size(); i++){
        delete code[i];
    }
}
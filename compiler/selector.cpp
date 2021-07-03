#include "selector.h"
#include "intercode.h"
#include "symbol.h"
#include "platform.h"

Selector::Selector(vector<InterInst*>& irCode, ILoc& ilocCode)
    :ir(irCode),iloc(ilocCode)
{}

/*
    指令选择
*/
void Selector::select(){
    for(unsigned int i=0; i<ir.size(); i++){
        translate(ir[i]);
    }
}

/*
    翻译
*/
void Selector::translate(InterInst* inst){
    string label = inst->getLabel();
    if(label!=""){ // 打印标签
        iloc.label(label);
        return;
    }
    Operator op=inst->getOp();//操作符
    if(op==OP_ENTRY){//函数入口
        Fun* fun = inst->getFun();
        iloc.comment("entry of procedure");
        iloc.inst("mov","ip","sp");
        if(fun->isRelocated()){
            iloc.inst("stmfd","sp!","{r0-r7,fp,ip,lr,pc}");// 保护现场
        }
        else{
            iloc.inst("stmfd","sp!","{fp,ip,lr,pc}");// 不用保护现场
        }
        iloc.inst("sub","fp","ip","#4");
        iloc.comment("create stack of function");
        iloc.allocStack(fun,"r8"); // 为函数分配栈帧
        iloc.comment("load parameters to register");
        iloc.ldr_args(fun); // 加载参数到对应到寄存器
        iloc.comment("text of procedure");
    }
    else if(op==OP_EXIT){// 函数出口
        iloc.comment("exit procedure");
        if(inst->getFun()->isRelocated()){
            iloc.inst("ldmea","fp","{r0-r7,fp,sp,pc}"); // 恢复现场
        }
        else{
            iloc.inst("ldmea","fp","{fp,sp,pc}"); // 不用恢复现场
        }
    }
    else if(op==OP_DEC){ // 变量声明,初始化
        iloc.init_var(inst->getArg1(),"r8","r9"); // 初始值在r8可能用到r9
    }
    else if(op==OP_LEA){ // 取地址
        iloc.lea_var("r8",inst->getArg1()); // &arg1->r8
        iloc.str_var("r8",inst->getResult(),"r9"); // r8->rs 可能用到r9
    }
    else if(op==OP_SET){ // 设置指针指
        iloc.ldr_var("r8",inst->getResult()); // rs->r8
        iloc.ldr_var("r9",inst->getArg1()); // arg1->r9
        //虽然基址寄存器r9不能修改（r9又是临时寄存器），但是disp=0，因此不会使用r9作为临时寄存器
        iloc.str_base("r8","r9",0,"r9",inst->getArg1()->isCharPtr());//rs -> *arg1
        iloc.nop();//占位
    }
    else if(op==OP_JMP||op==OP_JT||op==OP_JF||op==OP_JNE||op==OP_RET||op==OP_RETV){ // 跳转，函数返回
        string des=inst->getTarget()->getLabel(); //要跳转到目标地址
        iloc.ldr_var("r8",inst->getArg1()); // arg1->r8
        iloc.ldr_var("r9",inst->getArg2()); // arg2->r9
        switch (op)
        {
        case OP_JMP:iloc.inst("b",des);break;
        case OP_JT:iloc.inst("cmp","r8","#0");iloc.inst("bne",des);break;
        case OP_JF:iloc.inst("cmp","r8","#0");iloc.inst("beq",des);break;
        case OP_RET:iloc.inst("b",des);break;
        case OP_RETV:iloc.inst("b",des);break;
        case OP_JNE:
            /* 
                set r8 with 1 when r8 is not equal t0 r9, otherwise set r8 0,
                then compare r8 to 0, branch to the destination if not equal
            */
            iloc.cmp("ne","eq","r8","r8","r9"); //r8!=r9 -> r8
            iloc.inst("cmp","r8","#0"); // cmp r8 #0
            iloc.inst("bne",des);
            break;
        }
    }
    else if(op==OP_ARG){ // push parameter into stack
        iloc.ldr_var("r8",inst->getArg1()); // arg1->r8
        iloc.inst("stmfd","sp!","{r8}"); // push r8 in stack
    }
    else if(op==OP_CALL){ // call function with return value
        iloc.call_fun(inst->getFun(),"r9"); // fun() -> r8, recover stack maybe need r9
        iloc.str_var("r8",inst->getResult(),"r9");// r8 -> rs, maybe will use r9
    }
    else if(op==OP_PROC){// call function without return value
        iloc.call_fun(inst->getFun(),"r9"); // fun()
    }
    else{ // numerical expression calculation
        Var* rs=inst->getResult();
        Var* arg1=inst->getArg1();
        Var* arg2=inst->getArg2();
        iloc.ldr_var("r8",arg1); // arg1 -> r8
        iloc.ldr_var("r9",arg2); // arg2 -> r9
        switch (op)
        {
            //assign
            case OP_AS:break;
            // arithmetic
            case OP_ADD:iloc.inst("add","r8","r8","r9");break;
            case OP_SUB:iloc.inst("sub","r8","r8","r9");break;
            case OP_MUL:
                iloc.inst("mul","r10","r8","r9");
                iloc.inst("mov","r8","r10");
                break;
            case OP_DIV:iloc.call_lib("__divsi3","r8","r8","r9");break;
            case OP_MOD:iloc.call_lib("__modsi3","r8","r8","r9");break;
            case OP_NEG:iloc.inst("rsb","r8","r8","#0");break; // r8=0-r8
            // compare
            case OP_GT:iloc.cmp("gt","le","r8","r8","r9");break;
            case OP_GE:iloc.cmp("ge","lt","r8","r8","r9");break;
            case OP_LT:iloc.cmp("lt","ge","r8","r8","r9");break;
            case OP_LE:iloc.cmp("le","gt","r8","r8","r9");break;
			case OP_EQU:iloc.cmp("eq","ne","r8","r8","r9");break;
			case OP_NE:iloc.cmp("ne","eq","r8","r8","r9");break;
            // logic
            case OP_AND:iloc.logic_and("r8","r8","r9");break;
			case OP_OR:iloc.logic_or("r8","r8","r9");break;
			case OP_NOT:iloc.logic_not("r8","r8");break;

            // pointer, need to consider type translation
            // a = *p
            case OP_GET:iloc.ldr_base("r8","r8",0,rs->isChar());break;
        }
        iloc.str_var("r8",rs,"r9"); // r8 -> rs
    }
}
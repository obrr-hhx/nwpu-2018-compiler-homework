#include "selector.h"
#include "intercode.h"
#include "symbol.h"
#include "platform.h"

Selector::Selector(vector<InterInst*>& irCode, ILoc& ilocCode, InstMips& instMips)
    :ir(irCode),iloc(ilocCode),instMips(instMips)
{}

/*
    指令选择
*/
void Selector::select(bool arm){
    if(arm){
        for(unsigned int i=0; i<ir.size(); i++){
            translate(ir[i]);
        }
    }
    else{
        for(unsigned int i=0; i<ir.size(); i++){
            translate_mips(ir[i]);
        }
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
        iloc.comment("Prologue");
        iloc.inst("mov","ip","sp");
        if(fun->isRelocated()){
            iloc.inst("stmfd","sp!","{r0-r7,fp,ip,lr,pc}");// 保护现场
        }
        else{
            iloc.inst("stmfd","sp!","{fp,ip,lr,pc}");// 不用保护现场
        }
        iloc.inst("sub","fp","ip","#4");
        // if(fun->getLeaf()){ // 叶子函数
        //     iloc.comment("leaf");
        //     iloc.inst("push","{r11}");
        //     iloc.inst("add","r11","sp","#0");
        //     iloc.inst("sub","sp","sp","#12");
        // }else{// 非叶子函数
        //     iloc.comment("non-leaf");
        //     iloc.inst("push","{r11,lr}");
        //     // iloc.inst("add","r11","sp","#0");
        //     iloc.inst("add","r11","sp","#4");
        //     iloc.inst("sub","sp","sp","#16");
        // }
        iloc.comment("create stack of function");
        iloc.allocStack(fun,"r8"); // 为函数分配栈帧
        iloc.comment("load parameters to register");
        iloc.ldr_args(fun); // 加载参数到对应到寄存器
        iloc.comment("text of procedure");
    }
    else if(op==OP_EXIT){// 函数出口
        iloc.comment("Epilogue");
        if(inst->getFun()->isRelocated()){
            iloc.inst("ldmea","fp","{r0-r7,fp,sp,pc}"); // 恢复现场
        }
        else{
            iloc.inst("ldmea","fp","{fp,sp,pc}"); // 不用恢复现场
        }
        // if(inst->getFun()->getLeaf()){ // leaf procedure
        //     iloc.inst("add","sp","r11","#0");
        //     iloc.inst("pop","{r11}");
        //     iloc.inst("bx","lr");
        // }else{ // non-leaf procedure
        //     iloc.inst("sub","sp","r11","#4");
        //     iloc.inst("pop","{r11,pc}");
        // }
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
                    set r8 with 1 when r8 is not equal to r9, otherwise set r8 0,
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

void Selector::translate_mips(InterInst* inst){
    string label = inst->getLabel();
    if(label!=""){
        instMips.label(label);
        return;
    }
    Operator op = inst->getOp();
    if(op==OP_ENTRY){
        Fun* fun = inst->getFun();
        instMips.comment("Prologue");
        if(fun->getLeaf()){
            instMips.comment("leaf");
            instMips.inst("addi","$sp","$sp","-8");
            instMips.inst("sw","$fp","($sp)");
            instMips.inst("move","$fp","$sp");
            instMips.inst("addi","$sp","$sp","-4");
        }else{ // non-leaf
            instMips.comment("non-leaf");
            string mainName = inst->getFun()->getName();
            if(mainName.compare("main")){// non-main
                instMips.inst("addi","$sp","$sp","-8");
                instMips.inst("sw","$fp","($sp)");
                instMips.inst("sw","$ra","4($sp)");
                instMips.inst("move","$fp","$sp");
                instMips.inst("addi","$sp","$sp","-4");
            }else{// main
                instMips.inst("addi","$sp","$sp","-4");
                instMips.inst("sw","$fp","($sp)");
                instMips.inst("move","$fp","$sp");
                instMips.inst("addi","$sp","$sp","-4");
            }
        }
        instMips.comment("create stack for procedure");
        instMips.allocStack(fun,"$v0");
        instMips.comment("load parameters to register");
        instMips.lw_args(fun);
        instMips.comment("content of procedure");
    }else if(op==OP_EXIT){
        instMips.comment("Epilogue");
        if(inst->getFun()->getLeaf()){
            instMips.inst("add","$sp","$fp","$zero");
            instMips.inst("lw","$fp","($sp)");
            instMips.inst("addi","$sp","$sp","8");
            string mainName = inst->getFun()->getName();
            if(mainName.compare("main")){
                instMips.inst("jr","$ra");
            }
        }else{// non-leaf
            instMips.inst("add","$sp","$fp","$zero");
            instMips.inst("lw","$fp","-4($sp)");
            string mainName = inst->getFun()->getName();
            if(mainName.compare("main")){ // non-main
                instMips.inst("lw","$ra","($sp)");
            }
            instMips.inst("addi","$sp","$sp","4");
            if(mainName.compare("main")){ // non-main
                instMips.inst("jr","$ra");
            }
            if(!mainName.compare("main")){
                instMips.inst("li","$v0","10");
                instMips.syscall();
            }
        }
    }
    else if(op==OP_DEC){
        instMips.init_var(inst->getArg1(),"$v0","$v1");// 初始值用到$v0, 可能用到$v1
    }
    else if(op==OP_LEA){
        instMips.lea_var("$v0",inst->getArg1());// &arg1 -> $v0
        instMips.str_var("$v0",inst->getResult(),"$v1");// $v0 -> rs
    }
    else if(op==OP_SET){
        instMips.lw_var("$v0",inst->getResult()); // rs -> $v0
        instMips.lw_var("$v1",inst->getArg1()); // arg1 -> $v1
        instMips.sw_base("$v0","$v1",0,"$v1",inst->getArg1()->isCharPtr());// rs -> *arg1
        // sw $v0 ($v1)
        instMips.nop();
    }
    else if(op==OP_JMP||op==OP_JT||op==OP_JF||op==OP_JNE||op==OP_RET||op==OP_RETV){
        string des=inst->getTarget()->getLabel();
        instMips.lw_var("$v0",inst->getArg1());
        instMips.lw_var("$v1",inst->getArg2());
        switch(op){
            case OP_JMP:instMips.inst("b",des);break;
            case OP_JT:instMips.inst("bne","$v0","$zero",des);break;
            case OP_JF:instMips.inst("beq","$v0","$zero",des);break;
            case OP_RET:instMips.inst("b",des);break;
            case OP_RETV:instMips.inst("b",des);break;
            case OP_JNE:instMips.inst("bne","$v0","$v1",des);break;
        }
    }
    else if(op==OP_ARG){
        instMips.lw_var("$v0",inst->getArg1());// arg1 -> $v0
        // push $v0
        instMips.inst("addi","$sp","$sp","-4");
        instMips.inst("sw","$v0","($sp)");
    }
    else if(op==OP_CALL){
        instMips.call_fun(inst->getFun(), "$v1");
        instMips.str_var("$v0",inst->getResult(),"$v1");
    }
    else if(op==OP_PROC){
        instMips.call_fun(inst->getFun(),"$v1");
    }
    else{
        Var* rs = inst->getResult();
        Var* arg1 = inst->getArg1();
        Var* arg2 = inst->getArg2();
        instMips.lw_var("$v0",arg1);
        instMips.lw_var("$v1",arg2);
        switch(op){
            case OP_AS:break;
            case OP_ADD:instMips.inst("add","$v0","$v0","$v1");break;
            case OP_SUB:instMips.inst("sub","$v0","$v0","$v1");break;
            case OP_MUL:
                instMips.inst("mul","$t0","$v0","$v1");
                instMips.inst("move","$v0","$t0");
                break;
            case OP_DIV:
                instMips.inst("div","$v0","$v1");
                instMips.inst("mflo","$v0");
                break;
            case OP_MOD:
                instMips.inst("div","$v0","$v1");
                instMips.inst("mfhi","$v0");
                break;
            case OP_NEG:instMips.inst("sub","$v0","$zero","$v0");break;
            case OP_GT:instMips.cmp("gt","$v0","$v0","$v1");break;
            case OP_GE:instMips.cmp("ge","$v0","$v0","$v1");break;
            case OP_LT:instMips.cmp("lt","$v0","$v0","$v1");break;
            case OP_LE:instMips.cmp("le","$v0","$v0","$v1");break;
            case OP_EQU:instMips.cmp("eq","$v0","$v0","$v1");break;
            case OP_NE:instMips.cmp("ne","$v0","$v0","$v1");break;
            case OP_AND:instMips.logic_and("$v0","$v0","$v1");break;
            case OP_OR:instMips.logic_or("$v0","$v0","$v1");break;
            case OP_NOT:instMips.logic_not("$v0","$v0");break;
            case OP_GET:instMips.lw_base("$v0","$v0",0,rs->isChar());break;
        }
        instMips.str_var("$v0",rs,"$v1"); // $v0 -> rs
    }
}
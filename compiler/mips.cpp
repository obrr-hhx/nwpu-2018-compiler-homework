#include "mips.h"
#include "platform.h"
#include "symbol.h"
#include <sstream>

MIPS::MIPS(string op, string rs, string arg1, string arg2, string add)
    :opcode(op),result(rs),reg1(arg1),reg2(arg2),addition(add),dead(false)
{}

// replace instruction
void MIPS::replace(string op, string rs, string a1, string a2, string add){
    opcode = op;
    result = rs;
    reg1 = a1;
    reg2 = a2;
    addition = add;
}

// set invalid instruction
void MIPS::setDead(){
    dead=true;
}

string MIPS::outPut(){
    if(dead) return "";
    if(opcode=="") return "nop";
    string ret = opcode+" "+result;
    if(reg1!="") ret += ","+reg1;
    if(reg2!="") ret += ","+reg2;
    if(addition!="") ret += ","+addition;
    return ret;
}

#define emit(args...) code.push_back(new MIPS(args))

string InstMips::toStr(int num){
    string ret="";
    stringstream ss;
    ss<<num;
    ret += ss.str();
    return ret;
}

void InstMips::label(string name){
    emit(name,":");
}

void InstMips::comment(string comment){
    emit("#",comment);
}

// 0 operand
void InstMips::inst(string op, string rs){
    emit(op,rs);
}

// 1 operand
void InstMips::inst(string op, string rs, string reg1){
    emit(op,rs,reg1);
}

// 2 operands
void InstMips::inst(string op, string rs, string reg1, string reg2){
    emit(op,rs,reg1,reg2);
}

void InstMips::lw_imm(string rsReg, int num){
    emit("li", rsReg, toStr(num));
}

void InstMips::lw_lb(string rsReg, string name){
    emit("la", rsReg, name);
}

// 基址寻址 lw $t0,100($fp)
void InstMips::lw_base(string rsReg, string base, int disp, bool isChar){
    if(disp) base = toStr(disp)+"("+base+")"; // 16($fp)
    else base = "("+base+")";
    emit(isChar?"lb":"lw",rsReg,base);
}

void InstMips::sw_base(string srcReg, string base, int disp, string tmpReg, bool isChar){
    if(disp) base = toStr(disp)+"("+base+")";
    else base = "("+base+")";
    emit(isChar?"sb":"sw", srcReg, base);
}

void InstMips::init_var(Var* var, string initReg, string tmpReg){
    if(!var->unInit()){
        if(var->isBase()){
            inst("li",initReg,toStr(var->getVal())); // li v0,100
        }
        else{
            lw_lb(initReg, var->getPtrVal());
        }
        str_var(initReg, var, tmpReg); // 将初始值保存到变量内存
    }
}

// 加载变量到寄存器
void InstMips::lw_var(string rsReg, Var* var){
    if(!var) return;
    bool isChar=var->isChar();
    if(var->notConst()){ // 变量不是常量
        int id = var->regId;
        if(id != -1){
            emit("move", rsReg, Plat::regName_mips[id]);
        }
        else{
            int off=var->getOffset();
            bool isGlb = !off; // 是否是全局变量
            bool isVar = !var->getArray(); // 0为数组，1为变量
            if(isGlb){
                lw_lb(rsReg,var->getName());//la r8,glb
				if(isVar)lw_base(rsReg,rsReg,0,isChar);
            }
            else{
                if(isVar) lw_base(rsReg,"$fp",off,isChar);// lw $v0,-16($fp)
                else leaStack(rsReg,off); // add $v0,$fp,-16
            }
        }
    }else{ // 常量
        if(var->isBase()){
            lw_imm(rsReg,var->getVal());
        }else{
            lw_lb(rsReg,var->getName());
        }
    }
}

// 加载变量地址到寄存器
void InstMips::lea_var(string rsReg, Var*var){
    int off=var->getOffset();
    bool isGlb=!off;
    if(isGlb){
        emit("la",rsReg,var->getName());// la $v0,glb
    }else{
        leaStack(rsReg,off); // lea $v0, -16($fp)
    }
}

void InstMips::str_var(string srcReg, Var* var, string tmpReg){
    int id=var->regId;
    bool isChar=var->isChar();
    if(id!=-1){
        emit("move",Plat::regName_mips[id],srcReg);// move $t0,$v0
    }else{
        int off=var->getOffset();
        bool isGlb = !off;
        if(isGlb){
            if(!var->isBase())
                emit("la", tmpReg, var->getName());
            else
                lw_lb(tmpReg,var->getName());
            sw_base(srcReg,tmpReg,0,tmpReg,isChar);// sw $v0,($v1)
        }
        else{
            sw_base(srcReg,"$fp",off,tmpReg,isChar);// sw $v0,-16($fp)
        }
    }
    nop();
}

// 加载栈内变量地址
void InstMips::leaStack(string rsReg, int off){
    if(off>0){
        // emit("lw",rsReg,toStr(off)+"($fp)");
        lw_base(rsReg,"$fp",off,false);
    }else
        emit("addi",rsReg,"$fp",toStr(off));
}

void InstMips::allocStack(Fun*fun, string tmpReg){
    int base = Plat::stackBase_mips;
    if(fun->isRelocated()) base=Plat::stackBase_protect_mips;
    int off=fun->getMaxDepth()-base;
    emit("addi","$sp","$sp","-"+toStr(off));
}

// 将函数参数从栈内加载到寄存器中
void InstMips::lw_args(Fun*fun){
    vector<Var*> args = fun->getParaVar();
    for(int i=0; i<args.size(); i++){
        Var*arg=args[i];
        if(arg->regId != -1){
            lw_base(Plat::regName_mips[arg->regId],"$fp",arg->getOffset(),false);
        }
    }
}

void InstMips::call_fun(Fun*fun, string tmpReg){
    string funName=fun->getName();
    emit("jal",funName);
    int off=fun->getParaVar().size();
    off *= 4;
    emit("addi","$sp","$sp",toStr(off));
}

void InstMips::logic_and(string rsReg, string reg1, string reg2){
    emit("and",rsReg,reg1,reg2);
}

void InstMips::logic_or(string rsReg, string reg1, string reg2){
    emit("or", rsReg, reg1, reg2);
}

void InstMips::logic_not(string rsReg, string reg1){
    emit("nor",rsReg,reg1,"1");
}

void InstMips::cmp(string op, string rsReg, string reg1, string reg2){
    if(!op.compare("gt")){
        emit("sgt",rsReg,reg1,reg2);
    }else if(!op.compare("ge")){
        emit("sge",rsReg,reg1,reg2);
    }
    else if(!op.compare("lt")){
        emit("slt",rsReg,reg1,reg2);
    }
    else if(!op.compare("le")){
        emit("sle",rsReg,reg1,reg2);
    }
    else if(!op.compare("eq")){
        emit("seq",rsReg,reg1,reg2);
    }
    else if(!op.compare("ne")){
        emit("sne",rsReg,reg1,reg2);
    }
}

void InstMips::nop(){
    emit("");
}

void InstMips::syscall(){
    emit("syscall");
}

void InstMips::outPut(FILE*file){
    for(list<MIPS*>::iterator it=code.begin(); it!=code.end(); it++){
        string s=(*it)->outPut();
        if(s!="") fprintf(file,"\t%s\n",s.c_str());
    }
}

list<MIPS*>& InstMips::getCode(){
    return code;
}
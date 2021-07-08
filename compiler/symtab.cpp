#include "symtab.h"
#include "error.h"
#include "symbol.h"
#include "genir.h"

//打印语义错误
#define SEMERROR(code,name) Error::semError(code,name)

Var* SymTab::voidVar = NULL;
Var* SymTab::zero = NULL;
Var* SymTab::one = NULL;
Var* SymTab::four = NULL;

//初始化符号表
SymTab::SymTab()
{
	/*
		此处产生特殊的常量void，1,4
	*/
	voidVar=new Var();//void变量
	zero=new Var(1);//常量0
	one=new Var(1);//常量1
	four=new Var(4);//常量4
	addVar(voidVar);//让符号表管理这些特殊变量
	addVar(one);//让符号表管理这些特殊变量
	addVar(zero);//让符号表管理这些特殊变量
	addVar(four);//让符号表管理这些特殊变量

	scopeId=0;
	curFun=NULL;
	ir=NULL;
	scopePath.push_back(0);//全局作用域	
}

SymTab::~SymTab(){
    // 清除函数
    unordered_map<string, Fun*>::iterator funIt, funEnd = funTab.end();
    for(funIt = funTab.begin(); funIt != funEnd; ++funIt){
        delete funIt->second;
    }
    // 清除变量
    unordered_map<string, vector<Var*>*>::iterator varIt, varEnd=varTab.end();
    for(varIt = varTab.begin(); varIt != varEnd; ++varIt){
        vector<Var*> &list = *varIt->second;
        for(int i = 0; i < list.size(); i++){
            delete list[i];
        }
        delete &list;
    }
    //清除串
    unordered_map<string, Var*>::iterator strIt, strEnd = strTab.end();
    for(strIt = strTab.begin(); strIt != strEnd; strIt++){
        delete strIt->second;
    }
}

// 添加变量对象
void SymTab::addVar(Var* var){
    if(varTab.find(var->getName()) == varTab.end()){
        varTab[var->getName()] = new vector<Var*>; // 没有同名变量列表时创建
        varTab[var->getName()]->push_back(var); // 将变量放入这个名称的列表中  
        varlist.push_back(var->getName()); // 将变量名添加到变量链表中  
    }else{
        vector<Var *> &list = *varTab[var->getName()]; // 同名变量判断
        int i;
        for ( i = 0; i < list.size(); i++)
        {
            if(list[i]->getPath().back() == var->getPath().back()){// 判断作用域是否一致，若一致则报错
                break;
            }
        }
        if(i == list.size() || var->getName()[0] == '<'){ // 作用域不一致，或者为常量
            list.push_back(var);
        }else{
            SEMERROR(VAR_RE_DEF, var->getName());
            delete var;
            return;
        }
    }
    if(ir){
        int flag = ir->genVarInit(var); // 变量初始化语句
        if(curFun && flag) curFun->locate(var); // 计算局部变量栈帧偏移
    }
}

// 添加字符串常量
void SymTab::addStr(Var* v){
    // unordered_map<string, Var*>::iterator strIt, strEnd = strTab.end();
    // for(strIt = strTab.begin(); strIt != strEnd; ++strIt){// 判断字符串常量是否已存在
    //     Var* str = strIt->second;
    //     if(str->getStrVal() == v->getStrVal()){// 字符串常量已存在
    //         delete v;
    //         v = str;
    //         return;
    //     }
    // }
    strTab[v->getName()] = v;
}

// 获取变量
Var* SymTab::getVar(string name){
    Var* select = NULL;
    if(varTab.find(name) != varTab.end()){// 变量表中存在此变量
        vector<Var*> &list = *varTab[name]; // 得到此变量的同名变量列表
        int pathLen = scopePath.size(); // 获取当前作用域路径的长度
        int maxLen = 0; // 已经匹配的最大长度
        for(int i = 0; i < list.size(); i++){
            int len = list[i]->getPath().size(); // 获取当前遍历得到的变量的作用域路径长度
            if(len <= pathLen && list[i]->getPath()[len-1] == scopePath[len-1]){// 当此变量的作用域路径长度不超过当前作用域路径长度，且可以最长匹配
                if(len > maxLen){ // 变量作用域路径需要最长匹配
                /*
                    因为不同作用域变量可以同名，在更深的作用域中的变量可以覆盖作用域路径更浅的同名变量
                */
                    maxLen = len;
                    select = list[i];
                }
            }
        }
    }
    if(!select){// 若变量表中没有此变量或者当前作用域下没有此变量
        SEMERROR(VAR_UN_DEC, name);// 变量未定义错误
    }
    return select;
}

// 获取全局变量
vector<Var*> SymTab::getGlbVars(){
    vector<Var*> glbVars;
    for(int i = 0; i< varlist.size(); i++){
        string varName = varlist[i];
        if(varName[0] == '<') continue;// 忽略常量
        vector<Var*> &list = *varTab[varName];
        for(int j = 0; j < list.size(); j++){
            if(list[j]->getPath().size() == 1){
                glbVars.push_back(list[j]);
                break;// 同名情况下，全局变量只可能有一个
            }
        }
    }
    return glbVars;
}

// 声明函数
void SymTab::decFun(Fun* f){
    f->setExtern(true);
    if(funTab.find(f->getName()) == funTab.end()){ // 函数表中没有此函数
        funTab[f->getName()] = f; // 在函数表中添加函数
        funlist.push_back(f->getName());
    }else{// 函数表中有此函数，需要匹配检测
        Fun* existed = funTab[f->getName()];
        if(!existed->match(f)){
            SEMERROR(FUN_DEC_ERR, f->getName());// 不匹配则需要报声明错误
        }
        delete f;
    }
}

// 定义函数
void SymTab::defFun(Fun* fun){
    if(fun->getExtern()){// extern不能在定义出现
        SEMERROR(EXTERN_FUN_DEF,fun->getName());
        fun->setExtern(false);
    }
    if(funTab.find(fun->getName()) == funTab.end()){// 函数表中没有此函数
        funTab[fun->getName()] = fun;
        funlist.push_back(fun->getName());
    }else{
        Fun* last = funTab[fun->getName()]; // 已经声明过
        if(last->getExtern()){
            if(!last->match(fun)){// 不匹配声明
                SEMERROR(FUN_DEC_ERR,fun->getName());
            }
            last->define(fun); // 保存定义信息
        }else
            SEMERROR(FUN_RE_DEF, fun->getName());// 重复定义了
        delete fun; // 删除当前函数对象
        fun = last; // 公用函数体
    }
    curFun = fun;
    ir->genFunHead(curFun); // 产生函数入口
}

// 结束函数定义
void SymTab::endDefFun(){
    ir->genFunTail(curFun); // 产生函数出口
    curFun = NULL; // 当前函数定义结束，置为空
}

// 获取函数
Fun* SymTab::getFun(string name, vector<Var*>& args){
    if(funTab.find(name) != funTab.end()){ // 此函数存在在函数表中
        Fun* last = funTab[name];
        if(!last->match(args)){
            SEMERROR(FUN_CALL_ERR, name);
            return NULL;
        }
        return last;
    }
    SEMERROR(FUN_UN_DEC,name);//函数未声明
	return NULL;
}

// 获取当前函数
Fun* SymTab::getCurFun(){
    return curFun;
}

// 作用域进入
void SymTab::enter(){
    scopeId++;
    scopePath.push_back(scopeId);
    if(curFun)
        curFun->enterScope();
}
// 作用域退出
void SymTab::leave(){
    scopePath.pop_back();
    if(curFun)
        curFun->leaveScope();
}

//获取scopePath
vector<int> &SymTab::getScopePath(){
    return scopePath;
}

void SymTab::addInst(InterInst*inst){
    if(curFun) curFun->addInst(inst);
    else delete inst;
}

void SymTab::setIR(GenIR* ir){
    this->ir = ir;
}

// 输出信息
void SymTab::toString(){
    // 输出变量表
    printf("---------变量表---------\n");
    for(int i = 0; i < varlist.size(); i++){
        string varName = varlist[i];
        vector<Var*> &list = *varTab[varName];
        printf("%s:\n",varName.c_str());
        for(int j = 0; j < list.size(); j++){
            printf("\t");
            list[j]->toString();
            printf("\n");
        }
    }
    printf("----------串表-----------\n");
	unordered_map<string, Var*>::iterator strIt,strEnd=strTab.end();
	for(strIt=strTab.begin();strIt!=strEnd;++strIt)
		printf("%s=%s\n",strIt->second->getName().c_str(),strIt->second->getStrVal().c_str());
	printf("----------函数表----------\n");
	for(int i=0;i<funlist.size();i++){
		funTab[funlist[i]]->toString();
	}
}

// 输出中间代码
void SymTab::printInterCode(){
    for(int i = 0; i < funlist.size(); i++){
        funTab[funlist[i]]->printInterCode();
    }
}

// 生成数据段代码
void SymTab::genData(FILE *file){
    // 进行数据段的存储，生成常量字符串，.rodata段
    fprintf(file, ".section .rodata\n");
    // 常量字符串
    unordered_map<string, Var*>::iterator strIt,strEnd=strTab.end();
    for(strIt=strTab.begin(); strIt!=strEnd; strIt++){
        Var *str = strIt->second;
        fprintf(file, "%s:\n", str->getName().c_str());
        fprintf(file, "\t.ascii \"%s\"\n", str->getRawStr().c_str());
    }
    // 生成数据段和.bss段
    fprintf(file, ".data\n");
    vector<Var*> glbVars = getGlbVars();
    for(unsigned int i = 0; i < glbVars.size(); i++){
        Var* var = glbVars[i];
        fprintf(file, "\t.global %s\n", var->getName().c_str());
        if(!var->unInit()){// 如果变量被初始化了
            fprintf(file, "%s:\n", var->getName().c_str());// var:
            if(var->isBase()){
                const char* t = var->isChar() ? ".byte" : ".word";
                fprintf(file, "\t%s %d\n", t, var->getVal());// .byte  value   .word   value
            }else{// 字符指针初始化
                fprintf(file, "\t.word %s\n", var->getPtrVal().c_str());// .word    .L0
            }
        }else{ // 放在bss段
            fprintf(file, "\t.comm %s,%d\n", var->getName().c_str(), var->getSize());
        }
    }
}

// 输出汇编文件
void SymTab::genAsm(const char *fileName){
    string newName = fileName;
    int idx = newName.rfind(".hhx");
    if(idx>0 && idx==newName.length()-4){
        newName.replace(idx,4,".asm");
    }else{
        newName = newName+".asm";
    }


    FILE* file = fopen(newName.c_str(), "w"); // 创建输出文件

    // 生成数据段
    genData(file);
    // 生成代码段
    fprintf(file, ".text\n");
    for(int i=0; i<funlist.size(); i++){
        funTab[funlist[i]]->genAsm(file);
    }
    fclose(file);
}

/*
    MIPS生成
*/

void SymTab::genData_mips(FILE*file){
    // 进行数据段的存储，生成常量字符串，.rodata段
    // 常量字符串
    
    // 生成数据段和.bss段
    Var *var;
    Var *str;

    fprintf(file, ".data\n");
    vector<Var*> glbVars = getGlbVars();
    for(unsigned int i = 0; i < glbVars.size(); i++){
        var = glbVars[i];
        fprintf(file, "\t.globl %s\n", var->getName().c_str());
        if(!var->unInit()){// 如果变量被初始化了
            fprintf(file, "%s:\n", var->getName().c_str());// var:
            if(var->isBase()){
                const char* t = var->isChar() ? ".byte" : ".word";
                fprintf(file, "\t%s %d\n", t, var->getVal());// .byte  value   .word   value
            }else{// 字符指针初始化
                Var* tmpStr = strTab[var->getPtrVal()];
                fprintf(file, "\t.asciiz \"%s\"\n", tmpStr->getStrVal().c_str());
            }
        }else{ // 放在bss段
            fprintf(file, "%s:\t.space %d\n", var->getName().c_str(), var->getSize());// var:  .space  n
        }
    }
    unordered_map<string, Var*>::iterator strIt,strEnd=strTab.end();
    for(strIt=strTab.begin(); strIt!=strEnd; strIt++){
        bool flag = true;
        str = strIt->second;

        for(unsigned int i = 0; i < glbVars.size(); i++){
            flag = true;
            var = glbVars[i];
            if(str->getName() == var->getPtrVal()){
                flag = false;
                break;
            }
        }

        if(flag){
            fprintf(file, "%s:\n", str->getName().c_str());
            fprintf(file, "\t.asciiz \"%s\"\n", str->getStrVal().c_str());
        }
    }
}

void SymTab::genAsm_MIPS(const char* fileName){
    string newName = fileName;
    int idx = newName.rfind(".hhx");
    if(idx>0 && idx==newName.length()-4){
        newName.replace(idx,9,"_mips.asm");
    }else{
        newName = newName+"_mips.asm";
    }
    FILE* file = fopen(newName.c_str(), "w"); // 创建输出文件

    // 生成数据段
    genData_mips(file);
    // 生成代码段
    fprintf(file, ".text\n");

    fprintf(file,"\t.globl\tmain\n");
    for(int i=funlist.size()-1; i>=0; i--){
        string fName = funlist[i];
        funTab[fName]->genAsm_mips(file);
    }
    fclose(file);
}
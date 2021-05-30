#include "genir.h"
#include "symbol.h"
#include "symtab.h"
#include<sstream>
#include "error.h"

int GenIR::labelNum = 0;

GenIR::GenIR(SymTab& tab):symtab(tab){

}

GenIR::~GenIR(){
    
}

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

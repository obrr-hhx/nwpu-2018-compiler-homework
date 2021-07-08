#include "compiler.h"
#include "lexer.h"
#include "error.h"
#include "symtab.h"
#include "genir.h"
#include "parser.h"

bool Args::showChar=false;
bool Args::showToken=false;
bool Args::showSymTab=false;
bool Args::showIr=false;
bool Args::showOpIr=false;
bool Args::genArm=true;
bool Args::opt=false;
bool Args::showHelp=false;
bool Args::showVersion=false;
bool Args::removeFile=false;

void Compiler::compile(char* file){
    Scanner scanner(file);
    Error error(&scanner);
    Lexer lex(scanner);
    SymTab symtab;
    GenIR ir(symtab);
    Parser parser(lex,symtab,ir);

    parser.analyse();

    if(Error::getErrorNum()+Error::getWarnNum()) return; // 出错不进行

    //中间结果
    if(Args::showSymTab) symtab.toString();
    if(Args::showIr) symtab.printInterCode();

    //生成汇编
    if(Args::genArm)
        symtab.genAsm(file);
    else
        symtab.genAsm_MIPS(file);
}

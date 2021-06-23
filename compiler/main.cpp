#include"common.h"
#include"error.h"
#include"token.h"
#include"lexer.h"
#include"parser.h"
#include"symtab.h"
#include"symbol.h"
#include"genir.h"
#include"intercode.h"

string varietyTable[48] = {
    "ERR",																					//错误，异常
	"END",																					//文件结束标记
	"ID",																						//标识符
	"KW_INT","KW_CHAR","KW_VOID",												//数据类型
	"KW_EXTERN",																		//extern
	"NUM","CH","STR",																		//字面量
	"NOT","LEA",																			//单目运算 ! - & *
	"ADD","SUB","MUL","DIV","MOD",													//算术运算符
	"INC","DEC",
	"GT","GE","LT","LE","EQU","NEQU",													//比较运算符
	"AND","OR",																				//逻辑运算
	"LPAREN","RPAREN",																//()
	"LBRACK","RBRACK",																//[]
	"LBRACE","RBRACE",																//{}
	"COMMA","COLON","SEMICON",													//逗号,冒号,分号
	"ASSIGN",																				//赋值
	"KW_IF","KW_ELSE",																//if-else
	"KW_SWITCH","KW_CASE","KW_DEFAULT",									//swicth-case-deault
	"KW_WHILE","KW_DO","KW_FOR",												//循环
	"KW_BREAK","KW_CONTINUE","KW_RETURN"};

int main(int argc, char *argv[]){
    string file = "test.txt";
	cout<<file<<endl;
	const char* name = file.c_str();
	Scanner scanner(name);
	Lexer lex(scanner);
	// Lexer lex1(scanner1);
	Error error(&scanner);
	// Token* t = lex.tokenize();
	// cout<<varietyTable[t->tag]<<endl;
	// while(t->tag != END){
	// 	t = lex.tokenize();
	// 	cout<<varietyTable[t->tag]<<endl;
	// }
	SymTab symtab;
	GenIR ir(symtab);
	Parser parser(lex,symtab,ir);
	parser.analyse();
	symtab.toString();
	symtab.printInterCode();
}
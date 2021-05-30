#include"common.h"
#include"lexer.h"
#include"error.h"
#include "token.h"

int Error::errorNum = 0, Error::warnNum = 0;// 错误总个数，警告总个数
Scanner* Error::scanner = NULL;

Error::Error(Scanner *sc){
  scanner = sc;
}

int Error::getErrorNum(){
  return errorNum;
}

int Error::getWarnNum(){
  return warnNum;
}

// 词法错误
void Error::lexError(int code){
  //词法错误信息串
	static const char *lexErrorTable[]=
	{
		"字符串丢失右引号",
		"二进制数没有实体数据",
		"十六进制数没有实体数据",
		"字符丢失右单引号",
		"不支持空字符",
		"错误的或运算符",
		"多行注释没有正常结束",
		"词法记号不存在"
	};
	errorNum++;
	printf("%s<%d行> 词法错误 : %s.\n",scanner->getFileName(),scanner->getLineNum(),lexErrorTable[code]);
}

// 打印语法错误
void Error::synError(int code,Token*t)
{
	//语法错误信息串
	static const char *synErrorTable[]=
	{
		"类型",
		"标识符",
		"数组长度",
		"常量",
		"逗号",
		"分号",
		"=",
		"冒号",
		"while",
		"(",
		")",
		"[",
		"]",
		"{",
		"}"
	};
	errorNum++;
	if(code%2==0)//lost
		printf("%s<第%d行> 语法错误 : 在 %s 之前丢失 %s .\n",scanner->getFileName(),scanner->getLineNum(), t->tostring().c_str(),synErrorTable[code/2]);
	else//wrong
		printf("%s<第%d行> 语法错误 : 在 %s 处没有正确匹配 %s .\n",scanner->getFileName(),scanner->getLineNum(), t->tostring().c_str(),synErrorTable[code]);
}

// 打印语义错误
void Error::semError(int code, string name){
	//语义错误信息串
	static const char *semErrorTable[]=
	{
		"变量重定义",										//附加名称信息
		"函数重定义",
		"变量未声明",
		"函数未声明",
		"函数声明与定义不匹配",
		"函数行参实参不匹配",
		"变量声明时不允许初始化",
		"函数定义不能声明extern",
		"数组长度应该是正整数",
		"变量初始化类型错误",
		"全局变量初始化值不是常量",
		"变量不能声明为void类型",					//没有名称信息
		"无效的左值表达式",
		"赋值表达式类型不兼容",
		"表达式运算对象不能是基本类型",
		"表达式运算对象不是基本类型",
		"数组索引运算类型错误",
		"void的函数返回值不能参与表达式运算",
		"break语句不能出现在循环或switch语句之外",
		"continue不能出现在循环之外",
		"return语句和函数返回值类型不匹配"
	};
	errorNum++;
	printf("%s<第%d行> 语义错误 : %s %s.\n",scanner->getFileName(),scanner->getLineNum(),name.c_str(),semErrorTable[code]);
}

// 打印语义警告
void Error::semWarn(int code, string name){
		//语义警告信息串
	static const char *semWarnTable[]=
	{
		"函数参数列表类型冲突",					//附加名称信息
		"函数返回值类型不精确匹配"
	};
	warnNum++;
	printf("%s<第%d行> 语义警告 : %s %s.\n", scanner->getFileName(), scanner->getLineNum(), name.c_str(), semWarnTable[code]);
}

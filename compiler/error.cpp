#include"common.h"
#include"lexer.h"
#include"error.h"

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
	printf("%s<%d行> 词法错误 : %s.\n",scanner->getFileName(),
		scanner->getLineNum(),lexErrorTable[code]);
}

/*
	打印语法错误
*/
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
		printf("%s<第%d行> 语法错误 : 在 %s 之前丢失 %s .\n",scanner->getFileName(),scanner->getLineNum()
			,t->toString().c_str(),synErrorTable[code/2]);
	else//wrong
		printf("%s<第%d行> 语法错误 : 在 %s 处没有正确匹配 %s .\n",scanner->getFileName(),scanner->getLineNum()
			,t->toString().c_str(),synErrorTable[code]);
}

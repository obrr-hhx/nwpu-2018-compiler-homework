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

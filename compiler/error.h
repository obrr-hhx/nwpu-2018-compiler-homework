#pragma once
#include"common.h"

/******
 * 错误类
 */
class Error{
    static Scanner *scanner;
    public:
        Error(Scanner *sc);
        static int errorNum;//错误个数
	    static int warnNum;	//警告个数
	    //外界接口
	    static int getErrorNum();
	    static int getWarnNum();
	
	    //错误接口
	    static void lexError(int code);//打印词法错误
};

//错误级别,可选，用于修饰错误信息头部
#define FATAL "<fatal>:"
#define ERROR "<error>:"
#define WARN  "<warn>:"
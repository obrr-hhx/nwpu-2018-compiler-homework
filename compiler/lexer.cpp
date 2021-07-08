#include"common.h"
#include"lexer.h"
#include"token.h"
#include"error.h"
#include "compiler.h"

/*********************************************************************
 *                             扫描器
 * ******************************************************************/

Scanner::Scanner(const char* name){
    file = fopen(name,"r");
    if(!file){
        printf("文件%s打开失败\n",name);
        Error::errorNum++;
    }
    fileName = name;

    // 初始化
    lineLen = 0;
    charAtLine = -1;
    lineNum = 1;
    oldCh = 0;
}

Scanner::~Scanner(){
    if(file){
        printf("文件未读完\n");
        Error::warnNum++;
        fclose(file);
    }
}

void Scanner::showChar(char ch){
    if(ch == -1) printf("EOF");
    else if(ch == '\n') printf("\\n");
    else if(ch == '\t') printf("\\t");
    else if(ch== ' ') printf("<blank>");
    else cout<<ch;
    printf("\t\t<%d>\n",ch);
}

int Scanner::scan(){
    if(!file)
        return -1;
    if(charAtLine == lineLen-1){// 缓冲区读取完毕
        lineLen = fread(line, 1, BUFFLEN, file);// 加载缓冲区数据
        if(lineLen == 0){// 没有数据了
            lineLen = 1;// 不再往缓冲区加载数据
            line[0] = -1;// 文件结束标志
        }
        charAtLine = -1;// 重置读取字符位置
    }
    charAtLine++;
    char ch = line[charAtLine];
    oldCh = ch;
    if(oldCh == '\n'){
        lineNum++;
    }else if(oldCh == -1){
        fclose(file);
        file = NULL;
    }
    if(Args::showChar) showChar(ch);
    return ch;
}

const char* Scanner::getFileName(){
    return fileName;
}

FILE* Scanner::getFile(){
    return file;
}

int Scanner::getLineNum(){
    return lineNum;
}

/*************************************************************
 *                      保留字
 * **********************************************************/
Keywords::Keywords(){
    keywords["int"]=KW_INT;
	keywords["char"]=KW_CHAR;
	keywords["void"]=KW_VOID;
	keywords["extern"]=KW_EXTERN;
	keywords["if"]=KW_IF;
	keywords["else"]=KW_ELSE;
	keywords["switch"]=KW_SWITCH;
	keywords["case"]=KW_CASE;
	keywords["default"]=KW_DEFAULT;
	keywords["while"]=KW_WHILE;
	keywords["do"]=KW_DO;
	keywords["for"]=KW_FOR;
	keywords["break"]=KW_BREAK;
	keywords["continue"]=KW_CONTINUE;
	keywords["return"]=KW_RETURN;
}

Tag Keywords::getTag(string name){
    return keywords.find(name) != keywords.end() ? keywords[name] : ID;
}



/*************************************************************
 *                      词法分析器
 * *********************************************************/
Keywords Lexer::keywords;

string Lexer::varietyTable[48] = {
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
	"KW_BREAK","KW_CONTINUE","KW_RETURN"
};

Lexer::Lexer(Scanner &sc):scanner(sc){
    token = NULL;
    ch = ' ';
}

Lexer::~Lexer(){
    if(token){
        delete token;
    }
}

bool Lexer::scan(char need /*= 0*/){
    ch = scanner.scan();
    if(need){
        if(ch != need)
            return false;
        ch = scanner.scan();// 与预期吻合扫描下一个 
        return true;
    }
    return true;
}

#define LEXERROR(code) Error::lexError(code);

// 有限自动机匹配，词法记号解析

Token* Lexer::tokenize(){
    while(ch != -1){
        Token* t = NULL;
        // 遇到空格，换行，制表符，忽略
        while(ch == ' ' || ch == '\n' || ch == '\t') scan();

        // 识别标识符
        if(ch == '_' || (ch >= 'a'&& ch <= 'z') || (ch >= 'A'&&ch <= 'Z')){
            string name = "";
            do{
                name.push_back(ch);
                scan();
            }while((ch >= 'a'&&ch <= 'z') || (ch >= 'A'&&ch <='Z') || (ch >= '0'&&ch <= '9'));

            Tag tag = keywords.getTag(name);// 判断是否为保留字
            if(tag == ID) t = new Id(name);
            else t = new Token(tag);
        }else if(ch == '"'){// string
            string str;
            while(!scan('"')){
                if(ch == -1 || ch == '\n'){// 文件结束
                    LEXERROR(STR_NO_R_QUTION);
                    t = new Token(ERR);
                    break;
                }else if(ch == '\\'){// 转义字符
                    switch (ch)
                    {
                    case 'n':
                        str.push_back('\n');
                        break;
                    case 't':
                        str.push_back('\t');
                        break;
                    case '"':
                        str.push_back('"');
                        break;
                    case '0':
                        str.push_back('\0');
                        break;
                    case '\n':
                        break;
                    case -1:
                        LEXERROR(STR_NO_R_QUTION);
                        t = new Token(ERR);
                        break; 
                    default:
                        str.push_back(ch);
                        break;
                    }
                }else
                    str.push_back(ch);
            }
            if(!t) t = new Str(str);
        }else if(ch >= '0' && ch <= '9'){// number
            int number = 0;
            do{
                number = number*10 + ch-'0';
                scan();
            }while(ch >= '0' && ch <= '9');
            if(!t) t=new Num(number);
        }else if(ch == '\''){// character
            char c;
            scan();
            if(ch == '\\'){// 转义
                scan();
                if(ch == 'n') c='\n';
                else if(ch == 't') c='\t';
                else if(ch == '0') c='\0';
                else if(ch == '\\') c='\\';
                else if(ch == -1 || ch == '\n'){
                    LEXERROR(CHAR_NO_R_QUTION);
                    t = new Token(ERR);
                }else
                    c = ch;
            }else if(ch == -1 || ch == '\n'){
                LEXERROR(CHAR_NO_R_QUTION);
                t = new Token(ERR);
            }else if(ch == '\''){
                LEXERROR(CHAR_NO_DATA);
                t = new Token(ERR);
            }else
                c = ch;
            if(scan('\'')){// 如果字符存在右引号
                if(!t) t=new Chara(c);
            }else{
                LEXERROR(CHAR_NO_R_QUTION);
                t = new Token(ERR);
            }
        }else{// 界符
            switch (ch)
            {
            case '#':
                while(ch != -1 || ch != '\n'){
                    scan();
                }
                t = new Token(ERR);
                break;
            case '+':
                t = new Token(scan('+')?INC:ADD);
                break;
            case '-':
                t = new Token(scan('-')?DEC:SUB);
                break;
            case '!':
                t = new Token(scan('=')?NEQU:NOT);
                break;
            case '*':
                t = new Token(MUL);
                scan();
                break;
            case '/':
                if(scan('/')){// 单行注释
                    while(ch != '\n' && ch != -1) scan();
                    t = new Token(ERR);
                }else if(ch == '*'){// 多行注释
                   while (!scan(-1))// 没有结束就一直读
                   {
                       if(ch == '*'){
                           if(scan('/')) break;
                       }
                   }
                   if(ch == -1){
                       LEXERROR(COMMENT_NO_END);
                       t = new Token(ERR);
                   }
                }else
                    t = new Token(DIV);
                break;
            case '%':
                t = new Token(MOD);
                scan();
                break;
            case '>':
                t = new Token(scan('=')?GE:GT);
                break;
            case '<':
                t = new Token(scan('=')?LE:LT);
                break;
            case '=':
                t = new Token(scan('=')?EQU:ASSIGN);
                break;
            case '&':
                t = new Token(scan('&')?AND:LEA);
                break;
            case '|':
                t = new Token(scan('|')?OR:ERR);
                if(t->tag == ERR) LEXERROR(OR_NO_PAIR);
                break;
            case ',':
				t=new Token(COMMA);scan();break;
            case ':':
                t=new Token(COLON);scan();break;
            case ';':
                t=new Token(SEMICON);scan();break;
            case '(':
                t=new Token(LPAREN);scan();break;
            case ')':
                t=new Token(RPAREN);scan();break;
            case '[':
                t=new Token(LBRACK);scan();break;
            case ']':
                t=new Token(RBRACK);scan();break;
            case '{':
                t=new Token(LBRACE);scan();break;
            case '}':
                t=new Token(RBRACE);scan();break;
            case -1:// 文件结束
                break;
            default:
                LEXERROR(TOKEN_NO_EXIST);
                t = new Token(ERR);
                scan();
                break;
            }
        }
        
        if(token) delete token;
        token = t;
        if(token && token->tag != ERR)
            return token;
        else
            continue;
    }
    // 文件结束
    if(token) delete token;
    return token = new Token(END);
}















#include"common.h"
#include"error.h"
#include"token.h"
#include"lexer.h"
#include"parser.h"
#include"symtab.h"
#include"symbol.h"
#include"genir.h"
#include"intercode.h"
#include "compiler.h"
#include <sys/stat.h>

int main(int argc, char *argv[]){
    vector<char*> srcFiles;
	if(argc>1){
		for(int i=1; i<argc-1; i++){
			srcFiles.push_back(argv[i]);
		}
		char* opt=argv[argc-1]; // 最后一个参数
		if(!strcmp(opt,"-char")) Args::showChar=true;
		else if(!strcmp(opt,"-token")) Args::showToken=true;
		else if(!strcmp(opt,"-symbol")) Args::showSymTab=true;
		else if(!strcmp(opt,"-ir")) Args::showIr=true;
		else if(!strcmp(opt,"-or")) {Args::opt=true;Args::showOpIr=true;}
		else if(!strcmp(opt,"-o")) Args::opt=true;
		else if(!strcmp(opt,"-h")) Args::showHelp=true;
		else if(!strcmp(opt,"-mips")) Args::genArm=false;
		else if(!strcmp(opt,"-v")) Args::showVersion=true;
		else if(!strcmp(opt,"-rm")) Args::removeFile=true;
		else srcFiles.push_back(opt);
	}
	if(Args::showHelp){
		printf("This is help information\n");
		printf("Default Assembly is ARM\n");
		printf("Command Format: ./hhx filename.hhx flag\n");
		printf("flag:\n");
		printf("\t-o\t\t#execute optimization\n");
		printf("\t-char\t\t#display the character\n");
		printf("\t-token\t\t#display lexical token\n");
		printf("\t-symbol\t\t#display the table of symbol\n");
		printf("\t-ir\t\t#display the intermediate code\n");
		printf("\t-or\t\t#display the intermediate code after executing optimization\n");
		printf("\t-mips\t\t#generate MIPS code\n");
		printf("\t-h\t\t#display the help information\n");
		printf("\t-v\t\t#display the version of hhx\n");
		printf("\t-rm\t\t#delete file that is not hhx\n");
	}
	else if(Args::showVersion){
		printf("hhx 0.0.0\n");
		printf("Will have new version soon\n");
	}
	else if(Args::removeFile){
		string tmpFile;
		for(int i=0; i<srcFiles.size();i++){
			tmpFile=srcFiles[i];
			if(tmpFile.rfind(".hhx")==string::npos && tmpFile.rfind(".cpp")==string::npos && tmpFile.rfind(".h")==string::npos){
				// 后缀非hhx,cpp,h才可以删除
				remove(srcFiles[i]);
			}
			else{
				cout<<"you want to delete\t"<<tmpFile<<"?"<<endl;
				cout<<"THAT IS IMPOSSIBLE!!!"<<endl;
			}
		}
	}
	else if(srcFiles.size()){
		Compiler compiler;
		for(int i=0; i<srcFiles.size(); i++){
			compiler.compile(srcFiles[i]);
		}
		int error=Error::getErrorNum();
		int warn=Error::getWarnNum();
		cout<<"compile finished! error="<<error<<" warning="<<warn<<endl;
	}else{
		cout<<"command format wrong!"<<endl;
		cout<<"please use ./hhx -h to find help"<<endl;
	}
	return 0;
}
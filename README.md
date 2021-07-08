# nwpu-2018-compiler-homework
## 简介

西北工业大学2018级编译原理试点班大作业

一个很简单的编译器，编译后缀为.hhx的文件代码，支持多文件编译

语法为类似c0的语法，详细信息请查看[语法分析介绍](https://github.com/obrr-hhx/nwpu-2018-compiler-homework/blob/master/%E8%AF%AD%E6%B3%95%E5%88%86%E6%9E%90.txt)

生成的汇编为arm和mips

运行平台 OS X

---

## 运行方式

下载代码后编译即可运行

```bash
$ git clone https://github.com/obrr-hhx/nwpu-2018-compiler-homework.git
$ cd ./compiler
$ make
```

编译完成之后会生成hhx可执行文件

执行hhx的命令格式如下

```bash
$ ./hhx -h #显示帮助信息
This is help information
Default Assembly is ARM
Command Format: ./hhx filename.hhx flag
flag:
	-o		#execute optimization
	-char		#display the character
	-token		#display lexical token
	-symbol		#display the table of symbol
	-ir		#display the intermediate code
	-or		#display the intermediate code after executing optimization
	-mips		#generate MIPS code
	-h		#display the help information
	-v		#display the version of hhx
	-rm		#delete file that is not hhx
```



### MIPS

生成的*_mips.asm汇编文件可以使用mars来仿真，以此来验证正确性，关于mips的介绍请参考[quick start](https://minnie.tuhs.org/CompArch/Resources/mips_quick_tutorial.html)

### ARM

本编译器默认生成arm汇编，*.asm的就是arm，如果想要仿真的话，可以直接买一个树莓派的板子，当然，也可以用qemu模拟环境，关于如何使用qemu模拟树莓派环境请看[这里](https://azeria-labs.com/emulate-raspberry-pi-with-qemu/)

tips:

​	如果是windows，建议使用WSL来运行，非常方便

​	由于时间原因，还没有写优化部分，也没有进行连接生成可执行文件，以后有时间会做的

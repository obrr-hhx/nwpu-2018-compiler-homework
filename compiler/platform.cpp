#include "platform.h"

const char* Plat::regName[regNum]={
    "r0","r1","r2","r3","r4","r5","r6","r7"
};
const char* Plat::regName_mips[regNum_mips]={
    "$t0","$t1","$t2","$t3","$t4","$t5","$t6","$t7","$t8","t9"
};

/*
    循环左移两位
*/
void Plat::roundLeftShiftTwoBit(unsigned int& num){
    unsigned int overFlow=num & 0xc0000000; // 取左移即将溢出的两位
    num=(num<<2) | (overFlow>>30); // 将溢出部分追加到尾部
}

// 判断num是否是常量表达式，8位数字循环右移偶数位得到
bool Plat::__constExpr(unsigned int num){
    for(int i=0; i<16; i++){
        if(num<=0xff) return true; // 有效位图
        else roundLeftShiftTwoBit(num); // 循环左移2位
    }

}

// 同时处理正数和负数
bool Plat::constExpr(int num){
    return __constExpr(num) || __constExpr(-num);
}

// 判断是否是合法的偏移
bool Plat::isDisp(int num){
    return num<4096 && num>-4096;
}

// 判断是否是合法的寄存器名
bool Plat::isReg(string s){
    return s=="r0" || s=="r1" || s=="r2" || s=="r3" || s=="r4"
			|| s=="r5" || s=="r6" || s=="r7" || s=="r8" || s=="r9"
			|| s=="r10" || s=="fp" || s=="ip" || s=="sp" || s=="lr"
			|| s=="pc";
}
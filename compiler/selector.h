#pragma once
#include "common.h"
#include "iloc.h"
#include "mips.h"

/*
    指令选择器
*/
class Selector{
    vector<InterInst*>& ir;
    ILoc& iloc;
    InstMips& instMips;
    void translate(InterInst* inst);
    void translate_mips(InterInst* inst);
public:
    Selector(vector<InterInst*>& ir, ILoc& iloc, InstMips& instMips);
    void select(bool arm);
};
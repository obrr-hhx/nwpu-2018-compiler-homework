#pragma once
#include "common.h"
#include "iloc.h"

/*
    指令选择器
*/
class Selector{
    vector<InterInst*>& ir;
    ILoc& iloc;
    void translate(InterInst* inst);
public:
    Selector(vector<InterInst*>& ir, ILoc& iloc);
    void select();
};
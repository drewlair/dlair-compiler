#include "../include/scratch.h"

int regNum[5] = {1,1,1,1,1};

int paramNum = 0;

int scratch_alloc(){
    int i = 0;
    while (i < 5){
        if (regNum[i] == 1){
            regNum[i] = 0;
            return i;
        }
        else{
            i += 1;
        }
    }
    return -1;
}

void scratch_free( int reg ){
    regNum[reg] = 1;
    return;
}

const char *scratch_name( int reg ){
    switch(reg){
        case RBX:
            return "%rbx";
            break;
        case R12:
            return "%r12";
            break;
        case R13:
            return "%r13";
            break;
        case R14:
            return "%r14";
            break;
        case R15:
            return "%r15";
            break;
        default:
            return "\0";
            break;
    }
}

int param_allocate(){
    if (paramNum == 4){
        return -1;
    }
    return paramNum++;
}

void param_free(){
    paramNum = 0;
}

const char *param_name( int reg ){
    switch(reg){
        case RDI:
            return "%rdi";
            break;
        case RSI:
            return "%rsi";
            break;
        case RDX:
            return "%rdx";
            break;
        case RCX:
            return "%rcx";
            break;
    }
    return "\0";
}
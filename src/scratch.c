#include "../include/scratch.h"
#include <stdio.h>
#include <string.h>
int regNum[5] = {1,1,1,1,1};

int paramNum = 0;

int fLabel = 0;

int floatRegisters[8] = {1,1,1,1,1,1,1,1};

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

int float_allocate(){
    int i = 0;
    while (i < 8){
        if (floatRegisters[i] == 1){
            floatRegisters[i] = 0;
            return i;
        }
        i += 1;
    }
    return -1;
}

void float_free( int reg ){
    floatRegisters[reg] = 1;
}

const char *float_name( int reg ){
    switch(reg){
        case 0:
            return "%xmm0";
            break;
        case 1:
            return "%xmm1";
            break;
        case 2:
            return "%xmm2";
            break;
        case 3:
            return "%xmm3";
            break;
        case 4:
            return "%xmm4";
            break;
        case 5:
            return "%xmm5";
            break;
        case 6:
            return "%xmm6";
            break;
        case 7:
            return "%xmm7";
            break;
        default:
            return "\0";
    }
}

const char *float_label(){
    char str[10];
    sprintf(str, "F%d", fLabel);
    fLabel++;
    return strdup(str);
}




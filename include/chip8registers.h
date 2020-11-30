//
// Created by Paltrickontpb on 27-11-2020.
//
#include "config.h"

#ifndef _CHIP8REGISTERS_H_
#define _CHIP8REGISTERS_H_

struct chip8_registers {
    unsigned char V[CHIP8_DATAREG_COUNT];
    unsigned short I;
    unsigned char dt, st;
    unsigned short PC;
    unsigned char SP;
};

#endif //_CHIP8REGISTERS_H_

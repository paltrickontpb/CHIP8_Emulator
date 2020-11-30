//
// Created by Paltrickontpb  on 26-11-2020.
//

#ifndef _CHIP8_H_
#define _CHIP8_H_

#include "config.h"
#include "chip8memory.h"
#include "chip8registers.h"
#include "chip8stack.h"
#include "chip8keyboard.h"
#include "chip8screen.h"

struct chip8 {
    struct chip8_memory memory;
    struct chip8_registers registers;
    struct chip8_stack stack;
    struct chip8_keyboard keyboard;
    struct chip8_screen screen;
};

void chip8_init(struct chip8 *chip);

#endif //_CHIP8_H_

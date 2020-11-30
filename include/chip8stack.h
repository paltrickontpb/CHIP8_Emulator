//
// Created by Paltrickontpb on 27-11-2020.
//

#ifndef _CHIP8STACK_H_
#define _CHIP8STACK_H_
#include "config.h"

struct chip8;
struct chip8_stack{
    unsigned short stack[CHIP8_STACK_SIZE];
};

void chip8_stack_push(struct chip8 *chip8, unsigned short val);
unsigned short chip8_stack_pop(struct chip8 *chip8);

#endif //CHIP8_CHIP8STACK_H

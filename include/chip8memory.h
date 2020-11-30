//
// Created by Paltrickontpb on 27-11-2020.
//

#ifndef _CHIP8MEMORY_H_
#define _CHIP8MEMORY_H_
#include "config.h"

struct chip8_memory{
    unsigned char memory[CHIP8_MEMORY_SIZE];
};

void chip8_memory_set(struct chip8_memory *memory, int index, unsigned char val);
unsigned char chip8_memory_get(struct chip8_memory *memory, int index);

#endif //_CHIP8MEMORY_H_
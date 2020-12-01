//
// Created by Paltrickontpb on 27-11-2020.
//
#include "chip8memory.h"
#include "assert.h"

static void chip8_is_mem_in_range(int index){
    assert(index >= 0 && index < CHIP8_MEMORY_SIZE);
}

void chip8_memory_set(struct chip8_memory *memory, int index, unsigned char val){
    chip8_is_mem_in_range(index);
    memory->memory[index] = val;
}
unsigned char chip8_memory_get(struct chip8_memory *memory, int index){
    chip8_is_mem_in_range(index);
    return memory->memory[index];
}

unsigned short chip8_memory_get_short(struct chip8_memory *memory, int index){
    unsigned char b1 = chip8_memory_get(memory, index);
    unsigned char b2 = chip8_memory_get(memory, index+1);
    return (b1<<8 | b2);
}


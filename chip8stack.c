//
// Created by Paltrickontpb on 27-11-2020.
//

#include "chip8.h"
#include "chip8stack.h"
#include "assert.h"

static void chip8_is_sp_in_range(struct chip8 *chip8){
    assert(chip8->registers.SP < CHIP8_STACK_SIZE);
}

void chip8_stack_push(struct chip8 *chip8, unsigned short val){
    chip8_is_sp_in_range(chip8);
    chip8->stack.stack[chip8->registers.SP] = val;
    chip8->registers.SP += 1;
}

unsigned short chip8_stack_pop(struct chip8 *chip8){
    chip8->registers.SP -= 1;
    chip8_is_sp_in_range(chip8);
    return chip8->stack.stack[chip8->registers.SP];
}
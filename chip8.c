//
// Created by Paltrickontpb on 29-11-2020.
//

#include "chip8.h"
#include "memory.h"
#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "SDL/SDL.h"

const char chip8_default_character_set[] = {
        0xf0, 0x90, 0x90, 0x90, 0xf0,
        0x20, 0x60, 0x20, 0x20, 0x70,
        0xf0, 0x10, 0xf0, 0x80, 0xf0,
        0xf0, 0x10, 0xf0, 0x10, 0xf0,
        0x90, 0x90, 0xf0, 0x10, 0x10,
        0xf0, 0x80, 0xf0, 0x10, 0xf0,
        0xf0, 0x80, 0xf0, 0x90, 0xf0,
        0xf0, 0x10, 0x20, 0x40, 0x40,
        0xf0, 0x90, 0xf0, 0x90, 0xf0,
        0xf0, 0x90, 0xf0, 0x10, 0xf0,
        0xf0, 0x90, 0xf0, 0x90, 0x90,
        0xe0, 0x90, 0xe0, 0x90, 0xe0,
        0xf0, 0x80, 0x80, 0x80, 0xf0,
        0xe0, 0x90, 0x90, 0x90, 0xe0,
        0xf0, 0x80, 0xf0, 0x80, 0xf0,
        0xf0, 0x80, 0xf0, 0x80, 0x80
};

void chip8_init(struct chip8 *chip8){
    memset(chip8, 0, sizeof(struct chip8));
    memcpy(&chip8->memory.memory, chip8_default_character_set, sizeof(chip8_default_character_set));
}

void chip8_load(struct chip8 *chip8, const char* buf, size_t size){
    assert((size+CHIP8_PROG_LD_ADDR) < CHIP8_MEMORY_SIZE);
    memcpy(&chip8->memory.memory[CHIP8_PROG_LD_ADDR], buf, size);
    chip8->registers.PC = CHIP8_PROG_LD_ADDR;
}

static void chip8_exec_arithmetic(struct chip8 *chip8, unsigned short opcode){
    unsigned char x = (opcode >> 8) & 0x000F;
    unsigned char y = (opcode >> 4) & 0x000F;
    unsigned char kk = opcode & 0x0FF;
    unsigned char n = opcode & 0x000F;
    unsigned short temp = 0;

    switch(n){
        case 0x00:
            chip8->registers.V[x] = chip8->registers.V[y];
            break;
        case 0x01:
            chip8->registers.V[x] |= chip8->registers.V[y];
            break;
        case 0x02:
            chip8->registers.V[x] &= chip8->registers.V[y];
            break;
        case 0x03:
            chip8->registers.V[x] ^= chip8->registers.V[y];
            break;
        case 0x04:
            temp = chip8->registers.V[x] + chip8->registers.V[y];
            chip8->registers.V[0x0F] = false;
            if (temp < 0xff) chip8->registers.V[0x0F] = true;
            chip8->registers.V[x] = temp;
            break;
        case 0x05:
            chip8->registers.V[0x0F] = false;
            if (chip8->registers.V[x] > chip8->registers.V[y]) chip8->registers.V[0x0F] = true;
            chip8->registers.V[x] -= chip8->registers.V[y];
            break;
        case 0x06:
            chip8->registers.V[0x0F] = 0x01 & chip8->registers.V[x];
            chip8->registers.V[x] /= 2;
            break;
        case 0x07:
            chip8->registers.V[0x0F] = false;
            if (chip8->registers.V[x] < chip8->registers.V[y]) chip8->registers.V[0x0F] = true;
            chip8->registers.V[x] = chip8->registers.V[y] - chip8->registers.V[x];
            break;
        case 0x0E:
            chip8->registers.V[0x0F] = chip8->registers.V[x] & 0b10000000;
            chip8->registers.V[x] *= 2;
            break;
    }
}

static char wait_for_keypress(struct chip8 *chip8){
    SDL_Event event;
    while(SDL_WaitEvent(&event)){
        if(event.type != SDL_KEYDOWN) continue;
        char c = event.key.keysym.sym;
        char chip8_key = chip8_keyboard_map(&chip8->keyboard, c);
        if (chip8_key != -1) return chip8_key;
        return -1;
    }
}

static void chip8_exec_hwmanip(struct chip8 *chip8, unsigned short opcode){
    unsigned char x = (opcode >> 8) & 0x000F;
    switch (opcode & 0x00ff){
        case 0x07:
            chip8->registers.V[x] = chip8->registers.dt;
            break;
        case 0x0A:
            char pressed_key = wait_for_keypress(chip8);
            chip8->registers.V[x] = pressed_key;
            break;
        case 0x15:
            chip8->registers.dt = chip8->registers.V[x];
            break;
        case 0x18:
            chip8->registers.st = chip8->registers.V[x];
            break;
        case 0x1E:
            chip8->registers.I += chip8->registers.V[x];
            break;
        case 0x29:
            chip8->registers.I = chip8->registers.V[x] * 5;
            break;
        case 0x33:
            chip8->memory.memory[chip8->registers.I] = chip8->registers.V[x] / 100;
            chip8->memory.memory[chip8->registers.I + 1] = chip8->registers.V[x] / 10 % 10;
            chip8->memory.memory[chip8->registers.I + 2] = chip8->registers.V[x] % 10;
            break;
        case 0x55:
            for (int i=0; i<=x; i++) chip8->memory.memory[chip8->registers.I + i] = chip8->registers.V[i];
            break;
        case 0x65:
            for (int i=0; i<=x; i++) chip8->registers.V[i] = chip8->memory.memory[chip8->registers.I + i];
            break;
    }
}

static void chip8_adv_opcode_exec(struct chip8 *chip8, unsigned short opcode){
    unsigned short nnn = opcode & 0x0FFF;
    unsigned char x = (opcode >> 8) & 0x000F;
    unsigned char y = (opcode >> 4) & 0x000F;
    unsigned char kk = opcode & 0x0FF;
    unsigned char n = opcode & 0x000F;

    switch(opcode & 0xF000){
        case 0x1000: // Jump
            chip8->registers.PC = nnn;
            break;
        case 0x2000: // Call Sub routine
            chip8_stack_push(chip8, chip8->registers.PC);
            chip8->registers.PC = nnn;
            break;
        case 0x3000: //SE Immediate
            if(chip8->registers.V[x] == kk) chip8->registers.PC += 2;
            break;
        case 0x4000: //SNE Immediate
            if(chip8->registers.V[x] != kk) chip8->registers.PC += 2;
            break;
        case 0x5000: //SE Registers
            if(chip8->registers.V[x] == chip8->registers.V[y]) chip8->registers.PC += 2;
            break;
        case 0x6000: //LD
            chip8->registers.V[x] = kk;
            break;
        case 0x7000: //LD Add
            chip8->registers.V[x] += kk;
            break;
        case 0x8000: //Arithmetic Opcodes Register
            chip8_exec_arithmetic(chip8, opcode);
            break;
        case 0x9000:
            if (chip8->registers.V[x] != chip8->registers.V[y]) chip8->registers.PC += 2;
            break;
        case 0xA000:
            chip8->registers.I = nnn;
            break;
        case 0xB000:
            chip8->registers.PC = chip8->registers.V[0] + nnn;
            break;
        case 0xC000:
            srand(clock());
            chip8->registers.V[x] = (rand() % 255) & kk;
            break;
        case 0xD000:
            const char *sprite = (const char*)&chip8->memory.memory[chip8->registers.I];
            chip8->registers.V[0x0f] = chip8_screen_draw_sprite(&chip8->screen, chip8->registers.V[x], chip8->registers.V[y], sprite, n);
            break;
        case 0xE000:
            switch(opcode & 0x00FF){
                case 0x9E:
                    if (chip8_is_keyboard_down(&chip8->keyboard, chip8->registers.V[x])) chip8->registers.PC += 2;
                    break;
                case 0xA1:
                    if (!chip8_is_keyboard_down(&chip8->keyboard, chip8->registers.V[x])) chip8->registers.PC += 2;
                    break;
            }
            break;
        case 0xF000:
            chip8_exec_hwmanip(chip8, opcode);
            break;

    }

}

void chip8_exec(struct chip8 *chip8, unsigned short opcode){
    switch(opcode){
        case 0x00E0: //Clear Display
            chip8_clear_screen(&chip8->screen);
            break;
        case 0x00EE: //Return from Subroutine
            chip8->registers.PC = chip8_stack_pop(chip8);
            break;
        default:
            chip8_adv_opcode_exec(chip8, opcode);
            break;
    }
}
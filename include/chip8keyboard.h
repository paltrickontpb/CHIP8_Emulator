//
// Created by Paltrickontpb on 27-11-2020.
//

#ifndef _CHIP8KEYBOARD_H_
#define _CHIP8KEYBOARD_H_
#include "config.h"
#include "stdbool.h"

struct chip8_keyboard{
    bool keyboard[KEYBOARD_SIZE];
    const char *keyboard_mapp;
};

void chip8_keyboard_set_map(struct chip8_keyboard *keyboard, const char *map);
int chip8_keyboard_map(struct chip8_keyboard *keyboard, char key);
void chip8_keyboard_down(struct chip8_keyboard *keyboard, int key);
void chip8_keyboard_up(struct chip8_keyboard *keyboard, int key);
bool chip8_is_keyboard_down(struct chip8_keyboard *keyboard, int key);

#endif //_CHIP8KEYBOARD_H_

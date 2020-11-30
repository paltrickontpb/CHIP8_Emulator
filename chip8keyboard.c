//
// Created by Paltrickontpb on 27-11-2020.
//
#include "chip8keyboard.h"

static void key_in_bound(int key){
    assert(key>=0 && key<KEYBOARD_SIZE);
}

int chip8_keyboard_map(const char *map, char key){
    for (int i=0; i<KEYBOARD_SIZE; i++){
        if(map[i] == key) return i;
    }
    return -1;
}

void chip8_keyboard_down(struct chip8_keyboard *keyboard, int key){
    //key_in_bound(key);
    keyboard->keyboard[key] = true;
}

void chip8_keyboard_up(struct chip8_keyboard *keyboard, int key){
    //key_in_bound(key);
    keyboard->keyboard[key] = false;
}

bool chip8_is_keyboard_down(struct chip8_keyboard *keyboard, int key){
    //key_in_bound(key);
    return keyboard->keyboard[key];
}
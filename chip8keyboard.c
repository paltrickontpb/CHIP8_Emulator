//
// Created by Paltrickontpb on 27-11-2020.
//
#include "chip8keyboard.h"
#include<assert.h>

static void key_in_bound(int key){
    assert(key>=0 && key<KEYBOARD_SIZE);
}

void chip8_keyboard_set_map(struct chip8_keyboard *keyboard, const char *map){
    keyboard->keyboard_mapp = map;
}


int chip8_keyboard_map(struct chip8_keyboard *keyboard, char key){
    for (int i=0; i<KEYBOARD_SIZE; i++){
        if(keyboard->keyboard_mapp[i] == key) return i;
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
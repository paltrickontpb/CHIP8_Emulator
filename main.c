#include <stdio.h>
#include <stdbool.h>
#include "SDL/SDL.h"
#include "chip8.h"
#include <Windows.h>

const char keyboard_mapp[KEYBOARD_SIZE] = {
        SDLK_0, SDLK_1, SDLK_2, SDLK_3,
        SDLK_4, SDLK_5, SDLK_6, SDLK_7,
        SDLK_8, SDLK_9, SDLK_a, SDLK_b,
        SDLK_c, SDLK_d, SDLK_e, SDLK_f
};

int main(int argc, char **argv) {

    struct chip8 chip8;
    chip8_init(&chip8);
    chip8.registers.st = 30;
    chip8_screen_draw_sprite(&chip8.screen, 62, 0, &chip8.memory.memory[0x00], 5);

    SDL_Init(SDL_INIT_EVERYTHING);
    SDL_Window *window = SDL_CreateWindow(
            EMULATOR_WINDOW_TITLE,
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            CHIP8_WIDTH * CHIP8_SCALE_UP,
            CHIP8_HEIGHT * CHIP8_SCALE_UP,
            SDL_WINDOW_SHOWN
            );

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_TEXTUREACCESS_TARGET );
    while(1){
        SDL_Event event;
        while(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_QUIT:
                    goto out;
                    break;
                case SDL_KEYDOWN:
                    int pkey = chip8_keyboard_map(&keyboard_mapp, event.key.keysym.sym);
                    if (pkey != -1) chip8_keyboard_down(&chip8.keyboard, pkey);
                    break;
                case SDL_KEYUP:
                    int upkey = chip8_keyboard_map(&keyboard_mapp, event.key.keysym.sym);
                    if (upkey != -1) chip8_keyboard_down(&chip8.keyboard, upkey);
                    break;
            }
        }

        SDL_SetRenderDrawColor(renderer, 0,0,0,0);
        SDL_RenderClear(renderer);
        SDL_SetRenderDrawColor(renderer, 255,255,255,0);


        for(int x=0; x<CHIP8_WIDTH; x++){
            for(int y=0; y<CHIP8_HEIGHT; y++){
                if (chip8_screen_is_set(&chip8.screen,x,y)) {
                    SDL_Rect r;
                    r.x = x * CHIP8_SCALE_UP;
                    r.y = y * CHIP8_SCALE_UP;
                    r.h = CHIP8_SCALE_UP;
                    r.w = CHIP8_SCALE_UP;
                    SDL_RenderFillRect(renderer, &r);
                }
            }
        }
        SDL_RenderPresent(renderer);

        if(chip8.registers.dt > 0) {
            Sleep(100);
            chip8.registers.dt -= 1;
        }

        if(chip8.registers.st > 0) {
            Beep(15000, 100 * chip8.registers.st);
            chip8.registers.st -= 1;
        }

    }

    out:
        SDL_DestroyWindow(window);
        return 0;
}

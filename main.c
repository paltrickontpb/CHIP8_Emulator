#include <stdio.h>
//#include <stdbool.h>
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

    if(argc < 2){
        printf("Please enter a File Name! \n");
        return -1;
    }

    const char *filename = argv[1];
    printf("File Passed to Arg : %s\n", filename);

    FILE *f = fopen(filename, "rb");

    if(!f){
        printf("Failed to load %s\n", filename);
        fclose(f);
        return -1;
    }

    fseek(f, 0, SEEK_END);
    long size = ftell(f);
    fseek(f, 0, SEEK_SET);
    printf("Size of file : %lu\n", size);

    char buf[CHIP8_MEMORY_SIZE-CHIP8_PROG_LD_ADDR];
    int result = fread(buf, size, 1, f);

    if(result != 1){
        printf("Data not readable! :(\n");
        fclose(f);
        return -1;
    }

    struct chip8 chip8;
    chip8_init(&chip8);
    chip8_load(&chip8, &buf, size);
    chip8_keyboard_set_map(&chip8.keyboard, keyboard_mapp);

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
                    //break;
                case SDL_KEYDOWN:
                    int pkey = chip8_keyboard_map(&chip8.keyboard, event.key.keysym.sym);
                    if (pkey != -1) chip8_keyboard_down(&chip8.keyboard, pkey);
                    break;
                case SDL_KEYUP:
                    int upkey = chip8_keyboard_map(&chip8.keyboard, event.key.keysym.sym);
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
            Sleep(1);
            chip8.registers.dt -= 1;
        }

        if(chip8.registers.st > 0) {
            Beep(12000, 10 * chip8.registers.st);
            chip8.registers.st -= 1;
        }

        unsigned short opcode = chip8_memory_get_short(&chip8.memory, chip8.registers.PC);
        chip8.registers.PC += 2;
        chip8_exec(&chip8, opcode);

    }

    out:
        SDL_DestroyWindow(window);
        return 0;
}

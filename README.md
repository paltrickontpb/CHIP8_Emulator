# CHIP8_Emulator
An emulator for Chip8 roms. The project emulates the chip8 hardware, interprets the chip8 language and runs the program on a SDL2 front end screen

![Emulator Screenshot](https://github.com/paltrickontpb/CHIP8_Emulator/blob/main/chip8_ss.png "Emulator Screenshot")

Main techinical reference : [](http://devernay.free.fr/hacks/chip8/C8TECH10.HTM)

## Overview 
The Chip8 emulator consists of three main segments  
- Memory, Registers and Stack  
- Instruction Decoder and Execution Unit  
- SDL2 based IO handler  

![Emulator Architecture](https://github.com/paltrickontpb/CHIP8_Emulator/blob/main/chip8.png "Emulator Architecture")

## Virtual hardware
### Memory
The memory units in the chip8 emulator were created using in-built variable types like Unsigned Char (8 bit), unsigned short (16bit) and unsigned int (32-bit). As shown above, the registers and the memory are stored in seperate header files (Chip8memory.h and chip8registers.h). Similarly, as the stack operates independantly, it is defined under a seperate file altogether.

### I/O Devices
The Input/Output devices are defined directly by SDL2 or Simple DirectMedia Layer 2, which provides lower level access to audio, joystick, keyboard and graphics. The keyboard map is a 4x4 matrix of 16 elements defined by 0 to F. This was implemented using a 16 bit register and a polling function to check for keypress. The graphic mapping was done using the graphical abstraction provided by SDL2, which allows the user to draw pixels, or in our case, rectangles (bigger group of pixels) on a black canvas. Both are defined and written in chip8keyboard.c and chip8screen.c, and the defined functions are then called by the execution unit.

### Decoder and Execution
The instructions that are passed into the emulator are passed into the decoder unit, which segregate the 16 bit into 4 nibbles and maps them to the pre-defined opcodes. The techinical reference cites 35 standrd opcodes, which are all implemented in chip8.c program, mapping each opcode to an execution.

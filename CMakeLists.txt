cmake_minimum_required(VERSION 3.17)
project(Chip8 C)

set(CMAKE_C_STANDARD 99)

include_directories(include)

add_executable(Chip8 main.c chip8memory.c chip8stack.c chip8keyboard.c chip8.c chip8screen.c)

# SDL2 Config for CMake
target_link_libraries(Chip8 ${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2.lib)
target_link_libraries(Chip8 ${CMAKE_CURRENT_LIST_DIR}/lib/x86/SDL2main.lib)
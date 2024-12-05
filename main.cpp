#define SDL_MAIN_HANDLED

#include <cstdint>
#include <fstream>
#include <iostream>

#include "Chip8.h"
#include "Platform.h"

int main() {

    const uint16_t VIDEO_WIDTH = 64, VIDEO_HEIGHT = 32;

    Platform platform("CHIP-8", 320, 240, 20, 10);
    Chip8 chip8(VIDEO_WIDTH, VIDEO_HEIGHT);

    chip8.OP_00E0();

    return 0;
}

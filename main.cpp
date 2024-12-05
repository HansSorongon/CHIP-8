#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>

#include <cstdint>
#include <fstream>
#include <iostream>

#include "Chip8.h"

int main() {

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr<<"Error initializing SDL2!";
        return -1;
    };

    constexpr unsigned int PIXEL_SCALE = 20;

    SDL_Window* window = SDL_CreateWindow(
        "CHIP-8",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        64 * PIXEL_SCALE, 32 * PIXEL_SCALE,
        SDL_WINDOW_SHOWN
    );

    if (!window) {
        std::cerr << "SDL_Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    std::cout<<"Initializing CHIP-8...";
    Chip8 chip8;

    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}

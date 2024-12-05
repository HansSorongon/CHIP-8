#include "Platform.h"

Platform::Platform(char const* title, int window_width, int window_height, int texture_width, int texture_height) {

    SDL_Init(SDL_INIT_VIDEO);

    window = SDL_CreateWindow(title, 0, 0, window_width, window_height, SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    texture = SDL_CreateTexture(
        renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, texture_width, texture_height);

}

Platform::~Platform() {
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}


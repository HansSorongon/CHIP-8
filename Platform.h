#ifndef PLATFORM_H
#define PLATFORM_H

#include <SDL2/SDL.h>

class Platform {
public:
    Platform(char const* title, int window_width, int window_height, int texture_width, int texture_height);
    ~Platform();

private:
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Texture* texture{};
};

#endif

#ifndef PLATFORM_H
#define PLATFORM_H

#include <unordered_map>
#include <SDL2/SDL.h>

class Platform {
public:
    Platform(char const* title, int window_width, int window_height, int texture_width, int texture_height);
    ~Platform();

    static const std::unordered_map<SDL_Keycode, int> key_map;

    void update(void const* buffer, int pitch);
    void process_input(uint8_t* keys);

private:
    SDL_Window* window{};
    SDL_Renderer* renderer{};
    SDL_Texture* texture{};
};

#endif

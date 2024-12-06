#include <unordered_map>

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

const std::unordered_map<SDL_Keycode, int> Platform::key_map = {
    { SDLK_ESCAPE, -1 }, // Special case for quitting
    { SDLK_x, 0 },
    { SDLK_1, 1 },
    { SDLK_2, 2 },
    { SDLK_3, 3 },
    { SDLK_q, 4 },
    { SDLK_w, 5 },
    { SDLK_e, 6 },
    { SDLK_a, 7 },
    { SDLK_s, 8 },
    { SDLK_d, 9 },
    { SDLK_z, 0xA },
    { SDLK_c, 0xB },
    { SDLK_4, 0xC },
    { SDLK_r, 0xD },
    { SDLK_f, 0xE },
    { SDLK_v, 0xF }
};

void Platform::update(void const* buffer, int pitch) {
    SDL_UpdateTexture(texture, nullptr, buffer, pitch);
    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}

void Platform::process_input(uint8_t* keys) {
    bool quit = false;

    SDL_Event event;

    while (SDL_PollEvent(&event)) {

        switch (event.type) {

            case SDL_QUIT:
                quit = true;
                break;

            case SDL_KEYDOWN:
            case SDL_KEYUP: {

                auto it = key_map.find(event.key.keysym.sym);
                if (it != key_map.end()) {
                    keys[it->second] = (event.type == SDL_KEYDOWN) ? 1 : 0;
                }

                if (event.key.keysym.sym == SDLK_ESCAPE && event.type == SDL_KEYDOWN) {
                    quit = true;
                }

            } break;

        }

    }

}



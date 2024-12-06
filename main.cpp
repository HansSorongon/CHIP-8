#define SDL_MAIN_HANDLED

#include <chrono>
#include <iostream>
#include <cstdlib>
#include <string>

#include "Platform.h"
#include "Chip8.h"

constexpr int VIDEO_WIDTH = 64;
constexpr int VIDEO_HEIGHT = 32;

int main(int argc, char** argv) {

    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <Scale> <Delay> <ROM>\n";
        std::exit(EXIT_FAILURE);
    }

    try {

        int video_scale = std::stoi(argv[1]);
        int cycle_delay = std::stoi(argv[2]);
        char const* rom_filename = argv[3];

        Platform platform("CHIP-8 Emulator", VIDEO_WIDTH * video_scale, VIDEO_HEIGHT * video_scale, VIDEO_WIDTH, VIDEO_HEIGHT);
        Chip8 chip8(VIDEO_WIDTH, VIDEO_HEIGHT);
        chip8.LoadROM(rom_filename);

        int video_pitch = sizeof(chip8.video[0]) * VIDEO_WIDTH;

        auto lastCycleTime = std::chrono::high_resolution_clock::now();
        bool quit = false;

        while (!quit) {
            quit = platform.process_input(chip8.keypad);

            auto currentTime = std::chrono::high_resolution_clock::now();
            float dt = std::chrono::duration<float, std::chrono::milliseconds::period>(currentTime - lastCycleTime).count();

            if (dt > cycle_delay) {
                lastCycleTime = currentTime;

                chip8.cycle();

                platform.update(chip8.video, video_pitch);
            }
        }

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << '\n';
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}

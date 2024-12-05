#include "Chip8.h"

#include <fstream>
#include <chrono>

Chip8::Chip8() 
    : pc(START_ADDRESS), 
      rand_gen(std::chrono::system_clock::now().time_since_epoch().count()) {

    for (unsigned int i = 0; i < FONTSET_SIZE; ++i) {
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

};

void Chip8::LoadROM(const char* filename) {

    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if (file.is_open()) {

        std::streampos size = file.tellg();
        char* buffer = new char[size];

        file.seekg(0, std::ios::beg);
        file.read(buffer, size);
        file.close();

        for (long i = 0; i < size; ++i) {
            memory[START_ADDRESS + i] = buffer[i];
        }

        delete[] buffer;
    }

}


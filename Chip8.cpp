#include "Chip8.h"

#include <fstream>
#include <chrono>
#include <cstdlib>
#include <cstring>

Chip8::Chip8(uint16_t VIDEO_WIDTH, uint16_t VIDEO_HEIGHT) 
    : pc(START_ADDRESS), 
      rand_gen(std::chrono::system_clock::now().time_since_epoch().count()),
      rand_byte(0, 255),
      VIDEO_WIDTH(VIDEO_WIDTH),
      VIDEO_HEIGHT(VIDEO_HEIGHT) {

    for (unsigned int i = 0; i < FONTSET_SIZE; ++i) {
        memory[FONTSET_START_ADDRESS + i] = fontset[i];
    }

    // Function Pointer Table
    table[0x0] = &Chip8::Table0;
    table[0x1] = &Chip8::OP_1nnn;
    table[0x2] = &Chip8::OP_2nnn;
    table[0x3] = &Chip8::OP_3xkk;
    table[0x4] = &Chip8::OP_4xkk;
    table[0x5] = &Chip8::OP_5xy0;
    table[0x6] = &Chip8::OP_6xkk;
    table[0x7] = &Chip8::OP_7xkk;
    table[0x8] = &Chip8::Table8;
    table[0x9] = &Chip8::OP_9xy0;
    table[0xA] = &Chip8::OP_Annn;
    table[0xB] = &Chip8::OP_Bnnn;
    table[0xC] = &Chip8::OP_Cxkk;
    table[0xD] = &Chip8::OP_Dxyn;
    table[0xE] = &Chip8::TableE;
    table[0xF] = &Chip8::TableF;

    for (size_t i = 0; i <= 0xE; i++) {
        table0[i] = &Chip8::OP_NULL;
        table8[i] = &Chip8::OP_NULL;
        tableE[i] = &Chip8::OP_NULL;
    }

    table0[0x0] = &Chip8::OP_00E0;
    table0[0xE] = &Chip8::OP_00EE;

    table8[0x0] = &Chip8::OP_8xy0;
    table8[0x1] = &Chip8::OP_8xy1;
    table8[0x2] = &Chip8::OP_8xy2;
    table8[0x3] = &Chip8::OP_8xy3;
    table8[0x4] = &Chip8::OP_8xy4;
    table8[0x5] = &Chip8::OP_8xy5;
    table8[0x6] = &Chip8::OP_8xy6;
    table8[0x7] = &Chip8::OP_8xy7;
    table8[0xE] = &Chip8::OP_8xyE;

    tableE[0x1] = &Chip8::OP_ExA1;
    tableE[0xE] = &Chip8::OP_Ex9E;

    for (size_t i = 0; i <= 0x65; i++) {
        tableF[i] = &Chip8::OP_NULL;
    }

    tableF[0x07] = &Chip8::OP_Fx07;
    tableF[0x0A] = &Chip8::OP_Fx0A;
    tableF[0x15] = &Chip8::OP_Fx15;
    tableF[0x18] = &Chip8::OP_Fx18;
    tableF[0x1E] = &Chip8::OP_Fx1E;
    tableF[0x29] = &Chip8::OP_Fx29;
    tableF[0x33] = &Chip8::OP_Fx33;
    tableF[0x55] = &Chip8::OP_Fx55;
    tableF[0x65] = &Chip8::OP_Fx65;

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

// OPCODES

// CLS
void Chip8::OP_00E0() {
    memset(video, 0, sizeof(video));
}

// RET
void Chip8::OP_00EE() {
    --sp;
    pc = stack[sp];
}

// JP address
void Chip8::OP_1nnn() {
    uint16_t address = opcode & 0x0FFFu;

    pc = address;
}

// CALL addr
void Chip8::OP_2nnn() {
    uint16_t address = opcode & 0x0FFFu;

    stack[sp] = pc;
    ++sp;
    pc = address;
}

// SE Vx, byte
void Chip8::OP_3xkk() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    if (registers[Vx] == byte) {
        pc += 2;
    }
}

// SNE Vx, byte
void Chip8::OP_4xkk() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = opcode & 0x00FFu;

    if (registers[Vx] != byte) {
        pc += 2;
    }
}

// SE Vx, Vy
void Chip8::OP_5xy0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] == registers[Vy]) {
		pc += 2;
	}
}

// LD Vx, byte
void Chip8::OP_6xkk() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = (opcode & 0x00FFu);

    registers[Vx] = byte;
}

// ADD Vx, byte
void Chip8::OP_7xkk() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t byte = (opcode & 0x00FFu);

    registers[Vx] += byte;
}

void Chip8::OP_8xy0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	registers[Vx] = registers[Vy];
}

// OR Vx, Vy
void Chip8::OP_8xy1() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] |= registers[Vy];
}

// AND Vx, Vy
void Chip8::OP_8xy2() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] &= registers[Vy];
}

// XOR Vx, Vy
void Chip8::OP_8xy3() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[Vx] ^= registers[Vy];
}

// ADD Vx, Vy
void Chip8::OP_8xy4() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;
    uint16_t sum = registers[Vx] + registers[Vy];

    registers[0xF] = sum > 255u;

    registers[Vx] = sum & 0xFFu;
}

// SUB Vx, Vy
void Chip8::OP_8xy5() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[0xF] = registers[Vx] > registers[Vy];

    registers[Vx] -= registers[Vy];
}

// SHR Vx
void Chip8::OP_8xy6() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	registers[0xF] = (registers[Vx] & 0x1u);

    registers[Vx] >>= 1;

}

// SUBN Vx, Vy
void Chip8::OP_8xy7() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

    registers[0xF] = registers[Vy] > registers[Vx];
	registers[Vx] = registers[Vy] - registers[Vx];
}

// SHL Vx {, Vy}
void Chip8::OP_8xyE() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	registers[0xF] = (registers[Vx] & 0x80u) >> 7u;

    registers[Vx] <<= 1;

}

// SNE Vx, Vy
void Chip8::OP_9xy0() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;

	if (registers[Vx] != registers[Vy]) {
		pc += 2;
	}
}

// LD I, addr
void Chip8::OP_Annn() {
	uint16_t address = opcode & 0x0FFFu;

	index = address;
}

// JP V0, addr
void Chip8::OP_Bnnn() {
	uint16_t address = opcode & 0x0FFFu;

	pc = registers[0] + address;
}

// RND Vx, byte
void Chip8::OP_Cxkk() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t byte = opcode & 0x00FFu;

	registers[Vx] = rand_byte(rand_gen) & byte;
}

// DRW Vx, Vy, nibble
void Chip8::OP_Dxyn() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t Vy = (opcode & 0x00F0u) >> 4u;
	uint8_t height = opcode & 0x000Fu;

	uint8_t xPos = registers[Vx] % VIDEO_WIDTH;
	uint8_t yPos = registers[Vy] % VIDEO_HEIGHT;

	registers[0xF] = 0;

	for (unsigned int row = 0; row < height; ++row) {
		uint8_t spriteByte = memory[index + row];

		for (unsigned int col = 0; col < 8; ++col) {
			uint8_t spritePixel = spriteByte & (0x80u >> col);
			uint32_t* screenPixel = &video[(yPos + row) * VIDEO_WIDTH + (xPos + col)];

			if (spritePixel) {
				if (*screenPixel == 0xFFFFFFFF) {
					registers[0xF] = 1;
				}

				*screenPixel ^= 0xFFFFFFFF;
			}
		}
	}
}

// SKP Vx
void Chip8::OP_Ex9E() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	uint8_t key = registers[Vx];

	if (keypad[key]) {
		pc += 2;
	}
}

// SKNP Vx
void Chip8::OP_ExA1() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	uint8_t key = registers[Vx];

	if (!keypad[key]) {
		pc += 2;
	}
}

// LD Vx, DT
void Chip8::OP_Fx07() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	registers[Vx] = delayTimer;
}

// LD Vx, K
void Chip8::OP_Fx0A() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
    bool br = false;

    for (uint8_t i = 0; i < 16; ++i) {
        if (keypad[i]) {
            registers[Vx] = i;
            br = true;
            break;
        }
    }

    if (!br) pc -= 2;
}

// LD DT, Vx
void Chip8::OP_Fx15() {
    uint8_t Vx = (opcode & 0x0F00u) >> 8u;

    delayTimer = registers[Vx];
}

// LD ST, Vx
void Chip8::OP_Fx18() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	soundTimer = registers[Vx];
}

// ADD I, Vx
void Chip8::OP_Fx1E() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	index += registers[Vx];
}

// LD F, Vx
void Chip8::OP_Fx29() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t digit = registers[Vx];

	index = FONTSET_START_ADDRESS + (5 * digit);
}

// LD B, Vx
void Chip8::OP_Fx33() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;
	uint8_t value = registers[Vx];

	memory[index + 2] = value % 10;
	value /= 10;

	memory[index + 1] = value % 10;
	value /= 10;

	memory[index] = value % 10;
}

// LD[I], Vx
void Chip8::OP_Fx55() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i) {
		memory[index + i] = registers[i];
	}
}

// LD Vx, [I]
void Chip8::OP_Fx65() {
	uint8_t Vx = (opcode & 0x0F00u) >> 8u;

	for (uint8_t i = 0; i <= Vx; ++i) {
		registers[i] = memory[index + i];
	}
}

void Chip8::OP_NULL() {}

void Chip8::cycle() {
    opcode = (memory[pc] << 8u) | memory[pc + 1];

    pc += 2;

    ((*this).*(table[(opcode & 0xF000u) >> 12u]))();

    if (delayTimer > 0) --delayTimer;
    if (soundTimer > 0) --soundTimer;
}

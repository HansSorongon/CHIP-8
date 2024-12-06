CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17 -IC:/CPPLibs/SDL2/x86_64-w64-mingw32/include -Iinclude
LDFLAGS = -LC:/CPPLibs/SDL2/x86_64-w64-mingw32/lib -lSDL2 -lSDL2main

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

TARGET = $(BIN_DIR)/chip8.exe

SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

all: $(TARGET)

$(TARGET): $(OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)

run: $(TARGET)
	$(TARGET)

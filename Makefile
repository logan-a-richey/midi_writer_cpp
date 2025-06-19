# Makefile for C++ source
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build
BIN_DIR := bin
TARGET := $(BIN_DIR)/main.exe

# Compiler flags
CXX := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -I$(INC_DIR)

# .o files
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# target_object
all: $(TARGET)

# link
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(OBJS) -o $@
	@echo "Build complete: $@"

# compile
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# mkdirs
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

# TODO
# Shared library (Linux)
lib/libmidiwriter.so: src/midi_writer.cpp src/midi_c_api.cpp
	$(CXX) -fPIC -shared -o $@ $^

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
	@echo "Cleaned all build files."

# ===== Phony Targets =====
.PHONY: all clean


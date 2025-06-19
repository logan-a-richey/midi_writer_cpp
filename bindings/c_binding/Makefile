################################################################################
# bindings/c_binding/Makefile
# Makefile for libmidiwriter.so
# Config
CXX = g++
CXXFLAGS = -std=c++17 -fPIC -O2 -I$(INCLUDE_DIR)
# LDFLAGS = -shared

# This tells the compiled .so to always look for dependencies relative to itself
LDFLAGS = -shared -Wl,-rpath,'$$ORIGIN/../../lib'

SRC_DIR = ../../src
INCLUDE_DIR = ../../include
BUILD_DIR = build
LIB_DIR = ../../lib
TARGET = $(LIB_DIR)/libmidiwriter.so

# Source files
SRC_FILES = \
    $(SRC_DIR)/midi_writer.cpp \
    $(SRC_DIR)/track.cpp \
    $(SRC_DIR)/tests.cpp \
    midi_c_api.cpp

# Object files
# OBJ_FILES = $(SRC_FILES:%.cpp=$(BUILD_DIR)/%.o)
OBJ_FILES := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

# Default Target
all: $(TARGET)

# Build shared library
$(TARGET): $(OBJ_FILES)
	@mkdir -p $(LIB_DIR)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compile .cpp to .o
$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(BUILD_DIR) $(LIB_DIR)/libmidiwriter.so


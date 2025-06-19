# Makefile

# Dirs
ROOT_DIR := $(shell pwd)
SRC_DIR := src
INCLUDE_DIR := include

BUILD_DIR := build
BIN_DIR := bin
SHARED_LIB_DIR := lib

CTYPES_SRC_DIR := bindings/ctypes_binding/src
CTYPES_INCLUDE_DIR := bindings/ctypes_binding/include
CTYPES_BUILD_DIR := bindings/ctypes_binding/build

# Compiler Flags
CXX := g++
CXXFLAGS := -std=c++17 -fPIC -O2 -I$(INCLUDE_DIR)
CTYPES_CXXFLAGS := $(CXXFLAGS) -I$(CTYPES_INCLUDE_DIR)
LDFLAGS := -shared

# Files
SRC_FILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC_FILES))

CTYPES_SRC_FILES := $(wildcard $(CTYPES_SRC_DIR)/*.cpp)
CTYPES_OBJ_FILES := $(patsubst $(CTYPES_SRC_DIR)/%.cpp,$(CTYPES_BUILD_DIR)/%.o,$(CTYPES_SRC_FILES))

CPP_BIN := $(BIN_DIR)/cpp_test.exe
SHARED_LIB := $(SHARED_LIB_DIR)/libmidiwriter.so

# Targets
.PHONY: all clean cpp shared

all: $(CPP_BIN) $(SHARED_LIB)
	@echo "[BUILD] All targets built."

# Compile Main EXE
$(CPP_BIN): $(OBJ_FILES) | $(BIN_DIR)
	@echo "[BUILD] Linking binary: $@"
	$(CXX) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	@echo "[BUILD] Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile Shared Lib: libmidiwriter.so
$(SHARED_LIB): $(OBJ_FILES) $(CTYPES_OBJ_FILES) | $(SHARED_LIB_DIR)
	@echo "[BUILD] Linking shared library: $@"
	$(CXX) $(LDFLAGS) $^ -o $@

$(CTYPES_BUILD_DIR)/%.o: $(CTYPES_SRC_DIR)/%.cpp | $(CTYPES_BUILD_DIR)
	@echo "[BUILD] Compiling ctypes: $<"
	$(CXX) $(CTYPES_CXXFLAGS) -c $< -o $@

# Create needed dirs
#$(BUILD_DIR) $(BIN_DIR) $(SHARED_LIB_DIR) $(CTYPES_BUILD_DIR):
#	mkdir -p $@

$(BIN_DIR):
	@echo "[BUILD] Creating bin dir..."
	mkdir -p $@

$(BUILD_DIR):
	@echo "[BUILD] Creating build dir..."
	mkdir -p $@

$(SHARED_LIB_DIR):
	@echo "[BUILD] Creating shared lib dir..."
	mkdir -p $@

$(CTYPES_BUILD_DIR):
	@echo "[BUILD] Creating ctypes build dir..."
	mkdir -p $@

# Clean
clean:
	@echo "[CLEAN] Removing build artifacts..."
	rm -rf $(BUILD_DIR) $(BIN_DIR) $(SHARED_LIB_DIR) $(CTYPES_BUILD_DIR)


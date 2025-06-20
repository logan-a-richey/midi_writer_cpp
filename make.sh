#!/bin/bash
# make.sh

# Exit on any error
set -e 
echo "[BUILD] Started..."

# Dirs
ROOT_DIR="$(pwd)"

SRC_DIR="$ROOT_DIR/src"
INCLUDE_DIR="$ROOT_DIR/include"

BUILD_DIR="$ROOT_DIR/build"
BIN_DIR="$ROOT_DIR/bin"
SHARED_LIB_DIR="$ROOT_DIR/lib"

CTYPES_SRC_DIR="$ROOT_DIR/bindings/ctypes_binding/src"
CTYPES_INCLUDE_DIR="$ROOT_DIR/bindings/ctypes_binding/include"
CTYPES_BUILD_DIR="$ROOT_DIR/bindings/ctypes_binding/build"

# Compiler Flags
CXX=g++
CXXFLAGS="-std=c++17 -fPIC -O2 -I$INCLUDE_DIR"
CTYPES_CXXFLAGS="-std=c++17 -fPIC -O2 -I$INCLUDE_DIR -I$CTYPES_INCLUDE_DIR"
LDFLAGS="-shared"

# Clean/Create Build Dirs
echo "[BUILD] Creating build dirs..."
mkdir -p "$BUILD_DIR" "$BIN_DIR" "$CTYPES_BUILD_DIR" "$SHARED_LIB_DIR"

# Compile main .cpp files to .o
echo "[BUILD] Compiling src/*.cpp..."
$CXX $CXXFLAGS -c $SRC_DIR/*.cpp
mv *.o "$BUILD_DIR"

# Link to main testing exe
echo "[BUILD] Linking main binary..."
$CXX "$BUILD_DIR"/*.o -o "$BIN_DIR/cpp_test.exe"

# Compile ctypes binding files
echo "[BUILD] Compiling ctypes_binding/src..."
$CXX $CTYPES_CXXFLAGS -c "$CTYPES_SRC_DIR"/*.cpp
mv *.o "$CTYPES_BUILD_DIR"

# Link shared library
echo "[BUILD] Linking shared library..."
$CXX $CXXFLAGS $LDFLAGS "$CTYPES_BUILD_DIR"/*.o "$BUILD_DIR"/*.o -o "$SHARED_LIB_DIR/libmidiwriter.so"

# Finished
echo "[BUILD] Completed. Output: $SHARED_LIB_DIR/libmidiwriter.so"


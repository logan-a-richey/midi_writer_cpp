#!/bin/bash
# clean.sh

set -e

echo "[CLEAN] Removing build artifacts..."

ROOT_DIR="$(pwd)"
LIB_DIR="$ROOT_DIR/lib"
BUILD_DIR="$ROOT_DIR/build"
CTYPES_BUILD_DIR="$ROOT_DIR/bindings/ctypes_binding/build"

# Remove build artifacts
echo "Removing directory: $LIB_DIR" 
rm -rf "$LIB_DIR" 

echo "Removing directory: $BUILD_DIR" 
rm -rf "$BUILD_DIR" 

echo "Removing directory: $CTYPES_BUILD_DIR"
rm -rf "$CTYPES_BUILD_DIR"

echo "[CLEAN] Finished."

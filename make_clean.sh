#!/bin/bash
# clean.sh

set -e

echo "[CLEAN] Removing build artifacts..."

ROOT_DIR="$(pwd)"
LIB_DIR="$ROOT_DIR/lib"
BUILD_DIR="$ROOT_DIR/build"
CTYPES_BUILD_DIR="$ROOT_DIR/bindings/ctypes_binding/build"

# Remove build artifacts
rm -rf "$LIB_DIR" "$BUILD_DIR" "$CTYPES_BUILD_DIR"

echo "[CLEAN] Finished."

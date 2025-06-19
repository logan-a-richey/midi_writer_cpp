#!/bin/bash
set -e

echo "[CLEAN] Cleaning C++ SRC..."
make clean

echo "[CLEAN] Cleaning C shared library build..."
make -C bindings/c_binding clean

# Optional: Clean other build dirs
echo "[CLEAN] Cleaning build artifacts..."
rm -rf build/ lib/ bindings/c_binding/build

echo "[DONE] Clean completed."


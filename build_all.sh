#!/bin/bash
set -e

echo "[BUILD] Building C++ SRC."
make

echo "[BUILD] Building C shared library..."
make -C bindings/c_binding

echo "[DONE] All targets built successfully."


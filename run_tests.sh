#!/bin/bash
#run_tests.sh

set -e

ROOT_DIR="$(pwd)"
CPP_TEST="$ROOT_DIR/bin/cpp_test.exe"
PY_TEST="$ROOT_DIR/python_usage/py_test.py"

echo "[TEST] Running test files."
mkdir -p output
cd output 

echo "[TEST] Running C++ test..."
"$CPP_TEST"

echo "[TEST] Running Python3 test..."
python3 "$PY_TEST"

echo "[TEST] Created Midi files successfully."


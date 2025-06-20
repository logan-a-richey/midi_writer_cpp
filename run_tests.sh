#!/bin/bash
#run_tests.sh

# Exit on any error
set -e
echo "[TEST] Running test files."

ROOT_DIR="$(pwd)"
CPP_TEST="$ROOT_DIR/bin/cpp_test.exe"
PY_TEST="$ROOT_DIR/python_usage/py_test.py"

mkdir -p output
cd output 

echo "[TEST] Running C++ test..."
"$CPP_TEST"

echo "[TEST] Running Python3 test..."
python3 "$PY_TEST"

echo "[TEST] Created Midi files successfully."


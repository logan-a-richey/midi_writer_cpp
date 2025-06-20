#!/bin/bash
# set up python venv, install project requirements

# Exit on any error:
set -e
echo "[PYTHON_VENV] Started..."

# Check for python3 and pip
if ! command -v python3 &> /dev/null; then
    echo "Error: python3 is not installed."
    exit 1
fi

if ! command -v pip &> /dev/null; then
    echo "Error: pip is not installed. Try: sudo apt-get install python3-pip"
    exit 1
fi

# Ensure venv module is installed
if ! python3 -m venv --help > /dev/null 2>&1; then
    echo "[INFO] Installing python3-venv..."
    sudo apt-get update
    sudo apt-get install -y python3-venv
fi

# Create venv if it doesn't exist
if [ ! -d "venv" ]; then
    echo "[INFO] Creating virtual environment..."
    python3 -m venv venv
fi

# Activate venv
echo "[INFO] Activating virtual environment..."
# shellcheck disable=SC1091
source venv/bin/activate

# Install Python dependencies
if [ -f "requirements.txt" ]; then
    echo "[INFO] Installing requirements..."
    pip install -r requirements.txt
else
    echo "[WARN] requirements.txt not found. Skipping dependency install."
fi

echo "[DONE] Virtual environment setup complete."

#!/bin/bash
#run_test.sh

export LD_LIBRARY_PATH=$(realpath ../lib):$LD_LIBRARY_PATH
python3 midi_test.py

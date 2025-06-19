#!/usr/bin/env python3
# test_c_types.py

# NOTES:
# b'output/...' is a byte string, required for c_char_p.

import os
import ctypes
from ctypes import c_int, c_char_p, c_void_p

lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "..", "lib", "libmidiwriter.so"))
print("Looking for .so at:", lib_path)
assert os.path.isfile(lib_path), "Shared library not found at expected path"

LIB_PATH = os.path.join(os.path.dirname(__file__), "..", "..", "lib", "libmidiwriter.so")
lib = ctypes.CDLL(os.path.abspath(LIB_PATH))

os.makedirs("output", exist_ok=True)

# Load the shared library
lib = ctypes.CDLL(LIB_PATH)

# Return types
lib.midi_writer_create.restype = ctypes.c_void_p
lib.midi_writer_add_track.restype = ctypes.c_int

# Argument types
lib.midi_writer_destroy.argtypes = [ctypes.c_void_p]
lib.midi_writer_add_note.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int]
lib.midi_writer_add_bpm.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_int]
lib.midi_writer_set_channel.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int]
lib.midi_writer_add_track.argtypes = [ctypes.c_void_p]
lib.midi_writer_add_time_signature.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_int, ctypes.c_int, ctypes.c_int]
lib.midi_writer_add_track_name.argtypes = [ctypes.c_void_p, ctypes.c_int, ctypes.c_char_p, ctypes.c_int]
lib.midi_writer_save.argtypes = [ctypes.c_void_p, ctypes.c_char_p]

#*******************************************************************************
# Example usage in Python:

writer = lib.midi_writer_create()

lib.midi_writer_add_bpm(writer, 0, 0, 120)
lib.midi_writer_set_channel(writer, 0, 0)

# Add some notes
lib.midi_writer_add_note(writer, 0, 0, 0, 480, 60, 127)     # C4
lib.midi_writer_add_note(writer, 0, 0, 480, 480, 62, 127)   # D4
lib.midi_writer_add_note(writer, 0, 0, 960, 480, 64, 127)   # E4

lib.midi_writer_save(writer, b'output/test_ctypes.mid')
lib.midi_writer_destroy(writer)

print("Generated output/test_ctypes.mid")


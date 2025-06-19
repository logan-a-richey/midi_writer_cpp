# midi_writer.py

import os
import ctypes
from ctypes import c_int, c_char_p, c_void_p

lib_path = os.path.abspath(os.path.join(os.path.dirname(__file__), "..", "lib", "libmidiwriter.so"))
print("Looking for .so at:", lib_path)
assert os.path.isfile(lib_path), "Shared library not found at expected path"

LIB_PATH = os.path.join(os.path.dirname(__file__), "..", "lib", "libmidiwriter.so")
lib = ctypes.CDLL(os.path.abspath(LIB_PATH))

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

# Python wrapper class of C++ MidiWriter class:
class MidiWriter:
    def __init__(self) -> None:
        self.obj = lib.midi_writer_create()

    def __del__(self) -> None:
        lib.midi_writer_destroy(self.obj)

    def add_note(self, track: int, channel: int, start: int, duration: int, pitch: int, velocity: int) -> None:
        lib.midi_writer_add_note(self.obj, track, channel, start, duration, pitch, velocity)

    def add_bpm(self, track: int, tick: int, bpm: int) -> None:
        lib.midi_writer_add_bpm(self.obj, track, tick, bpm)

    def set_channel(self, track: int, channel: int) -> None:
        lib.midi_writer_set_channel(self.obj, track, channel)

    def add_track(self) -> int:
        return lib.midi_writer_add_track(self.obj)

    def add_time_signature(self, track: int, start: int, numerator: int, denominator: int) -> None:
        lib.midi_writer_add_time_signature(self.obj, track, start, numerator, denominator)

    def add_track_name(self, track: int, name: str, start: int = 0) -> None:
        lib.midi_writer_add_track_name(self.obj, track, name.encode('utf-8'), start)

    def save(self, filename: str) -> None:
        lib.midi_writer_save(self.obj, filename.encode('utf-8'))

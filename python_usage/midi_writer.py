# midi_writer.py

import ctypes

lib = ctypes.CDLL("lib/libmidiwriter.so")

lib.midi_writer_create.restype = ctypes.c_void_p

class MidiWriter:
    def __init__(self):
        self.obj = lib.midi_writer_create()

    def add_note(self, track, channel, start, duration, pitch, velocity):
        lib.midi_writer_add_note(self.obj, track, channel, start, duration, pitch, velocity)

    def add_bpm(self, track, tick, bpm):
        lib.midi_writer_add_bpm(self.obj, track, tick, bpm)

    def set_channel(self, track, channel):
        lib.midi_writer_set_channel(self.obj, track, channel)

    def save(self, filename):
        lib.midi_writer_save(self.obj, filename.encode('utf-8'))

    def __del__(self):
        lib.midi_writer_destroy(self.obj)


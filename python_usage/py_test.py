#!/usr/bin/env python3
# midi_test.py

import sys
sys.dont_write_bytecode = True

import os
import ctypes
from ctypes import c_int, c_char_p, c_void_p

#os.makedirs("output", exist_ok=True)

LIB_PATH = os.path.join(os.path.dirname(__file__), "..", "lib", "libmidiwriter.so")
lib = ctypes.CDLL(os.path.abspath(LIB_PATH))

from midi_writer import MidiWriter

################################################################################

FNAME1 = "py_twinkle_star.mid"
FNAME2 = "py_amen_drums.mid" 

LETTER_TO_PITCH = {
    'C': 60, 
    'D': 62, 
    'E': 64, 
    'F': 65,
    'G': 67, 
    'A': 69, 
    'B': 71
}

DRUMS = {
    "kick_drum": 35,
    "snare_drum_rim": 37,
    "snare_drum": 38,
    "cymbal_hihat_closed": 42,
    "cymbal_hihat_open": 46,
    "cymbal_crash1": 49,
    "cymbal_crash2": 57,
    "cymbal_china": 52,
    "cymbal_ride": 51,
    "cymbal_ride_bell": 53,
    "cymbal_splash": 55,
    "tom2": 43,
    "tom3": 45,
    "tom4": 47,
    "tom5": 50,
    "tambourine": 54,
    "cowbell": 56
}

def twinkle_star_test():
    midi_writer = MidiWriter()
    
    # track, start, bpm
    midi_writer.add_bpm(0, 0, 120)

    # track, start, num, den
    midi_writer.add_time_signature(0, 0, 3, 4)
    
    beat = 0
    
    melody = "CCGGAAG_FFEEDDC_GGFFEED_GGFFEED_CCGGAAG_FFEEDDC_";
    
    for note_char in melody:
        if note_char == '_':
            beat += 1
            continue
        
        note_pitch = LETTER_TO_PITCH.get(note_char, 60)
        
        # track, channel, start, duration, pitch, vol
        midi_writer.add_note(0, 0, 480 * beat, 480, note_pitch, 120)
        beat += 1
    
    midi_writer.save(FNAME1)
    print("[INFO] File created: {}".format(FNAME1))

def create_drum_midi(filename: str, bpm: int, patterns: dict) -> None:
    midi_writer = MidiWriter()
    midi_writer.add_bpm(0, 0, bpm);
    
    TICK_DUR = 120

    for drum_sequence, drum_pitch in patterns.items():
        tick = 0
        for char in drum_sequence:
            if char == 'x':
                midi_writer.add_note(0, 9, tick * TICK_DUR, TICK_DUR, drum_pitch, 120)
            if char != '|':
                tick += 1

    midi_writer.save(filename)
    print("[INFO] File created: {}".format(filename))

def amen_drum_test():
    patterns = {
        "................|................|................|..........x.....|": DRUMS["cymbal_crash1"],
        "x.x.x.x.x.x.x.x.|x.x.x.x.x.x.x.x.|x.x.x.x.x.x.x.x.|x.x.x.x.x...x.x.|": DRUMS["cymbal_ride"],
        "....x.......x...|....x.......x...|....x.........x.|....x.........x.|": DRUMS["snare_drum"],
        ".......x.x.....x|.......x.x.....x|.......x.x......|.x.....x.x......|": DRUMS["snare_drum_rim"],
        "x.........xx....|x.........xx....|x.x.......x.....|..xx......x.....|": DRUMS["kick_drum"]
    }
    create_drum_midi(FNAME2, 170, patterns)

################################################################################

def main():
    twinkle_star_test()
    amen_drum_test()

if __name__ == "__main__":
    main()


#!/usr/bin/env python3

from midi_writer import MidiWriter

def main():
    m = MidiWriter()
    m.add_bpm(0, 0, 120)
    m.set_channel(0, 0)
    m.add_note(0, 0, 480 * 0, 480, 60, 100)
    m.add_note(0, 0, 480 * 1, 480, 62, 100)
    m.add_note(0, 0, 480 * 2, 480, 64, 100)
    m.add_note(0, 0, 480 * 3, 480, 65, 100)
    m.save("test_output.mid")

    print("MIDI saved!")
    return

if __name__ == "__main__":
    main()


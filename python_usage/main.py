# main.py

from midi import MidiWriter

def main():
    m = MidiWriter()
    m.set_channel(0, 0)
    m.add_bpm(0, 0, 120)
    m.add_note(0, 0, 0, 480, 60, 100)
    m.save("output/test_python.mid")

if __name__ == "__main__":
    main()


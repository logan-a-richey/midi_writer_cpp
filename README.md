# MidiWriter

## Overview
- `MidiWriter` is a lightweight C++ class designed to generate and save MIDI files.
- It provides an easy-to-use interface for adding tracks, setting channels, specifying tempo, and inserting MIDI note events.

## Features
- Supports multiple tracks
- Customizable tempo (BPM)
- Ability to specify MIDI channels and programs
- Adds note events with configurable pitch, velocity, and duration
- Saves MIDI data to a file
- Uses standard 480 ticks per quarter note (modifiable)

## Installation
Simply include `MidiWriter.h` and compile it with your C++ project.

### Create Directory for Output MIDI files:
`mkdir output`

### Example compilation (Requires C++ 11 or higher):
`g++ -c *.o`
`g++ -o main *.o`
`./main`

## Usage
### 1. Creating a MIDI File
```cpp
#include <iostream>
#include "midi_writer.h" // import the MidiWriter class

int main() {
    // Simple test to write 4 quarter notes across 4 staves.
    const int TPQ; // TICKS_PER_QUARTER

    // Create an instance of the MidiWriter class:
    MidiWriter myMidi;

    // Create tempo event:
    myMidi.addBPM(0, 0, DEFAULT_BPM);

    // Create Track-Channel mappings:
    // Alternate channels for different tracks:
    myMidi.setChannel(0, 0);
    myMidi.setChannel(1, 0);

    // Add MIDI notes directly:
    myMidi.addNote(0, 0, 0 * TPQ, 1 * TPQ, 60); // C4
    myMidi.addNote(1, 1, 1 * TPQ, 1 * TPQ, 62); // D4
    myMidi.addNote(2, 0, 2 * TPQ, 1 * TPQ, 64); // E4
    myMidi.addNote(3, 1, 3 * TPQ, 1 * TPQ, 65); // F4

    // Write the MIDI:
    std::string output_filename = "output/test_multiple_tracks.mid";
    myMidi.save(output_filename);
    std::cout << "Successfully created " << output_filename << std::endl;
}
```

## API Reference

### `int addTrack()`
Adds a new track to the MIDI file.
- **Returns**: The index of the newly created track.

### `void setChannel(int channel = 0, int program = 0)`
Assigns a program (instrument) to a specific MIDI channel.
- **Parameters**:
  - `channel`: MIDI channel number (0-15).
  - `program`: Program number (0-127) representing an instrument.

### `void addBPM(int track = 0, int start = 0, int bpm = 120)`
Adds a tempo change event to a track.
- **Parameters**:
  - `track`: Track index.
  - `start`: Tick position where the tempo change should be inserted.
  - `bpm`: Beats per minute.

### `void addNote(int track, int channel, int start, int duration, int pitch, int velocity)`
Inserts a note into a specified track.
- **Parameters**:
  - `track`: Track index.
  - `channel`: MIDI channel (0-15).
  - `start`: Start time in ticks.
  - `duration`: Note duration in ticks.
  - `pitch`: MIDI note number (0-127, where 60 is Middle C).
  - `velocity`: Note volume (0-127).

### `void save(const std::string &output_filename)`
Writes the MIDI data to a file.
- **Parameters**:
  - `output_filename`: Name of the output MIDI file.

## Notes
1. Create an instance of the MidiWriter class: `myMidi = MidiWriter();`
- MIDI events are defined in terms of an integer number of ticks per quarter note. Default is 480 ticks per quarter.
- You can change the default ticks per quarter upon instantiating the MidiWriter class.

2. Define channel-to-program mapping (where program index refers to MIDI instrument).

3. Add tempo events to the MidiWriter class using `myMidi.addBPM`.

4. Add notes to the MidiWriter class using `myMidi.addNote(int track, int channel, int start, int duration, int pitch, int velocity)`.
- Tracks are automatically created if they do not exist. For example, adding a note to Track 4 will automatically create tracks up to track 4 to accommodate.

5. Save and write your MIDI object using `myMidi.save()`.
- All tracks and events must be added before calling `save()`.
- The default `TICKS_PER_QUARTER` is 480, aligning with standard MIDI resolution.

6. If you do not have an `output/` directory, create it, and the output MIDI files will appear there.

## License
This project is open-source and available under the MIT License.

## Contributing
- I designed this MidiWriter to be more lightweight than other APIs. There are many features I wish to add going forward.
- Feel free to offer suggestions and feedback!
- If you wish to contribute, please fork the repo and submit a pull request, and I can consider any changes.



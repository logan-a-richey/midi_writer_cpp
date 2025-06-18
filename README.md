# MidiWriter C++

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
* Compile with `make`
* To use, simply include `midi_writer.h` and instantiate a `MidiWriter` object in your C++ project.

## TODO
* Dynamic output directory. Create an `output` dir at parent level if it does not exist already. 
* Pass `const std::string& filename` into `test.h` and `test.cpp` functions for the correct filepath.
* Create a C api for Python. Modify MAKEFILE to also export `.so` files for external use.
* Consider using default params for midi functions.
* For `get_track`, consider using `std::optional<Track>` to return a `std::nullopt` in case the track lookup fails.
* Consider overloading the < operator in `struct Event` for faster sorting.

## Contributing
- Feel free to offer suggestions and feedback!
- If you wish to contribute, please create a separate branch, and I'll consider modifications.

---

## License
This project is open-source and available under the MIT License.

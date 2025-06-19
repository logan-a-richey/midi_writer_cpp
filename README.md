# MidiWriter C++

## Overview
- `MidiWriter` is a lightweight C++ class designed to generate and save MIDI files.
- It provides an easy-to-use interface for adding tracks, setting channels, specifying tempo, and inserting MIDI note events.
- Comes with Makefile and Python binding via CTypes.

## Features
* Creation of `.mid` files.
* class `MidiWriter` provides a simple API with function calls: `add_track`, `set_channel`, `add_bpm`, `add_note`, `add_time_signature`, `add_track_name`, and `save`.
* Uses standard 480 ticks per quarter note (modifiable)
* Supports `.mid` files with greater than 16 tracks.
* Supports channel to program mappng to set different channels to different instrument programs.
* `save` writes the accumlated MidiWriter data to a `.mid` file.

## Installation
* Compile with `./build_all` which calls all of the Makefiles.
* Clean with `./clean_all`.
* To use, simply include `midi_writer.h` and instantiate a `MidiWriter` object in your C++ project.
* To use in a Python project, create a submodule of this repo. From `python_usage/`, import the midi writer class using `from midi_writer import MidiWriter`. `MidiWriter` is the Python wrapper around the C++ version. 

## TODO
* Note there is a `bindings/src` dir that contains `.o` files from `src/`. This should be renamed to `src_build/` to avoid confusion or accidental deletion.
* Pybind11 usage. Currently there is a mock version that should be close, however, the correct Python paths have to be set.
* Default parameters for midi functions. Will involve refactoring ctypes or using pybind11. Not critical since most IDEs have IntelliSense.
* For `get_track`, consider using `std::optional<Track>` to return a `std::nullopt` in case the track lookup fails. Not critical since interal logic is currently working.
* Consider overloading the < operator in `struct Event` for faster sorting in `MidiWriter.cpp`.

## Contributing
- Feel free to offer suggestions and feedback.
- If you wish to contribute, please create a separate branch, and I will consider additional changes!

## License
This project is open-source and available under the MIT License.

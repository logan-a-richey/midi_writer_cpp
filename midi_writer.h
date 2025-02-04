/******************************************************************************
File:   midi_writer.h
Info:   Header file containing class definitions for MidiWriter
Author: Logan Richey
Date:   Feb 4, 2025
******************************************************************************/

#ifndef MIDI_WRITER_H
#define MIDI_WRITER_H

#include <vector>
#include <string>
#include <map>
#include <cstdint>

// An Event holds the absolute tick and a vector of MIDI bytes.
struct Event {
    int tick;
    std::vector<unsigned char> data;
};

// A Track stores a list of events.
class Track {
public:
    std::vector<Event> events;
    void add_event(int tick, const std::vector<unsigned char>& event_data);
    void sort_events();
};

// MidiWriter is a simple class to build and save MIDI files.
class MidiWriter {
public:
    int ticks_per_quarter;
    std::vector<Track> tracks;         // List of tracks.
    std::map<int, int> channel_program;  // Mapping: channel -> program number.

    MidiWriter();
    // Encodes an integer as a variable-length quantity.
    std::vector<unsigned char> encode_var_len(int value);

    // Adds a new track. Returns the index of the track.
    int addTrack();

    // Returns a reference to a track (auto-adds if needed).
    Track& get_track(int track_idx);

    // Sets the instrument (program) for a channel. Also writes a program-change event at tick 0.
    void setChannel(int channel = 0, int program = 0);

    // Inserts a tempo (BPM) change event.
    void addBPM(int track = 0, int start = 0, int bpm = 120);

    // Adds a note event (note-on and note-off) to a track.
    void addNote(int track = 0, int channel = 0, int start = 0, int duration = 480,
                 int pitch = 60, int velocity = 127);

    // Saves the built MIDI file to disk.
    void save(const std::string &output_filename);
};

#endif // MIDI_WRITER_H


/******************************************************************************
File:   midi_writer.h
Info:   Header file containing class definitions for MidiWriter
Author: Logan Richey
Date:   Feb 6, 2025
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
    int TICKS_PER_QUARTER;
    
    // constructor
    MidiWriter(int ticks=480){
        TICKS_PER_QUARTER = ticks;
    };
    
    // Public members
    int addTrack();
    void setChannel(int channel = 0, int program = 0);
    void addBPM(int track = 0, int start = 0, int bpm = 120);
    void addNote(
        int track = 0, int channel = 0, int start = 0,
        int duration = 480, int pitch = 60, int velocity = 127
    );
    void save(const std::string &output_filename);

private:
    std::vector<Track> tracks;         // List of tracks.
    std::map<int, int> channel_program;  // Mapping: channel -> program number.

    Track& get_track(int track_idx);
    std::vector<unsigned char> encode_var_len(int value);
};

#endif // MIDI_WRITER_H


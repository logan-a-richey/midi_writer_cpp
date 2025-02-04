/*
midi_writer.cpp

A C++ version of a simple MIDI file writer.

This code implements a MidiWriter class (and a nested Track class)
that lets you add MIDI tracks, tempo events, note events, and then save
the complete MIDI file to disk.

Compilation (with C++11 or later):
g++ -std=c++11 -o midi_writer midi_writer.cpp
*/

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdint>
#include <map>
#include <algorithm>

// #include "midi_writer.h"

//------------------------------------------------------------
// Struct representing a single MIDI event.
struct Event {
    int tick;                             // The absolute tick time.
    std::vector<unsigned char> data;      // The event bytes.
};

//------------------------------------------------------------
// Track class: a container for events.
class Track {
public:
    std::vector<Event> events;
    
    // Add an event with the given tick and event bytes.
    void add_event(int tick, const std::vector<unsigned char>& event_data) {
        Event ev;
        ev.tick = tick;
        ev.data = event_data;
        events.push_back(ev);
    }
    
    // Sort events in increasing order of tick.
    void sort_events() {
        std::sort(events.begin(), events.end(), [](const Event &a, const Event &b) {
            return a.tick < b.tick;
        });
    }
};

//------------------------------------------------------------
// MidiWriter class: manages tracks and writes the MIDI file.
class MidiWriter {
public:
    int ticks_per_quarter;
    std::vector<Track> tracks;       // A vector of MIDI tracks.
    std::map<int, int> channel_program;  // Mapping: channel -> program number.
    
    // Constructor: initializes with standard resolution (480 ticks per quarter note)
    MidiWriter() : ticks_per_quarter(480) {}
    
    // Encode an integer into MIDI variable-length format.
    // (This implementation mirrors the Python version.)
    std::vector<unsigned char> encode_var_len(int value) {
        // Start with the lowest 7 bits.
        uint32_t buffer = value & 0x7F;
        value >>= 7;
        while (value > 0) {
            // Prepend the next 7-bit chunk with the continuation bit (0x80) set.
            buffer = (buffer << 8) | ((value & 0x7F) | 0x80);
            value >>= 7;
        }
        std::vector<unsigned char> bytes;
        while (true) {
            bytes.push_back(buffer & 0xFF);
            if (buffer & 0x80)
                buffer >>= 8;
            else
                break;
        }
        return bytes;
    }
    
    // Append a new track and return its index.
    int addTrack() {
        Track track;
        tracks.push_back(track);
        return static_cast<int>(tracks.size()) - 1;
    }
    
    // Helper: get a track by index; auto-add tracks if needed.
    Track& get_track(int track_idx) {
        while (track_idx >= static_cast<int>(tracks.size())) {
            addTrack();
        }
        return tracks[track_idx];
    }
    
    // Set the instrument (program) for a channel.
    // Also writes a program change event at tick 0 on track 0.
    void setChannel(int channel = 0, int program = 0) {
        channel_program[channel] = program;
        Track &track0 = get_track(0);
        std::vector<unsigned char> event_bytes;
        event_bytes.push_back(0xC0 | (channel & 0x0F)); // Program change status
        event_bytes.push_back(program & 0x7F);
        track0.add_event(0, event_bytes);
    }
    
    // Insert a tempo (BPM) change event into the specified track.
    void addBPM(int track = 0, int start = 0, int bpm = 120) {
        if (track < 0 || start < 0 || bpm <= 0) {
            std::cerr << "[W] Invalid parameters for addBPM: "
                      << "track=" << track << ", start=" << start << ", bpm=" << bpm << std::endl;
            return;
        }
        
        int tick = start;
        
        // Calculate tempo in microseconds per quarter note (μs/qn): 60,000,000 / BPM.
        int tempo = 60000000 / bpm;
        
        // Convert tempo to 3-byte big-endian.
        std::vector<unsigned char> tempo_bytes(3);
        tempo_bytes[0] = (tempo >> 16) & 0xFF;
        tempo_bytes[1] = (tempo >> 8) & 0xFF;
        tempo_bytes[2] = tempo & 0xFF;
        
        // Build the tempo meta event: FF 51 03 <tempo_bytes>
        std::vector<unsigned char> meta_event = {0xFF, 0x51, 0x03};
        meta_event.insert(meta_event.end(), tempo_bytes.begin(), tempo_bytes.end());
        Track &trk = get_track(track);
        trk.add_event(tick, meta_event);
    }
    
    // Add a note event (with both note-on and note-off) to a track.
    void addNote(int track = 0, int channel = 0, int start = 0, int duration = 480,
                 int pitch = 60, int velocity = 127) {
        if (track < 0 || channel < 0 || start < 0 || duration <= 0 || velocity < 0 || velocity > 127) {
            std::cerr << "[W] Invalid parameters for addNote" << std::endl;
            return;
        }
        int start_tick = start;
        int end_tick = start + duration;
        Track &trk = get_track(track);

        // Note on event: 0x90 + channel, note number, velocity.
        std::vector<unsigned char> note_on = {
            static_cast<unsigned char>(0x90 | (channel & 0x0F)),
            static_cast<unsigned char>(pitch & 0x7F),
            static_cast<unsigned char>(velocity & 0x7F)
        };
        trk.add_event(start_tick, note_on);

        // Note off event: 0x80 + channel, note number, velocity 0.
        std::vector<unsigned char> note_off = {
            static_cast<unsigned char>(0x80 | (channel & 0x0F)),
            static_cast<unsigned char>(pitch & 0x7F),
            0
        };
        trk.add_event(end_tick, note_off);
    }
    
    // Save the MIDI file to disk.
    void save(const std::string &output_filename) {
        // Sort events in each track.
        for (auto &trk : tracks) {
            trk.sort_events();
        }
        
        std::vector<unsigned char> file_data;
        
        // --- Build the Header Chunk ---
        // Header chunk type "MThd"
        std::string header_chunk_type = "MThd";
        file_data.insert(file_data.end(), header_chunk_type.begin(), header_chunk_type.end());
        // Header length (always 6) as a 4-byte big-endian integer.
        file_data.push_back(0);
        file_data.push_back(0);
        file_data.push_back(0);
        file_data.push_back(6);
        // MIDI format: 1 if more than one track, else 0.
        int num_tracks = static_cast<int>(tracks.size());
        uint16_t midi_format = (num_tracks > 1) ? 1 : 0;
        file_data.push_back((midi_format >> 8) & 0xFF);
        file_data.push_back(midi_format & 0xFF);
        // Number of tracks (2 bytes, big-endian).
        file_data.push_back((num_tracks >> 8) & 0xFF);
        file_data.push_back(num_tracks & 0xFF);
        // Time division (ticks per quarter note, 2 bytes, big-endian).
        file_data.push_back((ticks_per_quarter >> 8) & 0xFF);
        file_data.push_back(ticks_per_quarter & 0xFF);
        
        // --- Build each Track Chunk ---
        for (auto &trk : tracks) {
            std::vector<unsigned char> track_data;
            int prev_tick = 0;
            // Process each event in the track.
            for (auto &ev : trk.events) {
                int delta_ticks = ev.tick - prev_tick;
                prev_tick = ev.tick;
                // Encode delta time.
                std::vector<unsigned char> delta = encode_var_len(delta_ticks);
                track_data.insert(track_data.end(), delta.begin(), delta.end());
                // Append the event data.
                track_data.insert(track_data.end(), ev.data.begin(), ev.data.end());
            }
            // Append the End-of-Track meta event:
            // Delta time (0) then FF 2F 00.
            std::vector<unsigned char> delta = encode_var_len(0);
            track_data.insert(track_data.end(), delta.begin(), delta.end());
            track_data.push_back(0xFF);
            track_data.push_back(0x2F);
            track_data.push_back(0x00);
            
            // Prepend the track header ("MTrk" + 4-byte length).
            std::string track_chunk_type = "MTrk";
            file_data.insert(file_data.end(), track_chunk_type.begin(), track_chunk_type.end());
            uint32_t track_length = static_cast<uint32_t>(track_data.size());
            file_data.push_back((track_length >> 24) & 0xFF);
            file_data.push_back((track_length >> 16) & 0xFF);
            file_data.push_back((track_length >> 8) & 0xFF);
            file_data.push_back(track_length & 0xFF);
            // Append the track data.
            file_data.insert(file_data.end(), track_data.begin(), track_data.end());
        }
        
        // --- Write the complete file data to disk ---
        std::ofstream outfile(output_filename, std::ios::binary);
        if (!outfile) {
            std::cerr << "[E] Error opening file " << output_filename << " for writing." << std::endl;
            return;
        }
        outfile.write(reinterpret_cast<const char*>(file_data.data()), file_data.size());
        outfile.close();
        std::cout << "MIDI file '" << output_filename << "' has been saved." << std::endl;
    }
};

//------------------------------------------------------------
// A simple demonstration in main():
int main() {
    MidiWriter midi;
    
    // Set channel 0 to program 0 (typically a piano)
    midi.setChannel(0, 0);
    
    // Set tempo: 120 BPM at tick 0 on track 0.
    midi.addBPM(0, 0, 120);
    
    // For example, add a C major scale.
    int start = 0;
    int duration = midi.ticks_per_quarter;  // quarter note duration
    midi.addNote(0, 0, start + 0 * duration, duration, 60, 100); // C
    midi.addNote(0, 0, start + 1 * duration, duration, 62, 100); // D
    midi.addNote(0, 0, start + 2 * duration, duration, 64, 100); // E
    midi.addNote(0, 0, start + 3 * duration, duration, 65, 100); // F
    midi.addNote(0, 0, start + 4 * duration, duration, 67, 100); // G
    midi.addNote(0, 0, start + 5 * duration, duration, 69, 100); // A
    midi.addNote(0, 0, start + 6 * duration, duration, 71, 100); // B
    midi.addNote(0, 0, start + 7 * duration, duration, 72, 100); // High C

    // Save the MIDI file.
    midi.save("output.mid");

    return 0;
}


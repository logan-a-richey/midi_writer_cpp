/******************************************************************************
File:   midi_writer.cpp
Info:   Contains class MidiWriter, a simple library for creating .mid files.
Author: Logan Richey
Date:   Feb 4, 2025
******************************************************************************/

#include "midi_writer.h"
#include <iostream>
#include <fstream>
#include <algorithm>

// --- Track member function definitions ---

void Track::add_event(int tick, const std::vector<unsigned char>& event_data) {
    Event ev;
    ev.tick = tick;
    ev.data = event_data;
    events.push_back(ev);
}

void Track::sort_events() {
    std::sort(events.begin(), events.end(), [](const Event &a, const Event &b) {
        return a.tick < b.tick;
    });
}

// --- MidiWriter member function definitions ---

MidiWriter::MidiWriter() : ticks_per_quarter(480) {}

std::vector<unsigned char> MidiWriter::encode_var_len(int value) {
    uint32_t buffer = value & 0x7F;
    value >>= 7;
    while (value > 0) {
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

int MidiWriter::addTrack() {
    Track track;
    tracks.push_back(track);
    return static_cast<int>(tracks.size()) - 1;
}

Track& MidiWriter::get_track(int track_idx) {
    while (track_idx >= static_cast<int>(tracks.size())) {
        addTrack();
    }
    return tracks[track_idx];
}

void MidiWriter::setChannel(int channel, int program) {
    channel_program[channel] = program;
    Track &track0 = get_track(0);
    std::vector<unsigned char> event_bytes;
    event_bytes.push_back(0xC0 | (channel & 0x0F)); // Program change status
    event_bytes.push_back(program & 0x7F);
    track0.add_event(0, event_bytes);
}

void MidiWriter::addBPM(int track, int start, int bpm) {
    if (track < 0 || start < 0 || bpm <= 0) {
        std::cerr << "[W] Invalid parameters for addBPM: "
                  << "track=" << track << ", start=" << start << ", bpm=" << bpm << std::endl;
        return;
    }
    
    int tick = start;
    int tempo = 60000000 / bpm;
    std::vector<unsigned char> tempo_bytes(3);
    tempo_bytes[0] = (tempo >> 16) & 0xFF;
    tempo_bytes[1] = (tempo >> 8) & 0xFF;
    tempo_bytes[2] = tempo & 0xFF;
    std::vector<unsigned char> meta_event = {0xFF, 0x51, 0x03};
    meta_event.insert(meta_event.end(), tempo_bytes.begin(), tempo_bytes.end());
    Track &trk = get_track(track);
    trk.add_event(tick, meta_event);
}

void MidiWriter::addNote(int track, int channel, int start, int duration,
                         int pitch, int velocity) {
    if (track < 0 || channel < 0 || start < 0 || duration <= 0 ||
        velocity < 0 || velocity > 127) {
        std::cerr << "[W] Invalid parameters for addNote" << std::endl;
        return;
    }
    int start_tick = start;
    int end_tick = start + duration;
    Track &trk = get_track(track);

    // Note on event.
    std::vector<unsigned char> note_on = {
        static_cast<unsigned char>(0x90 | (channel & 0x0F)),
        static_cast<unsigned char>(pitch & 0x7F),
        static_cast<unsigned char>(velocity & 0x7F)
    };
    trk.add_event(start_tick, note_on);

    // Note off event.
    std::vector<unsigned char> note_off = {
        static_cast<unsigned char>(0x80 | (channel & 0x0F)),
        static_cast<unsigned char>(pitch & 0x7F),
        0
    };
    trk.add_event(end_tick, note_off);
}

void MidiWriter::save(const std::string &output_filename) {
    // Sort events in each track.
    for (auto &trk : tracks) {
        trk.sort_events();
    }
    
    std::vector<unsigned char> file_data;
    
    // Build the Header Chunk.
    std::string header_chunk_type = "MThd";
    file_data.insert(file_data.end(), header_chunk_type.begin(), header_chunk_type.end());
    // Header length = 6 (4-byte big-endian).
    file_data.push_back(0);
    file_data.push_back(0);
    file_data.push_back(0);
    file_data.push_back(6);
    int num_tracks = static_cast<int>(tracks.size());
    uint16_t midi_format = (num_tracks > 1) ? 1 : 0;
    file_data.push_back((midi_format >> 8) & 0xFF);
    file_data.push_back(midi_format & 0xFF);
    file_data.push_back((num_tracks >> 8) & 0xFF);
    file_data.push_back(num_tracks & 0xFF);
    file_data.push_back((ticks_per_quarter >> 8) & 0xFF);
    file_data.push_back(ticks_per_quarter & 0xFF);
    
    // Build each Track Chunk.
    for (auto &trk : tracks) {
        std::vector<unsigned char> track_data;
        int prev_tick = 0;
        for (auto &ev : trk.events) {
            int delta_ticks = ev.tick - prev_tick;
            prev_tick = ev.tick;
            std::vector<unsigned char> delta = encode_var_len(delta_ticks);
            track_data.insert(track_data.end(), delta.begin(), delta.end());
            track_data.insert(track_data.end(), ev.data.begin(), ev.data.end());
        }
        // End-of-track meta event.
        std::vector<unsigned char> delta = encode_var_len(0);
        track_data.insert(track_data.end(), delta.begin(), delta.end());
        track_data.push_back(0xFF);
        track_data.push_back(0x2F);
        track_data.push_back(0x00);
        
        std::string track_chunk_type = "MTrk";
        file_data.insert(file_data.end(), track_chunk_type.begin(), track_chunk_type.end());
        uint32_t track_length = static_cast<uint32_t>(track_data.size());
        file_data.push_back((track_length >> 24) & 0xFF);
        file_data.push_back((track_length >> 16) & 0xFF);
        file_data.push_back((track_length >> 8) & 0xFF);
        file_data.push_back(track_length & 0xFF);
        file_data.insert(file_data.end(), track_data.begin(), track_data.end());
    }
    
    std::ofstream outfile(output_filename, std::ios::binary);
    if (!outfile) {
        std::cerr << "[E] Error opening file " << output_filename << " for writing." << std::endl;
        return;
    }
    outfile.write(reinterpret_cast<const char*>(file_data.data()), file_data.size());
    outfile.close();
    std::cout << "MIDI file '" << output_filename << "' has been saved." << std::endl;
}


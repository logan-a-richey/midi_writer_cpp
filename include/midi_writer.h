// midi_writer.h

#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <vector>

#include "track.h"

struct MidiWriter {
    MidiWriter();

    int add_track();
    void set_channel(int channel, int program);
    void add_bpm(int track_index, int start, int bpm);
    void add_note(int track_index, int channel, int start, int duration, int pitch, int velocity, int off_velocity = 64);
    void add_time_signature(int track_index, int start, int numerator, int denominator);
    void add_track_name(int track_index, const std::string &name, int start = 0);
    void save(const std::string &filename) const;

  private:
    static constexpr int TICKS_PER_QUARTER = 480;
    std::vector<Track> tracks_;
    std::unordered_map<int, int> channel_program_;

    Track &get_track(int track_index);
    std::vector<uint8_t> encode_var_len(int value) const;
};

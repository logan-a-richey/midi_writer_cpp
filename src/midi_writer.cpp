// midi_writer.cpp

#include "midi_writer.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

// Meta event constants
const std::vector<uint8_t> META_END_OF_TRACK = {0xFF, 0x2F, 0x00};
const std::vector<uint8_t> META_TEMPO_PREFIX = {0xFF, 0x51, 0x03};
const std::vector<uint8_t> META_TIME_SIGNATURE_PREFIX = {0xFF, 0x58, 0x04};

MidiWriter::MidiWriter() = default;

int MidiWriter::add_track() {
    tracks_.emplace_back();
    return static_cast<int>(tracks_.size()) - 1;
}

Track &MidiWriter::get_track(int track_index) {
    while (track_index >= static_cast<int>(tracks_.size())) {
        add_track();
    }
    return tracks_[track_index];
}

void MidiWriter::set_channel(int channel, int program) {
    channel_program_[channel] = program;
    std::vector<uint8_t> event = {
        static_cast<uint8_t>(0xC0 | (channel & 0x0F)), static_cast<uint8_t>(program & 0x7F)};
    get_track(0).add_event(0, event);
}

void MidiWriter::add_bpm(int track_index, int start, int bpm) {
    if (bpm <= 0) {
        std::cerr << "[W] Invalid BPM: " << bpm << "\n";
        return;
    }

    int tempo = 60000000 / bpm;
    std::vector<uint8_t> tempo_bytes = {static_cast<uint8_t>((tempo >> 16) & 0xFF),
        static_cast<uint8_t>((tempo >> 8) & 0xFF),
        static_cast<uint8_t>(tempo & 0xFF)};

    std::vector<uint8_t> event = META_TEMPO_PREFIX;
    event.insert(event.end(), tempo_bytes.begin(), tempo_bytes.end());

    get_track(track_index).add_event(start, event);
}

void MidiWriter::add_time_signature(int track_index, int start, int numerator, int denominator) {
    if (numerator <= 0 || (denominator & (denominator - 1)) != 0) {
        std::cerr << "[W] Invalid time signature: " << numerator << "/" << denominator << "\n";
        return;
    }

    int dd = 0;
    for (int d = denominator; d > 1; d >>= 1) ++dd;

    uint8_t cc = 24; // default MIDI clocks per metronome click
    uint8_t bb = 8;  // number of 32nd notes per MIDI quarter

    std::vector<uint8_t> event = META_TIME_SIGNATURE_PREFIX;
    event.push_back(static_cast<uint8_t>(numerator));
    event.push_back(static_cast<uint8_t>(dd));
    event.push_back(cc);
    event.push_back(bb);

    get_track(track_index).add_event(start, event);
}

void MidiWriter::add_track_name(int track_index, const std::string &name, int start) {
    std::vector<uint8_t> name_bytes(name.begin(), name.end());
    std::vector<uint8_t> event = {0xFF, 0x03, static_cast<uint8_t>(name_bytes.size())};
    event.insert(event.end(), name_bytes.begin(), name_bytes.end());

    get_track(track_index).add_event(start, event);
}

void MidiWriter::add_note(int track_index,
    int channel,
    int start,
    int duration,
    int pitch,
    int velocity,
    int off_velocity) {
    if (duration <= 0 || velocity < 0 || velocity > 127) {
        std::cerr << "[W] Invalid note parameters.\n";
        return;
    }

    int end = start + duration;
    auto &track = get_track(track_index);

    std::vector<uint8_t> note_on = {static_cast<uint8_t>(0x90 | (channel & 0x0F)),
        static_cast<uint8_t>(pitch & 0x7F),
        static_cast<uint8_t>(velocity & 0x7F)};

    std::vector<uint8_t> note_off = {static_cast<uint8_t>(0x80 | (channel & 0x0F)),
        static_cast<uint8_t>(pitch & 0x7F),
        static_cast<uint8_t>(off_velocity & 0x7F)};

    track.add_event(start, note_on);
    track.add_event(end, note_off);
}

std::vector<uint8_t> MidiWriter::encode_var_len(int value) const {
    std::vector<uint8_t> buffer;
    uint32_t shifted = value & 0x7F;
    while ((value >>= 7)) {
        shifted <<= 8;
        shifted |= ((value & 0x7F) | 0x80);
    }
    while (true) {
        buffer.push_back(static_cast<uint8_t>(shifted & 0xFF));
        if (shifted & 0x80)
            shifted >>= 8;
        else
            break;
    }
    return buffer;
}

void MidiWriter::save(const std::string &filename) const {
    std::ofstream out(filename, std::ios::binary);
    if (!out) {
        throw std::runtime_error("Failed to open output file.");
    }

    // sort events in each track
    for (const auto &track : tracks_) {
        const_cast<Track &>(track).sort_events();
    }

    std::vector<uint8_t> output;

    // Header chunk
    output.insert(output.end(), {'M', 'T', 'h', 'd'});
    output.insert(output.end(), {0x00, 0x00, 0x00, 0x06}); // header length
    uint16_t format = (tracks_.size() > 1) ? 1 : 0;
    uint16_t ntracks = static_cast<uint16_t>(tracks_.size());
    output.push_back(format >> 8);
    output.push_back(format & 0xFF);
    output.push_back(ntracks >> 8);
    output.push_back(ntracks & 0xFF);
    output.push_back(TICKS_PER_QUARTER >> 8);
    output.push_back(TICKS_PER_QUARTER & 0xFF);

    for (const auto &track : tracks_) {
        std::vector<uint8_t> track_data;
        int last_tick = 0;

        for (const auto &ev : track.get_events()) {
            int delta = static_cast<int>(ev.tick - last_tick);
            last_tick = ev.tick;

            auto delta_bytes = encode_var_len(delta);
            track_data.insert(track_data.end(), delta_bytes.begin(), delta_bytes.end());
            track_data.insert(track_data.end(), ev.data.begin(), ev.data.end());
        }

        auto eot = encode_var_len(0);
        track_data.insert(track_data.end(), eot.begin(), eot.end());
        track_data.insert(track_data.end(), META_END_OF_TRACK.begin(), META_END_OF_TRACK.end());

        output.insert(output.end(), {'M', 'T', 'r', 'k'});
        uint32_t len = static_cast<uint32_t>(track_data.size());
        output.push_back((len >> 24) & 0xFF);
        output.push_back((len >> 16) & 0xFF);
        output.push_back((len >> 8) & 0xFF);
        output.push_back(len & 0xFF);
        output.insert(output.end(), track_data.begin(), track_data.end());
    }

    out.write(reinterpret_cast<const char *>(output.data()), output.size());

    // DEBUG:
    // std::cout << "[INFO] MIDI file saved: " << filename << "\n";
}

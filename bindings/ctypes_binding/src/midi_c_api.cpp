// midi_c_api.cpp

#include "midi_c_api.h"
#include "midi_writer.h"

extern "C" {
void *midi_writer_create() {
    return new MidiWriter();
}

void midi_writer_destroy(void *writer) {
    delete static_cast<MidiWriter *>(writer);
}

void midi_writer_add_note(
    void *writer, int track, int channel, int start_tick, int duration, int pitch, int velocity) {
    static_cast<MidiWriter *>(writer)->add_note(
        track, channel, start_tick, duration, pitch, velocity);
}

void midi_writer_add_bpm(void *writer, int track, int tick, int bpm) {
    static_cast<MidiWriter *>(writer)->add_bpm(track, tick, bpm);
}

// TODO should be int channel, int program
void midi_writer_set_channel(void *writer, int channel, int program) {
    static_cast<MidiWriter *>(writer)->set_channel(channel, program);
}

void midi_writer_save(void *writer, const char *filename) {
    static_cast<MidiWriter *>(writer)->save(std::string(filename));
}

int midi_writer_add_track(void *writer) {
    return static_cast<MidiWriter *>(writer)->add_track();
}

void midi_writer_add_time_signature(
    void *writer,
    int track,
    int start,
    int numerator,
    int denominator) {
    static_cast<MidiWriter *>(writer)->add_time_signature(track, start, numerator, denominator);
}

void midi_writer_add_track_name(void *writer, int track, const char *name, int start) {
    static_cast<MidiWriter *>(writer)->add_track_name(track, std::string(name), start);
}
}

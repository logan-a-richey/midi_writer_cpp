// midi_c_api.cpp

#include "midi_writer.h"
#include "midi_c_api.h"

extern "C" {
    void* midi_writer_create() {
        return new MidiWriter();
    }

    void midi_writer_destroy(void* writer) {
        delete static_cast<MidiWriter*>(writer);
    }

    void midi_writer_add_note(void* writer, int track, int channel, int start_tick, int duration, int pitch, int velocity) {
        static_cast<MidiWriter*>(writer)->add_note(track, channel, start_tick, duration, pitch, velocity);
    }

    void midi_writer_add_bpm(void* writer, int track, int tick, int bpm) {
        static_cast<MidiWriter*>(writer)->add_bpm(track, tick, bpm);
    }

    void midi_writer_set_channel(void* writer, int track, int channel) {
        static_cast<MidiWriter*>(writer)->set_channel(track, channel);
    }

    void midi_writer_save(void* writer, const char* filename) {
        static_cast<MidiWriter*>(writer)->save(std::string(filename));
    }
}

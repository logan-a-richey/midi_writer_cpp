// midi_c_api.h

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

void* midi_writer_create();
void  midi_writer_destroy(void* writer);
void  midi_writer_add_note(void* writer, int track, int channel, int start_tick, int duration, int pitch, int velocity);
void  midi_writer_add_bpm(void* writer, int track, int tick, int bpm);
void  midi_writer_set_channel(void* writer, int track, int channel);
void  midi_writer_save(void* writer, const char* filename);

int   midi_writer_add_track(void* writer);
void  midi_writer_add_time_signature(void* writer, int track, int start, int numerator, int denominator);
void  midi_writer_add_track_name(void* writer, int track, const char* name, int start);

#ifdef __cplusplus
}
#endif

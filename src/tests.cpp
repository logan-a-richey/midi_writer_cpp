// test.cpp

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

#include "midi_writer.h"
#include "tests.h"

constexpr int TICKS_PER_QUARTER = 480;
constexpr int TICK_PER_16TH = TICKS_PER_QUARTER / 4;
constexpr int DEFAULT_BPM = 120;
constexpr int DEFAULT_VELOCITY = 120;
constexpr int DRUM_CHANNEL = 9;
constexpr int DEFAULT_DRUM_PITCH = 32;

const std::unordered_map<char, int> LETTER_TO_PITCH = {
    {'C', 60},
    {'D', 62},
    {'E', 64},
    {'F', 65},
    {'G', 67},
    {'A', 69},
    {'B', 71}
};

const std::unordered_map<std::string, int> DRUMS = {
    {"kick_drum", 35},
    {"snare_drum_rim", 37},
    {"snare_drum", 38},
    {"cymbal_hihat_closed", 42},
    {"cymbal_hihat_open", 46},
    {"cymbal_crash1", 49},
    {"cymbal_crash2", 57},
    {"cymbal_china", 52},
    {"cymbal_ride", 51},
    {"cymbal_ride_bell", 53},
    {"cymbal_splash", 55},
    {"tom2", 43},
    {"tom3", 45},
    {"tom4", 47},
    {"tom5", 50},
    {"tambourine", 54},
    {"cowbell", 56}
};

/*
int get_drum_pitch(const std::string &name) {
    auto it = DRUMS.find(name);
    return (it != DRUMS.end()) ? it->second : DEFAULT_DRUM_PITCH;
}
*/

void MidiTest::test_twinkle_star() {
    MidiWriter midi;
    midi.add_bpm(0, 0, DEFAULT_BPM);
    midi.set_channel(0, 0);

    const std::string melody = "CCGGAAG_FFEEDDC_GGFFEED_GGFFEED_CCGGAAG_FFEEDDC_";
    int beat = 0;

    for (char ch : melody) {
        if (ch == '_') {
            ++beat;
            continue;
        }
        auto it = LETTER_TO_PITCH.find(ch);
        if (it != LETTER_TO_PITCH.end()) {
            midi.add_note(
                0,                          /* track */
                0,                          /* channel */
                beat * TICKS_PER_QUARTER,   /* start */
                TICKS_PER_QUARTER,          /* duration */
                it->second,                 /* pitch */
                DEFAULT_VELOCITY            /* velocity */
            );
        }
        ++beat;
    }

    const std::string filename = "output/test_twinkle_star.mid";
    midi.save(filename);
    std::cout << "[INFO] Created " << filename << '\n';
}

void MidiTest::test_chords() {
    MidiWriter midi;
    midi.add_bpm(0, 0, DEFAULT_BPM);
    midi.set_channel(0, 0);

    struct NoteSpec {
        int start;
        int duration;
        int pitch;
    };

    const std::vector<NoteSpec> notes = {
        {0, 3, 60},
        {0, 3, 64},
        {0, 3, 67},
        {0, 3, 71},
        {4, 4, 60},
        {5, 4, 64},
        {6, 4, 67},
        {7, 4, 71}
    };

    for (const auto &n : notes) {
        midi.add_note(
            0,                              /* track */
            0,                              /* channel */
            n.start * TICKS_PER_QUARTER,    /* start */
            n.duration * TICKS_PER_QUARTER, /* duration */
            n.pitch,                        /* pitch */
            DEFAULT_VELOCITY);              /* velocity */
    }

    const std::string filename = "output/test_chords.mid";
    midi.save(filename);
    std::cout << "[INFO] Created " << filename << std::endl;
}

void MidiTest::test_multiple_tracks() {
    MidiWriter midi;
    midi.add_bpm(0, 0, DEFAULT_BPM);

    midi.set_channel(0, 0);
    midi.set_channel(1, 0);

    // track, channel, start, dur, pitch, volume
    midi.add_note(0, 0, 0 * TICKS_PER_QUARTER, 1 * TICKS_PER_QUARTER, 60, 120);
    midi.add_note(1, 1, 1 * TICKS_PER_QUARTER, 1 * TICKS_PER_QUARTER, 62, 100);
    midi.add_note(2, 0, 2 * TICKS_PER_QUARTER, 1 * TICKS_PER_QUARTER, 64, 80);
    midi.add_note(3, 1, 3 * TICKS_PER_QUARTER, 1 * TICKS_PER_QUARTER, 65, 60);

    const std::string filename = "output/test_multiple_tracks.mid";
    midi.save(filename);
    std::cout << "[INFO] Created " << filename << std::endl;
}

void MidiTest::create_drum_midi(
    const std::string &filename, 
    int bpm, 
    const std::map<std::string, 
    int> &patterns) 
{
    MidiWriter midi;
    midi.add_bpm(0, 0, bpm);

    for (const auto &[pattern, pitch] : patterns) {
        int tick_index = 0;
        for (char c : pattern) {
            if (c == 'x') {
                midi.add_note(0,
                    DRUM_CHANNEL,
                    tick_index * TICK_PER_16TH,
                    TICK_PER_16TH,
                    pitch,
                    DEFAULT_VELOCITY
                );
            }
            if (c != '|') {
                ++tick_index;
            }
        }
    }

    midi.save(filename);
    std::cout << "[INFO] Created " << filename << std::endl;
}

void MidiTest::test_amen_drums() {
    std::map<std::string, int> patterns = {
        {"................|................|................|..........x.....|", DRUMS["cymbal_crash1"]},
        {"x.x.x.x.x.x.x.x.|x.x.x.x.x.x.x.x.|x.x.x.x.x.x.x.x.|x.x.x.x.x...x.x.|", DRUMS["cymbal_ride"]},
        {"....x.......x...|....x.......x...|....x.........x.|....x.........x.|", DRUMS["snare_drum"]},
        {".......x.x.....x|.......x.x.....x|.......x.x......|.x.....x.x......|", DRUMS["snare_drum_rim"]},
        {"x.........xx....|x.........xx....|x.x.......x.....|..xx......x.....|", DRUMS["kick_drum"]}
    };

    create_drum_midi("output/test_drum_amen.mid", 170, patterns);
}

void MidiTest::test_disco_drums() {
    std::map<std::string, int> patterns = {
        {"|x...............|................|................|................|x...............|", DRUMS["cymbal_crash1"]},
        {"|................|................|x...x...x...x...|................|................|", DRUMS["cymbal_ride"]},
        {"|................|................|..x...x...x...x.|................|................|", DRUMS["cymbal_ride_bell"]},
        {"|....xx.xxx.xxx.x|xx.xxx.xxx.xxx.x|................|................|................|", DRUMS["cymbal_hihat_closed"]},
        {"|......x...x...x.|..x...x...x...x.|................|................|................|", DRUMS["cymbal_hihat_open"]},
        {"|................|................|................|xxx.............|................|", DRUMS["tom5"]},
        {"|................|................|................|...xxx..........|................|", DRUMS["tom4"]},
        {"|................|................|................|......xxx.......|................|", DRUMS["tom3"]},
        {"|................|................|................|.........xxx....|................|", DRUMS["tom2"]},
        {"|x...x...x...x...|x...x...x...x...|x...x...x...x...|x...x...x...x...|x...............|", DRUMS["cowbell"]},
        {"|....x.......x..x|....x.......xxxx|...x..x....x..x.|............xxxx|................|", DRUMS["snare_drum"]},
        {"|x.....x...x..x..|x.....x...x..x..|x.....x...x..x..|x...x...x...x...|x...............|", DRUMS["kick_drum"]}
    };

    create_drum_midi("output/test_drums_disco.mid", 125, patterns);
}


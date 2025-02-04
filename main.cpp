// main.cpp

/*
g++ -std=c++11 -o main midi_writer.cpp main.cpp
*/

#include "midi_writer.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Testing constants
const int TICKS_PER_QUARTER = 480;
const int DEFAULT_BPM = 120;
const int DEFAULT_VELOCITY = 120;
const int DRUM_CHANNEL = 9;
const int DEFAULT_DRUM_PITCH = 32;

// Define the DRUMS mapping.
const std::map<std::string, int> DRUMS = {
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
    {"tom1", 41},
    {"tom2", 43},
    {"tom3", 45},
    {"tom4", 47},
    {"tom5", 50},
    {"tambourine", 54},
    {"cowbell", 56}
};

void test_twinkle_star() {
    // Simpler alias for TICKS_PER_QUARTER
    int TPQ = TICKS_PER_QUARTER;

    // Create the MIDI object.
    MidiWriter myMidi;

    // Add Tempo Event
    myMidi.addBPM(0, 0, DEFAULT_BPM);

    // Add Track-Channel mappings
    myMidi.setChannel(0, 0);  // Acoustic Grand Piano

    // Create a data structure for the song
    std::string notes = "CCGGAAG_FFEEDDC_GGFFEED_GGFFEED_CCGGAAG_FFEEDDC_";
    std::map<char, int> letter_to_pitch = {
        {'C', 60},
        {'D', 62},
        {'E', 64},
        {'F', 65},
        {'G', 67},
        {'A', 69},
        {'B', 71}
    };

    // Iterate over the data structure and create MIDI notes
    for (size_t beat = 0; beat < notes.size(); ++beat) {
        char note = notes[beat];
        if (letter_to_pitch.find(note) == letter_to_pitch.end())
            continue;
        int note_pitch = letter_to_pitch[note];
        myMidi.addNote(0, 0, static_cast<int>(beat * TPQ), TPQ, note_pitch);
        if (beat == 8)
            myMidi.addBPM(0, static_cast<int>(beat * TPQ), 80);
        if (beat == 16)
            myMidi.addBPM(0, static_cast<int>(beat * TPQ), 180);
    }

    // Write the MIDI file
    std::string output_filename = "output/test_twinkle_star.mid";
    myMidi.save(output_filename);
    std::cout << "Successfully created " << output_filename << std::endl;
}

void test_chords() {
    // Simpler alias for TICKS_PER_QUARTER
    int TPQ = TICKS_PER_QUARTER;

    // Create MIDI object
    MidiWriter myMidi;

    // Create tempo event
    myMidi.addBPM(0, 0, DEFAULT_BPM);

    // Create Track-Channel mappings
    myMidi.setChannel(0, 0);  // Acoustic Grand Piano

    // Create data structure for song
    struct NoteSpec { int start; int duration; int pitch; };
    std::vector<NoteSpec> notes = {
        {0, 3, 60},
        {0, 3, 64},
        {0, 3, 67},
        {0, 3, 71},
        {4, 4, 60},
        {5, 4, 64},
        {6, 4, 67},
        {7, 4, 71}
    };

    // Loop over data structure for song, create MIDI notes
    for (const auto &note : notes) {
        myMidi.addNote(0, 0, note.start * TPQ, note.duration * TPQ, note.pitch);
    }

    // Write the MIDI
    std::string output_filename = "output/test_chords.mid";
    myMidi.save(output_filename);
    std::cout << "Successfully created " << output_filename << std::endl;
}

void test_multiple_tracks() {    
    // Simpler alias for TICKS_PER_QUARTER
    int TPQ = TICKS_PER_QUARTER;

    // Create the MIDI object
    MidiWriter myMidi;

    // Create tempo event
    myMidi.addBPM(0, 0, DEFAULT_BPM);

    // Create Track-Channel mappings
    // Alternate channels for different tracks.
    myMidi.setChannel(0, 0);
    myMidi.setChannel(1, 0);

    // Add MIDI notes directly
    myMidi.addNote(0, 0, 0 * TPQ, 1 * TPQ, 60);
    myMidi.addNote(1, 1, 1 * TPQ, 1 * TPQ, 62);
    myMidi.addNote(2, 0, 2 * TPQ, 1 * TPQ, 64);
    myMidi.addNote(3, 1, 3 * TPQ, 1 * TPQ, 65);

    // Write the MIDI
    std::string output_filename = "output/test_multiple_tracks.mid";
    myMidi.save(output_filename);
    std::cout << "Successfully created " << output_filename << std::endl;
}

void create_drum_midi(const std::string &filename, int bpm, const std::map<std::string, int> &patterns) {
    // Function to write different drum beats, based off of string patterns
    
    // Create MIDI object
    MidiWriter myMidi;

    // Create tempo event
    myMidi.addBPM(0, 0, bpm);

    // Simpler alias for 16th note ticks
    int TICK_PER_16TH = TICKS_PER_QUARTER / 4;

    // Loop over the drum patterns
    for (const auto &pair : patterns) {
        const std::string &pattern = pair.first;
        int drum_pitch = pair.second;
        int i = 0;
        for (char c : pattern) {
            if (c == 'x') {
                myMidi.addNote(0, DRUM_CHANNEL, i * TICK_PER_16TH,
                               TICK_PER_16TH, drum_pitch, DEFAULT_VELOCITY);
            }
            if (c != '|')
                ++i;
        }
    }

    // Write the MIDI
    myMidi.save(filename);
    std::cout << "Successfully created " << filename << std::endl;
}

void test_amen_drums() {
    // Contains data structure for Amen Drum beat, commonly heard in Breakcore music.
    std::map<std::string, int> amen_patterns;
    auto getDrum = [](const std::string &key) -> int {
        auto it = DRUMS.find(key);
        return (it != DRUMS.end()) ? it->second : DEFAULT_DRUM_PITCH;
    };
    amen_patterns["................|................|................|..........x.....|"] = getDrum("cymbal_crash1");
    amen_patterns["x.x.x.x.x.x.x.x.|x.x.x.x.x.x.x.x.|x.x.x.x.x.x.x.x.|x.x.x.x.x...x.x.|"] = getDrum("cymbal_ride");
    amen_patterns["....x.......x...|....x.......x...|....x.........x.|....x.........x.|"] = getDrum("snare_drum");
    amen_patterns[".......x.x.....x|.......x.x.....x|.......x.x......|.x.....x.x......|"] = getDrum("snare_drum_rim");
    amen_patterns["x.........xx....|x.........xx....|x.x.......x.....|..xx......x.....|"] = getDrum("kick_drum");

    int SONG_BPM = 170;
    create_drum_midi("output/test_drum_amen.mid", SONG_BPM, amen_patterns);
}

void test_disco_drums() {
    // Contains data structure for a drum beat containing more drum sounds.
    std::map<std::string, int> disco_patterns;
    auto getDrum = [](const std::string &key) -> int {
        auto it = DRUMS.find(key);
        return (it != DRUMS.end()) ? it->second : DEFAULT_DRUM_PITCH;
    };
    disco_patterns["|x...............|................|................|................|x...............|"] = getDrum("cymbal_crash1");
    disco_patterns["|................|................|x...x...x...x...|................|................|"] = getDrum("cymbal_ride");
    disco_patterns["|................|................|..x...x...x...x.|................|................|"] = getDrum("cymbal_ride_bell");
    disco_patterns["|....xx.xxx.xxx.x|xx.xxx.xxx.xxx.x|................|................|................|"] = getDrum("cymbal_hihat_closed");
    disco_patterns["|......x...x...x.|..x...x...x...x.|................|................|................|"] = getDrum("cymbal_hihat_open");
    disco_patterns["|................|................|................|xxx.............|................|"] = getDrum("tom5");
    disco_patterns["|................|................|................|...xxx..........|................|"] = getDrum("tom4");
    disco_patterns["|................|................|................|......xxx.......|................|"] = getDrum("tom3");
    disco_patterns["|................|................|................|.........xxx....|................|"] = getDrum("tom2");
    disco_patterns["|x...x...x...x...|x...x...x...x...|x...x...x...x...|x...x...x...x...|x...............|"] = getDrum("cowbell");
    disco_patterns["|....x.......x..x|....x.......xxxx|...x..x....x..x.|............xxxx|................|"] = getDrum("snare_drum");
    disco_patterns["|x.....x...x..x..|x.....x...x..x..|x.....x...x..x..|x...x...x...x...|x...............|"] = getDrum("kick_drum");

    int SONG_BPM = 125;
    create_drum_midi("output/test_drums_disco.mid", SONG_BPM, disco_patterns);
}

int main(){
    // Runs all of the tests.
    test_twinkle_star();
    test_chords();
    test_multiple_tracks();
    test_amen_drums();
    test_disco_drums();
    return 0;
}


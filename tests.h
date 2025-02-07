/*******************************************************************************
File:   tests.h
Info:   Declare tests for MidiWriter class
Author: Logan Richey
Date:   Feb 6, 2025
*******************************************************************************/

// INCLUDES
#include <iostream>
#include <map>
#include <string>
#include <vector>

// GLOBAL VARIABLES
const int TPQ = 480; // TICKS_PER_QUARTER
const int DEFAULT_BPM = 120;
const int DEFAULT_VELOCITY = 120;
const int DRUM_CHANNEL = 9;
const int DEFAULT_DRUM_PITCH = 32;
const std::map<std::string, int> DRUMS = { // Define the DRUMS mapping.
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

// FUNCTIONS
void test_twinkle_star();
void test_chords();
void test_multiple_tracks();

void create_drum_midi(
    const std::string &filename, 
    int bpm,
    const std::map<std::string, int> &patterns
);

void test_amen_drums();
void test_disco_drums();


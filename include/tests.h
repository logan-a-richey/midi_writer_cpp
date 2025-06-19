// tests.h

#pragma once

#include <iostream>
#include <map>
#include <string>
#include <unordered_map>
#include <vector>

namespace MidiTest {
void test_twinkle_star();
void test_chords();
void test_multiple_tracks();

void create_drum_midi(
    const std::string &filename, int bpm, const std::map<std::string, int> &patterns);
void test_amen_drums();
void test_disco_drums();
} // namespace MidiTest

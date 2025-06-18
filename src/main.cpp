// main.cpp

#include "tests.h"

int main(void)
{
    // run all tests

    MidiTest::test_twinkle_star();
    MidiTest::test_chords();
    MidiTest::test_multiple_tracks();
    MidiTest::test_amen_drums();
    MidiTest::test_disco_drums();

    return 0;
}

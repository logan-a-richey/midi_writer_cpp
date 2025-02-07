/******************************************************************************
File:   main.cpp
Info:   MidiWriter demo!
Author: Logan Richey
Date:   Feb 4, 2025
******************************************************************************/

/*
Compile and run demo program using:
The demo midi output files should appear in an output directory.

g++ -std=c++11 -o main midi_writer.cpp main.cpp
./main
*/

#include "tests.h"

int main(){
    // Runs all of the tests.
    test_twinkle_star();
    test_chords();
    test_multiple_tracks();
    test_amen_drums();
    test_disco_drums();
    return 0;
}


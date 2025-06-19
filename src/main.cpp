// main.cpp

#include <iostream>
// #include <filesystem>

#include "tests.h"

int main(void) 
{
    // Create `output/` dir at the same level as main.cpp if it does not exist already
    /*
    std::filesystem::path dir_path = "output";
    try {
        if (std::filesystem::create_directory(dir_path)) {
            std::cout << "Directory created successfully: " << dir_path << std::endl;
        } else {
            std::cout << "Directory already exists or could not be created: " << dir_path << std::endl;
        }
    } catch (const std::filesystem::filesystem_error& e) {
        std::cerr << "Error creating directory: " << e.what() << std::endl;
    }
    */

    // Run all tests
    MidiTest::test_twinkle_star();
    MidiTest::test_chords();
    MidiTest::test_multiple_tracks();
    MidiTest::test_amen_drums();
    MidiTest::test_disco_drums();

    return 0;
}

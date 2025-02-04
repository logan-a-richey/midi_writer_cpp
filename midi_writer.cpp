/*
Module:   midi_test.py
Author:   Logan Richey
Detail:   Simple MIDI file writing library.
Date:     Feb 3, 2025
*/

#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <list>

class Track(){
    // constructor
    Track(){
        // TODO what container to use here?
        // use std::pair in place of Python's tuple?
        std::list<std::pair> events;
    }

    public:
    void add_event(int tick, std::string event_bytes){}

    void sort_events(){}
}

class MidiWriter(){
    // constructor
    MidiWriter(){
        int TICKS_PER_QUARTER = 480;
        
        // TODO fix vector notation
        std::vector(<Track>) tracks;
        std::map(<int: int>) channel_program;
    }
    
    public:
    void addTrack(){}
    void setChannel(){}
    void addTempo(int channel=0, int program=0){}
    void addNote(
        int track=0,
        int channel=0,
        int start=0,
        int duration=480,
        int pitch=60,
        int velocity = 120
    ){}

    void save(std::string output_filename){}

    private:
    //unsigned short int encode_var_len(int value);
    unsigned char encode_var_len(int value){
        // TODO
        return 0;
    };

    void get_track(){}
}

int main(){
    std::cout << "Hello World!" << std::endl;
    return 0;
}

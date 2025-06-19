// event.h

#pragma once

#include <cstdint>
#include <vector>

struct Event {
    uint64_t tick;             // Absolute tick timestamp of the event
    std::vector<uint8_t> data; // Raw MIDI event data (e.g., [0x90, 60, 127])

    Event(uint64_t t, const std::vector<uint8_t> &d) : tick(t), data(d) {}
};

// track.h

#pragma once

#include <cstdint>
#include <vector>

#include "event.h"

struct Track
{
    void add_event(uint64_t tick, const std::vector<uint8_t> &event_data);
    void sort_events();
    const std::vector<Event> &get_events() const;

  private:
    std::vector<Event> events;
};

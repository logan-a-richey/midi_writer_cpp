// track.cpp

#include <algorithm>
#include <cstdint>
#include <vector>

#include "event.h"
#include "track.h"

void Track::add_event(uint64_t tick, const std::vector<uint8_t> &event_data)
{
    events.emplace_back(tick, event_data);
}

void Track::sort_events()
{
    std::sort(events.begin(), events.end(),
              [](const Event &a, const Event &b) { return a.tick < b.tick; });
}

const std::vector<Event> &Track::get_events() const
{
    return events;
}

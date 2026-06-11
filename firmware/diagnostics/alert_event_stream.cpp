#include "alert_event_stream.h"

namespace diagnostics {

bool AlertEventStream::isDuplicate(const AlertEvent& e) {
    for (uint8_t i = 0; i < size; i++) {
        if (events[i].id == e.id &&
            events[i].state == e.state) {
            return true;
        }
    }
    return false;
}

void AlertEventStream::push(const Alert& alert) {

    AlertEvent e;
    e.id = alert.id;
    e.state = alert.state;

    if (isDuplicate(e)) return;

    if (size < MAX_EVENTS) {
        events[size++] = e;
    } else {
        // shift left (no heap)
        for (uint8_t i = 1; i < MAX_EVENTS; i++) {
            events[i - 1] = events[i];
        }
        events[MAX_EVENTS - 1] = e;
    }
}

uint8_t AlertEventStream::getEvents(AlertEvent* out, uint8_t max) {
    uint8_t n = (size < max) ? size : max;

    for (uint8_t i = 0; i < n; i++) {
        out[i] = events[i];
    }

    return n;
}

void AlertEventStream::clear() {
    size = 0;
}

}
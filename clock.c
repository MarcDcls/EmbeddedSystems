#include <util/delay.h>

#include "clock.h"
#include "tools.h"

uint16_t big_needle = 0b0011111111111111;
uint16_t little_needle = 0b0000000011111111;

void empty_clock(uint16_t *leds, uint32_t resolution) {
    for (int i = 0; i < resolution; i++) {
        leds[i] = 0b0000000000000000;
    }
    for (int i = 0; i < 12; ++i) {
        leds[i * resolution / 12] = 0b1000000000000000;
    }
}

void seconds_to_time(uint16_t seconds, uint16_t *time) {
    time[0] = seconds / 3600; // hours

    uint16_t remaining_seconds = seconds - time[0] * 3600;
    time[1] = remaining_seconds / 60; // minutes

    remaining_seconds = remaining_seconds - time[1] * 60;
    time[2] = remaining_seconds; // seconds
}

void needle_clock(uint16_t *leds, uint32_t resolution, uint16_t seconds) {
    uint16_t t[3];
    seconds_to_time(seconds, t);

    // Init
    empty_clock(leds, resolution);

    // Seconds
    for (int i = 0; i < resolution * t[2] / 60; ++i) {
        leds[i] = ~leds[i] & 0b1000000000000000;
    }

    // Minutes
    int pos_min = resolution * t[1] / 60 + resolution * t[2] / 3600;
    leds[pos_min] |= big_needle;

    // Hours
    int pos_hours = resolution * t[0] / 12 + resolution * t[1] / 720 + resolution * t[2] / 43200;
    leds[pos_hours] |= little_needle;
}
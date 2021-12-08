#include <avr/io.h>
#include <util/delay.h>
#include <time.h>
#include <avr/interrupt.h>

#include "tools.h"
#include "spi.h"

void blue_led_debug() {
    // Make the blue LED blink 1 time
    DDRD |= _BV(PD6); // Blue LED as output
    PORTD |= _BV(PD6);
    _delay_ms(200);
    PORTD &= ~_BV(PD6);
    _delay_ms(200);
}

void blue_led_blink(int duration) {
    // Make the blue LED blink each second
    // duration are in seconds
    // It's a blocking function
    DDRD |= _BV(PD6); // Blue LED as output
    int elapsed_time = 0;
    while (elapsed_time < duration) {
        PORTD |= _BV(PD6);
        _delay_ms(1000);
        PORTD &= ~_BV(PD6);
        _delay_ms(1000);
        elapsed_time += 2;
    }
}

int is_magnet_dectected() {
    // Return 1 if a magnet is detected by the hall sensor
    // Return 0 otherwise
    DDRD &= ~_BV(PD2); // Hall sensor as input
    if (PIND & _BV(2))
        return 0;
    return 1;
}

void quarter(int duration) {
    struct tm initial_tm;
    struct tm current_tm;
    SPI_MasterInit();
    while (current_tm.tm_sec - initial_tm.tm_sec < duration) {
        if (is_magnet_dectected()) {
            SPI_MasterTransmit(0b1000000000000000);
            _delay_ms(13);
            SPI_MasterTransmit(0x00);
        }
        struct tm current_tm;
    }
}

uint16_t min(uint16_t a, uint16_t b) {
    if (a > b) return b;
    return a;
}

int max(int a, int b) {
    if (a < b) return b;
    return a;
}

int abs(int a) {
    if (a < 0) return -a;
    return a;
}

float mean(int *list, int len) {
    int sum = 0;
    for (int i = 0; i < len; i++) {
        sum += list[i];
    }
    return sum / len;
}

uint16_t power(int x, int n) {
    uint16_t res = 1;
    for (int i = 0; i < n; i++) {
        res *= x;
    }
    return res;
}

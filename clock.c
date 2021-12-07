#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "clock.h"
#include "spi.h"

#define RESOLUTION 256 // imply a minimal framerate of display of 256 * 2 * 20 = ~ 20 kHz

//////////////////////////////////////////////////// TOOLS ////////////////////////////////////////////////////////////

int min(int a, int b) {
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

float mean(int* list, int len){
    int sum=0;
    for (int i=0;i<len;i++){
        sum += list[i];
    }
    return sum/len;
}

void led_debug() {
    // Make the blue LED blink 1 time
    DDRD |= _BV(PD6); // Blue LED as output
    PORTD |= _BV(PD6);
    _delay_ms(200);
    PORTD &= ~_BV(PD6);
    _delay_ms(200);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

//unsigned long global_time = 0; // framerate of our clock is 203 125 Hz
int cycle_duration = 0; // duration of one revolution, updated by passing in front of the magnet

//struct clock cl;

// Hall interrupt
ISR(INT0_vect) {
    cycle_duration = TCNT1;
    TCNT1 = 0;
//    global_time += cycle_duration;
}

void setup_clock() {
    TCCR1B = 0b00000011;    // set prescaler to 64 (minimum possible to avoid overflow on TCNT1)
}

void setup_hall() {
    SREG |= _BV(SREG_I);
    EIMSK |= _BV(INT0);
    DDRD &= ~_BV(PD2); // Hall sensor as input
}

void init_clock(struct clock *cl, int h, int m, int s) {
    cl->hour_offset = h;
    cl->min_offset = m;
    cl->sec_offset = s;
}

void global_counter_to_clock(int global_counter, struct clock *cl) {
    int nb_sec = (int) (global_counter / 200);
    int nb_min = (int) (nb_sec / 60);
    int nb_hour = (int) (nb_min / 60);
    cl->hour = nb_hour % 12;
    cl->min = nb_min - 60 * nb_hour;
    cl->sec = nb_sec - 60 * nb_min;
}

void needle_clock(int h, int m, int s) {
    SPI_MasterInit();
    setup_clock();
    setup_hall();

    int16_t leds[RESOLUTION];
    for (int i = 0; i < RESOLUTION; i++) {
        if (!(i % (RESOLUTION / 12))) {
            leds[i] = 0b1000000000000000;
        }
        else {
            leds[i] = 0b0000000000000000;
        }
    }

    while (1) {
        int pos = (RESOLUTION * TCNT1 / cycle_duration) % RESOLUTION;
        SPI_MasterTransmit(leds[pos]);
    }
}

//void displayMin() {
//    SPI_MasterInit();
//    while (1) {
//        if (is_magnet_dectected()) {
//            for (int i = 0; i < 120; i++) {
//                if (i == 2 * nb_min) {
//                    SPI_MasterTransmit(0b1111111111111111);
//                } else {
//                    if (i <= 2 * nb_sec) {
//                        SPI_MasterTransmit(0b1000000000000000);
//                    }
//                }
//                _delay_ms(0.395);
//                SPI_MasterTransmit(0x00);
//            }
//        }
//    }
//    while (1) {}
//}
//
//void displaySec() {
//    SPI_MasterInit();
//    while (1) {
//        if (is_magnet_dectected()) {
//
//            for (int i = 0; i < 2 * nb_sec; i++) {
//                SPI_MasterTransmit(0b1000000000000000);
//                _delay_ms(0.38);
//                SPI_MasterTransmit(0x00);
//            }
//        }
//    }
//    while (1) {}
//}
#include <avr/io.h>
#include <util/delay.h>
#include <time.h>
#include <avr/interrupt.h>
#include <string.h>

#include "spi.h"
#include "usart.h"
#include "bluetooth.h"
#include "clock.h"
#include "tools.h"

const uint32_t RESOLUTION = 240; // imply a minimal framerate of display of 256 * 2 * 20 = ~ 20 kHz

unsigned long global_time = 0; // framerate of our clock is 203 125 Hz
uint16_t cycle_duration = 20000; // duration of one revolution, updated by passing in front of the magnet

struct ring_buffer ring_tx;
struct ring_buffer ring_rx;

// Hall interrupt
ISR(INT0_vect) {
    cycle_duration = TCNT1;
    TCNT1 = 0;
}

// Bluetooth interrupt (writing)
ISR(USART_UDRE_vect) {
    uint8_t data = read_inter(&ring_tx);
    UDR0 = data;
    if (ring_tx.write = ring_tx.read) {
        UCSR0B &= ~_BV(UDRIE0); // disable buffer-available interruption
    }
}

// Bluetooth interrupt (reading)
ISR(USART_RX_vect) {
    unsigned char data = UDR0;
    write_inter(data, &ring_rx);
}

void setup_clock() {
    TCCR1B = 0b00000011; // set prescaler to 64 (minimum possible to avoid overflow on TCNT1)
}

void setup_hall() {
    SREG |= _BV(SREG_I);
    EIMSK |= _BV(INT0);
    EICRA |= _BV(ISC01) | _BV(ISC01);
    DDRD &= ~_BV(PD2); // Hall sensor as input
}

int main() {
    SPI_MasterInit();
    setup_clock();
    setup_hall();
    setup_bluetooth(&ring_tx, &ring_rx);

    uint16_t leds[RESOLUTION];

    for (int i = 0; i < RESOLUTION; i++) {
        if (!(i % (RESOLUTION / 12))) {
            leds[i] = 0b1000000000000000;
        } else {
            leds[i] = 0b0000000000000000;
        }
    }

    while (1) {
    uint32_t pos = min(RESOLUTION - 1, (RESOLUTION * TCNT1) / cycle_duration);
    SPI_MasterTransmit(leds[pos]);

//        unsigned char data[5];
//        data[0] = 't';
//        data[1] = 'e';
//        data[2] = 's';
//        data[3] = 't';
//        data[4] = '\0';
////        itoa(pos, data, 10);
//        for (int i = 0; i < strlen(data); i++) {
//            ring_buffer_write(data[i], &ring_tx);
//        }
//        ring_buffer_write('E', &ring_tx);

    }
    return 0;
}
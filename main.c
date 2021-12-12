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

const uint32_t resolution = 240; // imply a minimal framerate of display of 240 * 2 * 20 = ~ 20 kHz

unsigned long global_time = 0;   // framerate of our clock is 50781.25 Hz -> can increment for ~ 24 hours before overflow
uint16_t cycle_duration = 20000; // duration of one revolution, updated by passing in front of the magnet
uint16_t nb_sec;
uint16_t iniSec;
uint16_t iniMinutes;
uint16_t iniHour;

struct ring_buffer ring_tx;
struct ring_buffer ring_rx;

// Hall interrupt
ISR(INT0_vect)
{
    cycle_duration = TCNT1;
    TCNT1 = 0;
    global_time += cycle_duration;
}

// Bluetooth interrupt (writing)
ISR(USART_UDRE_vect)
{
    uint8_t data = read_inter(&ring_tx);
    UDR0 = data;
    if (ring_tx.write = ring_tx.read)
    {
        UCSR0B &= ~_BV(UDRIE0); // disable buffer-available interruption
    }
}

// Bluetooth interrupt (reading)
ISR(USART_RX_vect)
{
    unsigned char data = UDR0;
    write_inter(data, &ring_rx);
}

void setup_clock()
{
    TCCR1B = 0b00000100; // set prescaler to 256 (no overflow on TCNT1)
}

void setup_hall()
{
    SREG |= _BV(SREG_I);
    EIMSK |= _BV(INT0);
    EICRA |= _BV(ISC01) | _BV(ISC01);
    DDRD &= ~_BV(PD2); // Hall sensor as input
}

int main()
{
    SPI_MasterInit();
    setup_clock();
    setup_hall();
    setup_bluetooth(&ring_tx, &ring_rx);

    uint16_t leds[resolution];
    empty_clock(leds, resolution);
    while (1)
    {
        // Mise à jour de l'heure
        if ((uint16_t)(global_time / 50781) > nb_sec)
        {
            iniSec = 0;
            iniMinutes = 59;
            iniHour = 6;
            needle_clock(leds, resolution, nb_sec + 3600 * iniHour + 60 * iniMinutes + iniSec);
            //displayImg(leds, resolution, nb_sec)
            nb_sec++;
        }

        // Display
        uint32_t pos = min(resolution - 1, (resolution * TCNT1) / cycle_duration);
        SPI_MasterTransmit(leds[pos]);

        // Bluetooth
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
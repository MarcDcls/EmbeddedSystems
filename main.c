#include <avr/io.h>
#include <util/delay.h>

#define TIMER 1000

void init_pins() {
    // Init all used pins
    DDRD |= _BV(PD6); // Blue LED as output
    DDRD &= ~_BV(PD2); // Hall sensor as input
}

void blue_led_blink(int duration) {
    // Make the blue LED blink each second
    // duration are in seconds
    int elapsed_time = 0;
    while (elapsed_time < duration){
        PORTD |= _BV(PD6);
        _delay_ms(TIMER);
        PORTD &= ~_BV(PD6);
        _delay_ms(TIMER);
        elapsed_time += 2;
    }
}

int main() {
    init_pins();
    blue_led_blink(120);
}
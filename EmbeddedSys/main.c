#include <avr/io.h>
#include <util/delay.h>

#define TIMER 1000

int main() {
    // Enable the pins
    DDRD |= _BV(PD6);

    while(1){
        PORTD |= _BV(PD6);
        _delay_ms(TIMER);
        PORTD &= ~_BV(PD6);
        _delay_ms(TIMER);
    }
}
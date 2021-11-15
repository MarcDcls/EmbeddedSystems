#include <avr/io.h>
#include <util/delay.h>

#define TIMER 1000

int main()
{
    // Enable the pins
    DDRD |= _BV(PD6);
    // DDRD |= _BV(PD2);

    while (1)
    {
        // PORTD |= _BV(PD6);
        // _delay_ms(TIMER);
        // PORTD &= ~_BV(PD6);
        // _delay_ms(TIMER);
        if (PIND & _BV(2))
        {
            // PORTD |= _BV(PD6);
            PORTD &= ~_BV(PD6);
        }
        else
        {
            PORTD |= _BV(PD6);
            // PORTD &= ~_BV(PD6);
        }
    }
}
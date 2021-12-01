#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "clock.h"

int nb_sec = 0;
int counter = 0;

ISR(TIMER0_COMPA_vect){
	// PORTD |= _BV(PD6);
    counter++;
    if (counter==3250/2){
    counter=1;
    nb_sec++;
    PORTD =(~(PORTD)&_BV(PD6))|(PORTD&~_BV(PD6));
	}
}

void init_clock(void){

	cli();
	DDRD |= _BV(PD6);

	nb_sec = 1;
	OCR0A=0b01111101;// 125 : plus grand diviseur de 203125 inférieur à 255 (valeur max que peut prendre le timer)
	//initalisation du timer (dont prescaler)
	TCCR0A &= ~(1<<WGM20);//set mode to output compare
	TCCR0A |= (1<<WGM21);//set mode to output compare
	TCCR0B |=(1<<CS01);// set precaler to 64
	TCCR0B |=(1<<CS00);// set precaler to 64
	TCCR0B &=~(1<<CS02);// set precaler to 64
	TIMSK0 |=(1<<TOIE0);//enable overflow 
	counter = 1;

	sei();
}

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#include "clock.h"

int nb_sec = 0;
int nb_min = 0;
int nb_h = 0;
int counter = 0;

void setup(){
	// cli();
	DDRD |= _BV(PD6);
	// blue_led_debug();
	// nb_sec = 1;
	// OCR0A=0b01111101;// 125 : plus grand diviseur de 203125 inférieur à 255 (valeur max que peut prendre le timer)
	//initalisation du timer (dont prescaler)
	// TCCR0A &= ~(1<<WGM20);//set mode to output compare
	// TCCR0A |= (1<<WGM21);//set mode to output compare
	// TCCR0B |=(1<<CS01);// set precaler to 64
	// TCCR0B |=(1<<CS00);// set precaler to 64
	// TCCR0B &=~(1<<CS02);// set precaler to 64
	// TIMSK0 |=(1<<TOIE0);//enable overflow
	// TIMSK0 =(1 << OCIE0A);;//enable overflow
	// counter = 1;
	TCCR0A = 0b00000010;    // C1:: timer 0 mode 2 - CTC
    TCCR0B = 0b00000100;    // C2:: set prescaler to 256
    OCR0A = 250;            // C3:: number of ticks in Output Compare Register
    TIMSK0 = 0b00000010;    // C4:: trigger interrupt when ctr (TCNT0) >= OCR0A
	sei();
}
ISR(TIMER0_COMPA_vect){
	// PORTD |= _BV(PD6);
	// SPI_MasterInit();
	// blue_led_debug();
    counter++;
    if (counter>200){
    	counter=0;

		if (nb_sec>59){
			nb_sec=0;
			nb_min++;
		}
    	nb_sec++;
		// nb_sec=3;
		// displaySec(nb_sec);
    	PORTD =(~(PORTD)&_BV(PD6))|(PORTD&~_BV(PD6));
    // (~SPI_MasterTransmit(0x00))|(SPI_MasterTransmit(0b1000000000000000));
	}
}

void displayMin(){
    SPI_MasterInit();
    while (1) {
        if(is_magnet_dectected()){
            for (int i=0;i < 120; i++){
				if (i == 2*nb_min){
					SPI_MasterTransmit(0b1111111111111111);
				}else{
					if (i <= 2*nb_sec){
						SPI_MasterTransmit(0b1000000000000000);
					}
				}

				// else{
				// 	SPI_MasterTransmit(0b0000000000000000);
				// }
                _delay_ms(0.395);
                SPI_MasterTransmit(0x00);
            }
        }
    }
}

void displaySec(){
    SPI_MasterInit();
    while (1) {
        if(is_magnet_dectected()){

            for (int i=0;i < 2* nb_sec; i++){
					SPI_MasterTransmit(0b1000000000000000);
				_delay_ms(0.38);
            	SPI_MasterTransmit(0x00);
            }

        }
    }
}

void init_clock() {
	setup();
	// displaySec();

	displayMin();

	// blue_led_debug();

	while(1){}
  //in this way you can count 1 second because the interrupt request is each 1ms
//   if (counter >= 1000) {
//     PORTD =(~(PORTD)&_BV(PD6))|(PORTD&~_BV(PD6));
//   }
}

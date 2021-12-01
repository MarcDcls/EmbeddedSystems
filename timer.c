#include <avr/io.h>
#include <util/delay.h>
#define FOSC 13000000 // Clock Speed
#define BAUD 38400
#define MYUBRR FOSC/16/BAUD-1

void USART_Init(unsigned int ubrr)
{
/*Set baud rate */
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
/*Enable receiver and transmitter */
UCSR0B = (1<<RXEN0)|(1<<TXEN0);
/* Set frame format: 8data, 2stop bit */
UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

void USART_Transmit(unsigned char data)
{
/* Wait for empty transmit buffer */
while (!(UCSR0A & (1<<UDRE0)))
;
/* Put data into buffer, sends the data */
UDR0 = data;
}

unsigned char USART_Receive(void)
{
/* Wait for data to be received */
while (!(UCSR0A & (1<<RXC0)))
;
/* Get and return received data from buffer */
return UDR0;
}

void main(void)
{
	DDRD |= _BV(PD6);
	USART_Init(MYUBRR);
	unsigned char data=0xFF;
	USART_Transmit(data);
	
	int sec=1;
	OCR0A=01111101;// 125 : plus grand diviseur de 203125 inférieur à 255 (valeur max que peut prendre le timer)
	//initalisation du timer (dont prescaler)
	TCCR0A &= ~(1<<WGM20);//set mode to output compare
	TCCR0A |= (1<<WGM21);//set mode to output compare
	TCCR0B |=(1<<CS01);// set precaler to 64
	TCCR0B |=(1<<CS00);// set precaler to 64
	TCCR0B &=~(1<<CS02);// set precaler to 64
	TIMSK0 |=(1<<TOIE0);//enable overflow flag
	int cmpt=1;
	while(1){
		if (TIFR0 &(1<<OCF0A)){
			TIFR0 |= (1<<OCF0A);
			cmpt++;
		}
		if (cmpt==3250){
			cmpt=1;
			sec++;
			PORTD =(~(PORTD)&_BV(PD6))|(PORTD&~_BV(PD6));

			USART_Transmit((unsigned char) sec);
		}

		
		
		
	}
}

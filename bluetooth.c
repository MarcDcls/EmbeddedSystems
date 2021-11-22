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

void USART_Transmit(const unsigned char data)
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
while(1){
    _delay_ms(2000);
	unsigned char data='h';
    USART_Transmit(data);
	unsigned char data1='e';
    USART_Transmit(data1);
    unsigned char data2='l';
    USART_Transmit(data2);
    unsigned char data3='l';
    USART_Transmit(data3);
    unsigned char data4='0';
    USART_Transmit(data4);
    unsigned char data5=' ';
    USART_Transmit(data5);
}

}
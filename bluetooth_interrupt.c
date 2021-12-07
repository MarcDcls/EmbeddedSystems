#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define FOSC 13000000 // Clock Speed
#define BAUD 38400
#define RING_BUFFER_SIZE 64
#define MYUBRR FOSC/16/BAUD-1


struct ring_buffer{
    uint8_t buffer[RING_BUFFER_SIZE];
    uint8_t read;
    uint8_t write;
};

struct ring_buffer ring_tx;
struct ring_buffer ring_rx;

void ring_buffer_init(struct ring_buffer *rb){
    rb->read=0;
    rb->write=0;
}

void USART_Init(unsigned int ubrr)
{

/*Set baud rate */
UBRR0H = (unsigned char)(ubrr>>8);
UBRR0L = (unsigned char)ubrr;
/* Enable receiver and transmitter */
UCSR0B = (1<<RXEN0)|(1<<TXEN0);
/* Set frame format: 8data, 2stop bit */
UCSR0C = (1<<USBS0)|(3<<UCSZ00);
}

uint8_t read_inter(struct ring_buffer *rb){
	uint8_t data;
    if (rb->write!=rb->read){
        data=rb->buffer[rb->read];
        rb->read+=1;
        if(rb->read>= RING_BUFFER_SIZE){
            rb->read=0;
        }
    }
    return data;
}

void write_inter(uint8_t data, struct ring_buffer *rb){
    rb->buffer[rb->write]=data;
    rb->write+=1;
    if(rb->write>= RING_BUFFER_SIZE){
        rb->write=0;
    }
}

void ring_buffer_write(uint8_t data, struct ring_buffer *rb){
    write_inter(data, rb);
    //enable buffer-available interruption
    UCSR0B |= _BV(UDRIE0);
}

uint8_t ring_buffer_read(struct ring_buffer *rb){
    uint8_t data=read_inter(rb);
    return data;
}


ISR(USART_UDRE_vect) {
	uint8_t data=read_inter(&ring_tx);
    UDR0= data;
    if (ring_tx.write=ring_tx.read){
        //disable buffer-available interruption
        UCSR0B &= ~_BV(UDRIE0);
    }
	//PORTD |= _BV(PD6);

}

ISR(USART_RX_vect){
	unsigned char data= UDR0;
    write_inter(data, &ring_rx);

}

void main(){
	DDRD |= _BV(PD6);
	USART_Init(MYUBRR);
	
    sei();
    UCSR0B |= _BV(RXCIE0);
    
    ring_buffer_init(&ring_tx);
    ring_buffer_init(&ring_rx);

    while(1){
		unsigned char data='h';
		ring_buffer_write(data, &ring_tx);
		_delay_ms(1000);
		if (ring_rx.buffer[0]=='a' && ring_rx.buffer[1]=='b'){
			PORTD |= _BV(PD6);
		}
	}
    

}

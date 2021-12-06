#include <avr/io.h>
#define FOSC 13000000 // Clock Speed
#define BAUD 938400
#define RING_BUFFER_SIZE 64
#define MYUBRR FOSC/16/BAUD-1
struct ring_buffer{
    uint8_t buffer(RING_BUFFER_SIZE);
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

void ring_buffer_write(uint8_t data, struct ring_buffer *rb){
    write_inter(data, rb);
    //enable buffer-available interruption
    UCSR0B |= _BV(UDRIE0);
}
uint8_t ring_buffer_read(struct ring_buffer *rb){
    uint8_t data;
    read_inter(data,rb);
    return data;
}
void read_inter(uint8_t data, struct ring_buffer *rb){
    if (rb->write!=rb->read){
        data=rb->buffer[rb->read];
        rb->read+=1;
        if(rb->read>= RING_BUFFER_SIZE){
            rb->read=0;
        }
    }
}
void write_inter(uint8_t data, struct ring_buffer *rb){
    rb->buffer[rb->write]=data;
    rb->write+=1;
    if(rb->write>= RING_BUFFER_SIZE){
        rb->write=0;
    }
}
ISR(USART_UDRE_vect) {
    read_inter(UDR0, &ring_tx);
    if (~(&ring_tx->write!=&ring_tx->read)){
        //disable buffer-available interruption
        UCSR0B &= ~_BV(UDRIE0);
    }
}

ISR(USART_RXC){
    write_inter(UDR0, &ring_rx);
}

void main(){
    sei();
    USART_Init(MYUBRR);
    ring_buffer_init(&ring_tx);
    ring_buffer_init(&ring_rx);
    

}
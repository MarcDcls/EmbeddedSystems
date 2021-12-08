#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "bluetooth.h"
#include "usart.h"

void ring_buffer_init(struct ring_buffer *rb) {
    rb->read = 0;
    rb->write = 0;
}

uint8_t read_inter(struct ring_buffer *rb) {
    uint8_t data;
    if (rb->write != rb->read) {
        data = rb->buffer[rb->read];
        rb->read += 1;
        if (rb->read >= RING_BUFFER_SIZE) {
            rb->read = 0;
        }
    }
    return data;
}

void write_inter(uint8_t data, struct ring_buffer *rb) {
    rb->buffer[rb->write] = data;
    rb->write += 1;
    if (rb->write >= RING_BUFFER_SIZE) {
        rb->write = 0;
    }
}

void ring_buffer_write(uint8_t data, struct ring_buffer *rb) {
    write_inter(data, rb);
    //enable buffer-available interruption
    UCSR0B |= _BV(UDRIE0);
}

uint8_t ring_buffer_read(struct ring_buffer *rb) {
    uint8_t data = read_inter(rb);
    return data;
}

void setup_bluetooth(struct ring_buffer *tx, struct ring_buffer *rx) {
    USART_Init(MYUBRR);

    sei();
    UCSR0B |= _BV(RXCIE0);

    ring_buffer_init(tx);
    ring_buffer_init(rx);
}

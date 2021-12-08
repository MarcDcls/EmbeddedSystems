#ifndef __BLUETOOTH_H
#define __BLUETOOTH_H

#define FOSC 13000000 // Clock Speed
#define BAUD 38400
#define RING_BUFFER_SIZE 64
#define MYUBRR FOSC/16/BAUD-1

struct ring_buffer{
    uint8_t buffer[RING_BUFFER_SIZE];
    uint8_t read;
    uint8_t write;
};

void setup_bluetooth(struct ring_buffer *tx, struct ring_buffer *rx);

uint8_t read_inter(struct ring_buffer *rb);

void write_inter(uint8_t data, struct ring_buffer *rb);

void ring_buffer_write(uint8_t data, struct ring_buffer *rb);

#endif
#include <avr/io.h>
#include <util/delay.h>

#include "spi.h"

#define DDR_SPI DDRB
#define DD_SS DDB2
#define DD_MOSI DDB3
#define DD_MISO DDB4
#define DD_SCK DDB5

void SPI_Clear() {
    // LE high
    PORTC |= _BV(PC2);
    SPDR = 0x00;
    // Wait for transmission complete
    while (!(SPSR & (1 << SPIF)));
    SPDR = 0x00;
    // Wait for transmission complete
    while (!(SPSR & (1 << SPIF)));
    // LE low
    PORTC &= ~_BV(PC2);
}

void SPI_MasterInit(void) {
    DDRC |= _BV(PC1); // OE as an output
    DDRC |= _BV(PC2); // LE as an output

    // OE and LE low
    PORTC &= ~_BV(PC1);
    PORTC &= ~_BV(PC2);

    // Set MOSI and SCK output, all others input
    DDR_SPI = (1 << DD_MOSI) | (1 << DD_SCK) | (1 << DD_SS);

    // Enable SPI, Master, set clock rate fck/16
    SPCR = (1 << SPE) | (1 << MSTR);
    SPI_Clear();
}

void uint16ToChar(uint16_t value, char cData[2]){
    cData[0] = (value >> 8);
    cData[1] = value;
}

void SPI_MasterTransmit(const uint16_t data) {
    // OE high
    PORTC |= _BV(PC1);

    SPI_Clear();
    char cData[2];
    uint16ToChar(data, cData);

    // LE high
    PORTC |= _BV(PC2);

    SPDR = cData[0];
    // Wait for transmission complete
    while (!(SPSR & (1 << SPIF)));

    SPDR = cData[1];
    // Wait for transmission complete
    while (!(SPSR & (1 << SPIF)));

    // LE and OE low
    PORTC &= ~_BV(PC2);
    PORTC &= ~_BV(PC1);
}
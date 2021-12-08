CC = avr-gcc
OBJCOPY = avr-objcopy
CFLAGS = -Os -mmcu=atmega328p -DF_CPU=13000000
PORT = /dev/ttyACM0
BIN = main
OBJS = main.o spi.o usart.o clock.o bluetooth.o tools.o

all: $(BIN).hex

$(BIN).hex: $(BIN).elf
	$(OBJCOPY) -O ihex -R .eeprom $< $@

$(BIN).elf: $(OBJS)
	$(CC) -o $@ $^ $(CFLAGS)

install: ${BIN}.hex
	avrdude -F -V -c usbasp -p ATMEGA328P -P $(PORT) -b 13000000 -U flash:w:$<

clean:
	rm -f ${BIN}.elf ${BIN}.hex ${OBJS}

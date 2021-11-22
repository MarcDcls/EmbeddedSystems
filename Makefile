CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-Os -mmcu=atmega328p -DF_CPU=13000000
PORT=/dev/ttyACM0

all: main.hex

main.hex: main.elf
	$(OBJCOPY) -O ihex -R .eeprom main.elf main.hex

main.elf: main.c
	$(CC) $(CFLAGS) -o main.elf main.c

install: main.hex
	avrdude -F -V -c usbasp -p ATMEGA328P -P $(PORT) -b 13000000 -U flash:w:main.hex:i

clean:
	rm -f *.elf *.hex *.o
CC=avr-gcc
OBJCOPY=avr-objcopy
DEVICE=atmega32
CFLAGS=-std=c99 -Os -Wall -mmcu=${DEVICE}

# list all srcs
SRCS=main.c
OBJS=$(SRCS:.c=.o)

# name of .elf, .hex, .o files
MAIN=main

.PHONY: depend clean

all: $(MAIN).hex

$(MAIN).hex: $(MAIN).elf
	$(OBJCOPY) -j .text -j .data -O ihex $(MAIN).elf $(MAIN).hex

$(MAIN).elf: ${OBJS}
	$(CC) $(CFLAGS) -o $(MAIN).elf $(OBJS)

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm *.o *.elf *.hex

depend: ${SRCS}
	makedepend $^
# DO NOT DELETE

main.o: /usr/include/stdlib.h /usr/include/string.h

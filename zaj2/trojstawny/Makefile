CC=avr-g++
OBJCOPY=avr-objcopy
DEVICE=atmega32
CFLAGS=-std=c++14 -Os -Wall -pedantic -mmcu=${DEVICE}

# list all srcs
SRCS=main.c LCD_HD44780.c
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

main.o: GLOBAL.H /usr/include/stdlib.h /usr/include/string.h
main.o: /usr/include/stdio.h LCD_HD44780.h GLOBAL.h
LCD_HD44780.o: LCD_HD44780.h GLOBAL.h /usr/include/stdio.h

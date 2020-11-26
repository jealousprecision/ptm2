#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>

#include "LCD_HD44780.h"

#ifndef _BV
#define _BV(bit)				(1<<(bit))
#endif

#ifndef sbi
#define sbi(reg,bit)		reg |= (_BV(bit))
#endif

#ifndef cbi
#define cbi(reg,bit)		reg &= ~(_BV(bit))
#endif

int _sp = 600;
int _pv = 0;
bool _cv = false;
int _e = 0;  // (-1000, 1000)
int _h = 8;  //

void wyswietl()
{
    char buffer[16];
    char floatBuffer[8];

    /**********************/
    /* SP=XX% PV=XX.X%
    /* H=X%   E=-XX.X%
    /***********************/

    LCD_HD44780::clear();
    //LCD_HD44780::writeText("Hello");


    //dtostrf(_pv / 10, 2, 1, floatBuffer);
    sprintf(buffer, "SP=%ld%% PV=%i%%", _sp / 10, _pv / 10);
    LCD_HD44780::writeText(buffer);

    /*
    LCD_HD44780::goTo(0, 1);
    dtostrf(_e / 10, 2, 1, floatBuffer);
    sprintf(buffer, "H=%i%%   E=%s%%", _h, floatBuffer);
    LCD_HD44780::writeText(buffer);
    */

}

/*
 *  Denis Firat
 *  Maksymilian Kadukowski
 */

int main()
{
    DDRD |= 0xff;
    ADMUX = 0x40;

    LCD_HD44780::init();

    while (true)
    {
        if (PINB & _BV(PB4))
        {
            _sp = 500;
        }

        if (PINB & _BV(PB5))
        {
            _sp = 400;
        }

        if (PINB & _BV(PB6))
        {
            _h = 4;
        }

        if (PINB & _BV(PB7))
        {
            _h = 10;
        }

        ADCSRA = 0xe0;

        _pv = ADC / 1.023;
        _e = _sp - _pv;

        wyswietl();
        _delay_ms(250);
    }
}

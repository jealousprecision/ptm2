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

int _sp = .6 * 1023 + 1;
int _pv = 0;
bool _cv = false;
int _e = 0;  // (-1023, 1023)
int _h = 8;  //

void wyswietl()
{
    char buffer[128];
    char floatBuffer[64];

    /**********************/
    /* SP=XX% PV=XX.X%
    /* H=X%   E=-XX.X%
    /***********************/

    LCD_HD44780::clear();

    dtostrf(_pv * 100.0 / 1023, 2, 1, floatBuffer);
    sprintf(buffer, "SP=%ld%% PV=%s%%", _sp * 100l / 1023, floatBuffer);
    LCD_HD44780::writeText(buffer);

    LCD_HD44780::goTo(0, 1);
    dtostrf(_e * 100.0 / 1023, 2, 1, floatBuffer);
    sprintf(buffer, "H=%i%%   E=%s%%", _h, floatBuffer);
    LCD_HD44780::writeText(buffer);
}

/*
 *  Denis Firat
 *  Maksymilian Kadukowski
 */

int main()
{
    sbi(DDRB, PB3);
    ADMUX = 0x40;

    LCD_HD44780::init();

    while (true)
    {
        if (PINB & _BV(PB4))
        {
            _sp = .5 * 1023 + 1;
        }

        if (PINB & _BV(PB5))
        {
            _sp = .4 * 1023 + 1;
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
        while (!ADIF) {}

        _pv = ADC;
        _e = _sp - _pv;

        if (!_cv && _e < (-_h * 1023 / 100 / 2))
        {
            _cv = true;
            cbi(PORTB, PB3);
        }

        if (_cv && _e > ((_h * 1023 / 100 / 2)))
        {
            _cv = false;
            sbi(PORTB, PB3);
        }

        wyswietl();
        _delay_ms(100);
    }
}

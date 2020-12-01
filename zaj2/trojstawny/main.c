#include "GLOBAL.H"
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <util/delay.h>

#include "LCD_HD44780.h"

#define FROM_ADC_TO_PV_UNITS(adc_val) ( (int)((adc_val) / 1.023 + 0.5) )
#define H_IN_PV_UNITS (_h * 10)

int _sp = 600; // (0, 1000)
int _pv = 600; // (0, 1000)
bool _cv[2] = {false, false};  // .0 = heating, .1 = cooling
int _e = 0;  // (-1000, 1000)
int _h = 8;  // (0, 100)

void display();

/*
 *  Denis Firat
 *  Maksymilian Kadukowski
 */

int main()
{
    DDRC |= _BV(PC0) | _BV(PC1);
    DDRB &= ~(_BV(PB4) | _BV(PB5) | _BV(PB6) | _BV(PB7));

    sbi(ADCSRA, ADEN);  // enable ADC

    ADMUX &= ~(_BV(MUX0) | _BV(MUX1) | _BV(MUX2) | _BV(MUX3) | _BV(MUX4));  // select PA0 as analog input
    sbi(ADMUX, REFS0);  // ADC to measure with respect to AVcc

    LCD_HD44780::init();

    while (true)
    {
        if (!(PINB & _BV(PB4))) _sp = 500;
        if (!(PINB & _BV(PB5))) _sp = 400;
        if (!(PINB & _BV(PB6))) _h = 4;
        if (!(PINB & _BV(PB7))) _h = 10;

        sbi(ADCSRA, ADSC);
        while (ADCSRA & ADSC) {}

        _pv = FROM_ADC_TO_PV_UNITS(ADC);
        _e = _sp - _pv;

        if ( _cv[0] && _e <      H_IN_PV_UNITS) _cv[0] = false;
        if (!_cv[0] && _e >  2 * H_IN_PV_UNITS) _cv[0] = true;
        if (!_cv[1] && _e < -2 * H_IN_PV_UNITS) _cv[1] = true;
        if ( _cv[1] && _e > -1 * H_IN_PV_UNITS) _cv[1] = false;

        if (_cv[0])
            sbi(PORTC, PC0);
        else
            cbi(PORTC, PC0);

        if (_cv[1])
            sbi(PORTC, PC1);
        else
            cbi(PORTC, PC1);

        display();
        _delay_ms(100);
    }
}

void display()
{
    char buffer[32];
    char floatBuffer[16];
    char pvBuffer[16];

    LCD_HD44780::clear();

    dtostrf(_pv / 10.0, 2, 1, pvBuffer);
    snprintf(buffer, sizeof(buffer), "SP=%2i%%  PV=%s%%", _sp / 10, pvBuffer);
    LCD_HD44780::writeText(buffer);

    LCD_HD44780::goTo(0, 1);
    if (_e >= 0)
    {
        floatBuffer[0] = '+';
        dtostrf(_e / 10.0, 2, 1, floatBuffer + 1);
    }
    else
    {
        dtostrf(_e / 10.0, 2, 1, floatBuffer);
    }

    snprintf(buffer, sizeof(buffer), " H=%2i%%  E=%s%%", _h, floatBuffer);
    LCD_HD44780::writeText(buffer);
}

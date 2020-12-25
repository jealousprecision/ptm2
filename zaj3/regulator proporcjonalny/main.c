#include "GLOBAL.H"
#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <util/delay.h>

#include "LCD_HD44780.h"

#define FROM_ADC_TO_PV_UNITS(adc_val) ( (int)((adc_val) / 1.023 + 0.5) )

int _sp = 600; // (0, 1000)
int _pv = 600; // (0, 1000)
int _cv = 50;  // (0, 100)
int _e = 0;  // (-1000, 1000)
int _xp = 20; // (0, 100)

void display();
int clamp(int min, int val, int max);

/*
 *  Denis Firat
 *  Maksymilian Kadukowski
 */

int main()
{
    DDRC |= _BV(PC0);
    DDRB &= ~(_BV(PB4) | _BV(PB5) | _BV(PB6) | _BV(PB7));

    sbi(ADCSRA, ADEN);  // enable ADC

    ADMUX &= ~(_BV(MUX0) | _BV(MUX1) | _BV(MUX2) | _BV(MUX3) | _BV(MUX4));  // select PA0 as analog input
    sbi(ADMUX, REFS0);  // ADC to measure with respect to AVcc

    LCD_HD44780::init();

    uint64_t time_passed = 0;
    while (true)
    {
        if (!(PINB & _BV(PB4))) _sp = 500;
        if (!(PINB & _BV(PB5))) _sp = 400;
        if (!(PINB & _BV(PB6))) _xp = 30;
        if (!(PINB & _BV(PB7))) _xp = 40;

        sbi(ADCSRA, ADSC);
        while (ADCSRA & ADSC)
        {
            // wait for ADC conversion to end
        }

        _pv = FROM_ADC_TO_PV_UNITS(ADC);
        _e = _sp - _pv;
        _cv = clamp(0, (int)(100.0 / _xp * _e / 10.0 + 50.0 + 0.5), 100);

        for (int i = 0; i < 100; i += 5)
        {
            if (i < _cv)
                sbi(PORTC, PC0);
            else
                cbi(PORTC, PC0);

            //_delay_ms(1000 * 2 / 20);
            _delay_ms(1);
        }

        time_passed += 20;
        if (!(time_passed % 100))
        {
            //display();
        }

        display();
    }
}

void display()
{
    char buffer[17];
    char floatBuffer[16];

    LCD_HD44780::clear();

    dtostrf(_pv / 10.0, 2, 1, floatBuffer);
    snprintf(buffer, sizeof(buffer), "SP=%2i%%  PV=%s%%", _sp / 10, floatBuffer);
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

    snprintf(buffer, sizeof(buffer), "cv=%2i%%  E=%s%%", _cv, floatBuffer);
    LCD_HD44780::writeText(buffer);
}

int clamp(int min, int val, int max)
{
    if (val < min)
    {
        return min;
    }
    if (max < val)
    {
        return max;
    }
    return val;
}

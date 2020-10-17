#define F_CPU 8000000UL

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include <util/delay.h>

#include "Button.hpp"
#include "State.hpp"

#ifndef _BV
#define _BV(bit)				(1<<(bit))
#endif

#ifndef sbi
#define sbi(reg,bit)		reg |= (_BV(bit))
#endif

#ifndef cbi
#define cbi(reg,bit)		reg &= ~(_BV(bit))
#endif

// CONFIG

#define PD_DIODE PD5
#define PD_BUTTON PD6

int read_button_state()
{
    return PIND & (_BV(PD_BUTTON));
}

int main()
{
    sbi(DDRD, PD_DIODE);
    cbi(DDRD, PD_BUTTON);

    Button button;
    State state = State::Blinking;
    unsigned long timePassed = 0ul;  // in ms

    while (true)
    {
        switch (state)
        {
            case State::Blinking:
                if ((timePassed / 500) % 2)
                    sbi(PORTD, PD_DIODE);
                else
                    cbi(PORTD, PD_DIODE);
                break;

            case State::On:
                sbi(PORTD, PD_DIODE);
                break;

            case State::Off:
                cbi(PORTD, PD_DIODE);
                break;
        }

        button.updateState(read_button_state(), timePassed);

        if (button.popWasPressed())
        {
            state = nextState(state);
        }

        timePassed += 1;
        _delay_ms(1);  // hack, timer should be impl as interrupt
    }
}

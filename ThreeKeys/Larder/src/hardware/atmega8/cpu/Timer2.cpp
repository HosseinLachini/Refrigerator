/*
    Pars-Refrigerator - open source firmware for a variety of Pars Refrigerators
    Copyright (C) 2019 Hossein Lachini. All right reserved.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "Time.h"
#include "Hardware.h"
#include "atomic.h"


// time measurement - It uses atmega8/Timer2 to measure TIMER_INTERRUPT_PERIOD_MICROSECONDS

ISR(TIMER2_COMP_vect)
{
    Time::callback();
}


void Time::initialize()
{
#if F_CPU != 8000000
#error "F_CPU != 8000000 - not implemented"
#endif
#if TIMER_INTERRUPT_PERIOD_MICROSECONDS != 500
#error "TIMER_INTERRUPT_PERIOD_MICROSECONDS != 500 - not implemented"
#endif

    TCCR2=(1<<WGM21);               //Clear Timer on Compare Match (CTC) Mode
    TCCR2|=(1 << CS22);             //clk/64 (From prescaler)

    TCNT2=0;
    OCR2=TIMER_INTERRUPT_PERIOD_MICROSECONDS/8 - 1;

    TIMSK|=(1<<OCIE2);              //OCIE2: Timer/Counter2 Output Compare Match Interrupt Enable
}

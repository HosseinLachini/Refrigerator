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
#include "AnalogInputsADC.h"
#include "IO.h"
#include <avr/pgmspace.h>

/// Temp = A * ADC + B
#define NTC2K7_A_FACTOR ((int32_t)(-86))
#define NTC2K7_B_FACTOR ((int32_t)(41270))


// Voltage Reference: AVCC pin
#define ADC_VREF_AVCC   ((0<<REFS1) | (1<<REFS0) | (0<<ADLAR))
// Voltage Reference: AREF pin
#define ADC_VREF_AREF   ((0<<REFS1) | (0<<REFS0) | (0<<ADLAR))
// Voltage Reference: Int. 2.56V, cap. on AREF
#define ADC_VREF_INTE   ((1<<REFS1) | (1<<REFS0) | (0<<ADLAR))

const uint16_t NTC2K7[27] PROGMEM = {923, 890, 848, 799, 742, 680, 614, 546, 480, 416, 358, 305, 258, 217, 183, 153, 129, 108, 91, 76, 64, 55, 46, 39, 34, 29, 25};

void AnalogInputsADC::initialize()
{
    ADMUX = ADC_VREF_AVCC;
    // ADC Clock frequency: 125/000 kHz
    // ADC Prescaler 64
    ADCSRA = (1<<ADEN) | (0<<ADSC) | (0<<ADFR) | (0<<ADIF) | (0<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (0<<ADPS0);
}

uint16_t AnalogInputsADC::readADC(uint8_t pin)
{
    ADMUX = ADC_VREF_AVCC | IO::pinToADC(pin);
    // Start the AD conversion
    ADCSRA |= (1<<ADSC);
    while(ADCSRA & (1<<ADSC));
    return ADCW;
}

int16_t AnalogInputsADC::readTemp(uint8_t pin)
{
    uint16_t in = readADC(pin);
    uint8_t idx;
    if((in > NTC2K7[0]) || (in < NTC2K7[26]))
        return AnalogInputsADC::FAIL;

    //return (((NTC2K7_A_FACTOR * in) + NTC2K7_B_FACTOR) / 100);

    for(idx = 0; pgm_read_word(&(NTC2K7[idx])) > in; idx++);
    if(NTC2K7[idx] == in)
    {
        return ((idx * 5 - 40)*10);
    }
    //return (((idx - 1) * 5 - 40 + (pgm_read_word(&(NTC2K7[idx - 1])) - in) * 5 / (pgm_read_word(&(NTC2K7[idx - 1])) - pgm_read_word(&(NTC2K7[idx]))))*10);
    return (((idx - 1) * 50 - 400 + (pgm_read_word(&(NTC2K7[idx - 1])) - in) * 50 / (pgm_read_word(&(NTC2K7[idx - 1])) - pgm_read_word(&(NTC2K7[idx])))));
}

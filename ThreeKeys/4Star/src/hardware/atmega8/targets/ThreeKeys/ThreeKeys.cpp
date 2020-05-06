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
#include "Hardware.h"
#include "IO.h"
#include "AnalogInputsADC.h"


#ifndef PINS_H_
#error pins not defined (include *pins.h header in your HardwareConfig.h)
#endif



void hardware::initializePins()
{
    setBuzzer(0);

    IO::analogReference(EXTERNAL);

    IO::pinMode(BUZZER_PIN, OUTPUT);
    IO::pinMode(COMPRESSOR_PIN, OUTPUT);
    IO::pinMode(LAMP_PIN, OUTPUT);

    IO::pinMode(DOOR_PIN, INPUT);

    IO::pinMode(SEG_A_PIN, INPUT);
    IO::pinMode(SEG_B_PIN, INPUT);
    IO::pinMode(SEG_C_PIN, OUTPUT);
    IO::pinMode(SEG_D_PIN, OUTPUT);
    IO::pinMode(SEG_E_PIN, OUTPUT);
    IO::pinMode(SEG_F_PIN, INPUT);
    IO::pinMode(SEG_G_PIN, OUTPUT);
    IO::pinMode(SEG_CA1_PIN, INPUT_PULLUP);
    IO::pinMode(SEG_CA2_PIN, INPUT_PULLUP);

    IO::pinMode(LED_EN_PIN, OUTPUT);

    //IO::digitalWrite(SEG_CA1_PIN, HIGH);
    //IO::digitalWrite(SEG_CA2_PIN, HIGH);
    IO::digitalWrite(LED_EN_PIN, HIGH);
}

void hardware::initialize()
{
    setBuzzer(0);
    setCompressor(0);
    setLamp(0);
    AnalogInputsADC::initialize();
}

uint8_t hardware::IsDoorOpen()
{
    return (IO::digitalRead(DOOR_PIN) == 0);
}

void hardware::setBuzzer(uint8_t val)
{
    IO::digitalWrite(BUZZER_PIN, (val&1));
}

void hardware::setLamp(uint8_t val)
{
    IO::digitalWrite(LAMP_PIN, (val&1));
}

uint8_t hardware::getLamp()
{
    return IO::digitalRead(LAMP_PIN);
}

void hardware::setCompressor(uint8_t val)
{
    IO::digitalWrite(COMPRESSOR_PIN, (val&1));
}

uint8_t hardware::getCompressor()
{
   return IO::digitalRead(COMPRESSOR_PIN);
}

void hardware::setDigit(uint8_t val)
{
    IO::digitalWrite(SEG_CA1_PIN, val & 0x01);
    IO::digitalWrite(SEG_CA2_PIN, val & 0x02);
    IO::digitalWrite(LED_EN_PIN, val & 0x04);
}

void hardware::printSegment(uint8_t val)
{
    setOutputSegment();
    val = ~val;
    IO::digitalWrite(SEG_A_PIN, val & 0x01);
    IO::digitalWrite(SEG_B_PIN, val & 0x02);
    IO::digitalWrite(SEG_C_PIN, val & 0x04);
    IO::digitalWrite(SEG_D_PIN, val & 0x08);
    IO::digitalWrite(SEG_E_PIN, val & 0x10);
    IO::digitalWrite(SEG_F_PIN, val & 0x20);
    IO::digitalWrite(SEG_G_PIN, val & 0x40);
}

void hardware::setInputSegment()
{
    IO::pinMode(SEG_A_PIN, INPUT);
    IO::pinMode(SEG_B_PIN, INPUT);
    IO::pinMode(SEG_F_PIN, INPUT);
    IO::pinMode(SEG_CA1_PIN, INPUT);
    IO::pinMode(SEG_CA2_PIN, INPUT);


    IO::digitalWrite(SEG_A_PIN, 0);
    IO::digitalWrite(SEG_B_PIN, 0);
    IO::digitalWrite(SEG_C_PIN, 1);
    IO::digitalWrite(SEG_D_PIN, 1);
    IO::digitalWrite(SEG_E_PIN, 1);
    IO::digitalWrite(SEG_F_PIN, 0);
    IO::digitalWrite(SEG_G_PIN, 1);
}

void hardware::setOutputSegment()
{
    IO::pinMode(SEG_A_PIN, OUTPUT);
    IO::pinMode(SEG_B_PIN, OUTPUT);
    IO::pinMode(SEG_F_PIN, OUTPUT);
    IO::pinMode(SEG_CA1_PIN, OUTPUT);
    IO::pinMode(SEG_CA2_PIN, OUTPUT);
}

uint8_t hardware::getKey()
{
    uint8_t key = 0;
    setInputSegment();
    key |= (IO::digitalRead(SEG_A_PIN) == 0) ? 0x01 : 0;
    key |= (IO::digitalRead(SEG_B_PIN) == 0) ? 0x02 : 0;
    key |= (IO::digitalRead(SEG_F_PIN) == 0) ? 0x04 : 0;

    return key;
}

void hardware::debug(uint8_t d)
{
    DDRB |= 0xf0;
    PORTB &= ((d & 0xf0) | 0x0f);
    PORTB |= (d & 0xf0);
}


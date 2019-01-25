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
//#include "Display.h"


#ifndef PINS_H_
#error pins not defined (include *pins.h header in your HardwareConfig.h)
#endif

uint8_t hardware::getKeyPressed()
{
//    return   (IO::digitalRead(BUTTON_STOP_PIN) ? 0 : BUTTON_STOP)
//            | (IO::digitalRead(BUTTON_DEC_PIN)  ? 0 : BUTTON_DEC)
//            | (IO::digitalRead(BUTTON_INC_PIN)  ? 0 : BUTTON_INC)
//            | (IO::digitalRead(BUTTON_START_PIN)? 0 : BUTTON_START);
}


void hardware::initializePins()
{
    setBuzzer(0);

    IO::analogReference(EXTERNAL);

    IO::pinMode(BUZZER_PIN, OUTPUT);
    IO::pinMode(COMPRESSOR_PIN, OUTPUT);
    IO::pinMode(LED_PIN, OUTPUT);
    IO::pinMode(LAMP_PIN, OUTPUT);
    IO::pinMode(FAN_PIN, OUTPUT);
    IO::pinMode(HEATER_PIN, OUTPUT);

    IO::pinMode(DOOR_PIN, INPUT);
}

void hardware::initialize()
{
//    AnalogInputsADC::initialize();
}

void hardware::setBuzzer(uint8_t val)
{
    IO::digitalWrite(BUZZER_PIN, (val&1));
}


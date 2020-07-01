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
//#include "Display.h"


#ifndef PINS_H_
#error pins not defined (include *pins.h header in your HardwareConfig.h)
#endif



void hardware::initializePins()
{
    IO::analogReference(EXTERNAL);

    IO::pinMode(BUZZER_PIN, OUTPUT);
    IO::pinMode(COMPRESSOR_PIN, OUTPUT);
    IO::pinMode(LED_PIN, OUTPUT);
    IO::pinMode(LAMP_PIN, OUTPUT);
    IO::pinMode(FAN_PIN, OUTPUT);
    IO::pinMode(HEATER_PIN, OUTPUT);

    IO::pinMode(DOOR_PIN, INPUT);
    IO::pinMode(DISP_SDA, INPUT);
    IO::pinMode(DISP_SCL, INPUT);
}

void hardware::initialize()
{
    setBuzzer(0);
    setCompressor(0);
    setFan(0);
    setHeater(0);
    setLamp(0);
    AnalogInputsADC::initialize();
}

uint8_t hardware::IsDoorOpen()
{
    return IO::digitalRead(DOOR_PIN);
}

void hardware::setBuzzer(uint8_t val)
{
    IO::digitalWrite(BUZZER_PIN, (val&1));
}

void hardware::setLamp(uint8_t val)
{
    IO::digitalWrite(LAMP_PIN, (val&1));
}

void hardware::setLED(uint8_t val)
{
    IO::digitalWrite(LED_PIN, (val&1));
}

void hardware::setFan(uint8_t val)
{
    IO::digitalWrite(FAN_PIN, (val&1));
}

uint8_t hardware::getFan()
{
    return IO::digitalRead(FAN_PIN);
}

void hardware::setHeater(uint8_t val)
{
    IO::digitalWrite(HEATER_PIN, (val&1));
}

uint8_t hardware::getHeater()
{
    return IO::digitalRead(HEATER_PIN);
}

void hardware::setCompressor(uint8_t val)
{
    IO::digitalWrite(COMPRESSOR_PIN, (val&1));
}

uint8_t hardware::getCompressor()
{
    return IO::digitalRead(COMPRESSOR_PIN);
}

void hardware::setSDA(uint8_t val)
{
    if(val)
    {
        IO::digitalWrite(DISP_SDA, LOW);
        IO::pinMode(DISP_SDA, INPUT);
    }
    else
    {
        IO::pinMode(DISP_SDA, OUTPUT);
        IO::digitalWrite(DISP_SDA, LOW);
    }
}

void hardware::setSCL(uint8_t val)
{
    if(val)
    {
        IO::digitalWrite(DISP_SCL, LOW);
        IO::pinMode(DISP_SCL, INPUT);
    }
    else
    {
        IO::pinMode(DISP_SCL, OUTPUT);
        IO::digitalWrite(DISP_SCL, LOW);
    }
}

uint8_t hardware::getSDA()
{
    return IO::digitalRead(DISP_SDA);
}

uint8_t hardware::getSCL()
{
    return IO::digitalRead(DISP_SDA);
}


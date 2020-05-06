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
#include "DisplayPanel.h"
//#include "SoftI2C.h"
#include "Buzzer.h"
#include "Hardware.h"
#include "memory.h"
#include "Settings.h"

#define DISPLAYPANEL_SEG_A                  0x01
#define DISPLAYPANEL_SEG_B                  0x02
#define DISPLAYPANEL_SEG_C                  0x04
#define DISPLAYPANEL_SEG_D                  0x08
#define DISPLAYPANEL_SEG_E                  0x10
#define DISPLAYPANEL_SEG_F                  0x20
#define DISPLAYPANEL_SEG_G                  0x40

#define DISPLAYPANEL_LEDS_DIGIT             2
#define DISPLAYPANEL_TEN_DIGIT              1
#define DISPLAYPANEL_ONE_DIGIT              0

#define DISPLAYPANEL_SUPER_DIGIT_IDX        DISPLAYPANEL_SEG_E
#define DISPLAYPANEL_ALARM_DIGIT_IDX        DISPLAYPANEL_SEG_C
#define DISPLAYPANEL_MINUS_DIGIT_IDX        DISPLAYPANEL_SEG_D

namespace DisplayPanel
{
    uint8_t _print_buf[3] = {0};
    uint8_t Brightness = 0x07;
    uint16_t BrightnessTimer;
    const uint8_t _pattern_digit[] PROGMEM = {
    DISPLAYPANEL_SEG_A | DISPLAYPANEL_SEG_B | DISPLAYPANEL_SEG_C | DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_F ,
    DISPLAYPANEL_SEG_B | DISPLAYPANEL_SEG_C ,
    DISPLAYPANEL_SEG_A | DISPLAYPANEL_SEG_B | DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_G ,
    DISPLAYPANEL_SEG_A | DISPLAYPANEL_SEG_B | DISPLAYPANEL_SEG_C | DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_G ,
    DISPLAYPANEL_SEG_B | DISPLAYPANEL_SEG_C | DISPLAYPANEL_SEG_F | DISPLAYPANEL_SEG_G ,
    DISPLAYPANEL_SEG_A | DISPLAYPANEL_SEG_C | DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_F | DISPLAYPANEL_SEG_G ,
    DISPLAYPANEL_SEG_A | DISPLAYPANEL_SEG_C | DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_F | DISPLAYPANEL_SEG_G ,
    DISPLAYPANEL_SEG_A | DISPLAYPANEL_SEG_B | DISPLAYPANEL_SEG_C ,
    DISPLAYPANEL_SEG_A | DISPLAYPANEL_SEG_B | DISPLAYPANEL_SEG_C | DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_F | DISPLAYPANEL_SEG_G ,
    DISPLAYPANEL_SEG_A | DISPLAYPANEL_SEG_B | DISPLAYPANEL_SEG_C | DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_F | DISPLAYPANEL_SEG_G ,
    DISPLAYPANEL_SEG_A | DISPLAYPANEL_SEG_B | DISPLAYPANEL_SEG_C | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_F | DISPLAYPANEL_SEG_G ,
    DISPLAYPANEL_SEG_C | DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_F | DISPLAYPANEL_SEG_G ,
    DISPLAYPANEL_SEG_A | DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_F,
    DISPLAYPANEL_SEG_B | DISPLAYPANEL_SEG_C | DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_G ,
    DISPLAYPANEL_SEG_A | DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_F | DISPLAYPANEL_SEG_G ,
    DISPLAYPANEL_SEG_A | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_F | DISPLAYPANEL_SEG_G ,
    };

    KeyType _old_key = None;
    uint8_t scan_key;
    uint8_t scan_idx = 0;
    uint16_t scan_time_U16_;
}

void DisplayPanel::setBrightness(uint8_t b)
{
    Brightness = 0x07 & b;
    //rightness = 0x07;
}

void DisplayPanel::printTemp(int16_t temp)
{
    uint8_t t;
    temp /= 10;
    if(temp < 0)
        t = -temp;
    else
        t = temp;

    _print_buf[DISPLAYPANEL_TEN_DIGIT] = pgm::read(_pattern_digit + (t / 10));// _pattern_digit[t / 10];
    _print_buf[DISPLAYPANEL_ONE_DIGIT] = pgm::read(_pattern_digit + (t % 10));// _pattern_digit[t % 10];

    if(temp < 0)
        _print_buf[DISPLAYPANEL_LEDS_DIGIT] |= DISPLAYPANEL_MINUS_DIGIT_IDX;
    else
        _print_buf[DISPLAYPANEL_LEDS_DIGIT] &= ~DISPLAYPANEL_MINUS_DIGIT_IDX;
}

void DisplayPanel::printHex(uint8_t hex)
{
    _print_buf[DISPLAYPANEL_TEN_DIGIT] = pgm::read(_pattern_digit + (hex >> 4));// _pattern_digit[hex >> 4];
    _print_buf[DISPLAYPANEL_ONE_DIGIT] = pgm::read(_pattern_digit + (hex & 0x0f));// _pattern_digit[hex & 0x0f];
    _print_buf[DISPLAYPANEL_LEDS_DIGIT] &= ~DISPLAYPANEL_MINUS_DIGIT_IDX;
}

void DisplayPanel::printBlank()
{
    _print_buf[DISPLAYPANEL_TEN_DIGIT] = 0x00;
    _print_buf[DISPLAYPANEL_ONE_DIGIT] = 0x00;
    _print_buf[DISPLAYPANEL_LEDS_DIGIT] &= ~DISPLAYPANEL_MINUS_DIGIT_IDX;
}

void DisplayPanel::printSu(void)
{
    printHex(0x50);
    _print_buf[DISPLAYPANEL_ONE_DIGIT] = DISPLAYPANEL_SEG_B |  DISPLAYPANEL_SEG_C |  DISPLAYPANEL_SEG_D |  DISPLAYPANEL_SEG_E |  DISPLAYPANEL_SEG_F ;
}

void DisplayPanel::printErrorEvaporator(void)
{
    printHex(0xEF);
}

void DisplayPanel::printErrorAmbiance(void)
{
    printHex(0xAF);

}

void DisplayPanel::printErrorLO()
{
    printHex(0x00);
    _print_buf[DISPLAYPANEL_TEN_DIGIT] = DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_F ;
}

void DisplayPanel::printSt()
{
    printHex(0x50);
    _print_buf[DISPLAYPANEL_ONE_DIGIT] = DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_F  | DISPLAYPANEL_SEG_G ;
}

void DisplayPanel::printOn(uint8_t on)
{
    _print_buf[DISPLAYPANEL_LEDS_DIGIT] &= ~DISPLAYPANEL_MINUS_DIGIT_IDX;
    if(on)
    {
        _print_buf[DISPLAYPANEL_TEN_DIGIT] = pgm::read(_pattern_digit + 0x00);
        _print_buf[DISPLAYPANEL_ONE_DIGIT] = DISPLAYPANEL_SEG_C | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_G ;
    }
    else
        _print_buf[DISPLAYPANEL_TEN_DIGIT] = _print_buf[DISPLAYPANEL_ONE_DIGIT] = DISPLAYPANEL_SEG_G;
}

void DisplayPanel::printTa()
{
    printHex(0x0A);
    _print_buf[DISPLAYPANEL_TEN_DIGIT] = DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_F  | DISPLAYPANEL_SEG_G ;
}

void DisplayPanel::printTe()
{
    printHex(0x0E);
    _print_buf[DISPLAYPANEL_TEN_DIGIT] = DISPLAYPANEL_SEG_D | DISPLAYPANEL_SEG_E | DISPLAYPANEL_SEG_F  | DISPLAYPANEL_SEG_G ;
}

void DisplayPanel::printCo()
{
    printHex(0xC0);
}

void DisplayPanel::setAlarm(uint8_t on)
{
    if(on)
        _print_buf[DISPLAYPANEL_LEDS_DIGIT] |= DISPLAYPANEL_ALARM_DIGIT_IDX;
    else
        _print_buf[DISPLAYPANEL_LEDS_DIGIT] &= ~DISPLAYPANEL_ALARM_DIGIT_IDX;
}

void DisplayPanel::setSuper(uint8_t on)
{
    if(on)
        _print_buf[DISPLAYPANEL_LEDS_DIGIT] |= DISPLAYPANEL_SUPER_DIGIT_IDX;
    else
        _print_buf[DISPLAYPANEL_LEDS_DIGIT] &= ~DISPLAYPANEL_SUPER_DIGIT_IDX;
}

uint8_t DisplayPanel::IsAlarmIconOn()
{
    return (_print_buf[DISPLAYPANEL_LEDS_DIGIT] & DISPLAYPANEL_ALARM_DIGIT_IDX);
}

uint8_t DisplayPanel::IsSuperIconOn()
{
    return (_print_buf[DISPLAYPANEL_LEDS_DIGIT] & DISPLAYPANEL_SUPER_DIGIT_IDX);
}

DisplayPanel::KeyType DisplayPanel::getKey(void)
{
    KeyType key = None;
    if(scan_key == 0x02)
        key = Power;
    else if(scan_key == 0x04)
        key = Temp;
    else if(scan_key == 0x01)
        key = Alarm;
    else if(scan_key == (0x01 | 0x04))
        key = Test;
    else if(scan_key == (0x01 | 0x02 | 0x04))
        key = State;

    if(key != _old_key)
    {
        _old_key = key;
        if(key != None)
        {
            //Buzzer::soundKeyboard();
        }
    }

    if(settings.power == 0)
    {
        if(key != Power)
            return None;
    }
    return key;
}

void DisplayPanel::doIdle()
{
    if(Time::diffU16(scan_time_U16_, Time::getInterruptsU16()) > 10)
    {
        scan_time_U16_ = Time::getInterruptsU16();
        if(settings.power)
        {
            hardware::setDigit(0x01 | 0x02 | 0x04);
            scan_key = hardware::getKey();

            scan_idx++;
            if(scan_idx > 2)
                scan_idx = 0;
        }
        else
            scan_idx = 3;
    }

    switch(scan_idx)
    {
        case 0 :
            hardware::printSegment(_print_buf[DISPLAYPANEL_ONE_DIGIT]);
            hardware::setDigit(0x02 | 0x04);
        break;
        case 1 :
            hardware::printSegment(_print_buf[DISPLAYPANEL_TEN_DIGIT]);
            hardware::setDigit(0x01 | 0x04);
        break;
        case 2 :
            hardware::printSegment(_print_buf[DISPLAYPANEL_LEDS_DIGIT]);
            hardware::setDigit(0x01 | 0x02);
        break;
        case 3 :
            hardware::setDigit(0x01 | 0x02 | 0x04);
            scan_key = hardware::getKey();
        break;
    }
}

void DisplayPanel::test()
{
    uint16_t _timeout_U16_;

    printBlank();
    setAlarm(0);
    setSuper(0);
    _print_buf[DISPLAYPANEL_TEN_DIGIT] = DISPLAYPANEL_SEG_A;
    do{
        Time::delayDoIdle(1000);
        _print_buf[DISPLAYPANEL_TEN_DIGIT] <<= 1;
    }while(_print_buf[DISPLAYPANEL_TEN_DIGIT] <= DISPLAYPANEL_SEG_G);
    _print_buf[DISPLAYPANEL_TEN_DIGIT] = 0x00;

    _print_buf[DISPLAYPANEL_ONE_DIGIT] = DISPLAYPANEL_SEG_A;
    do{
        Time::delayDoIdle(1000);
        _print_buf[DISPLAYPANEL_ONE_DIGIT] <<= 1;
    }while(_print_buf[DISPLAYPANEL_ONE_DIGIT] <= DISPLAYPANEL_SEG_G);
    _print_buf[DISPLAYPANEL_ONE_DIGIT] = 0x00;

    setAlarm(1);
    Time::delayDoIdle(1000);
    setAlarm(0);
    setSuper(1);
    Time::delayDoIdle(1000);
    setSuper(0);

    _timeout_U16_ = Time::getMilisecondsU16();
    do{
        Time::delayDoIdle(10);
        setAlarm(getKey() == Alarm);
        setSuper(getKey() == Power);
        if(getKey() == Temp)
            Buzzer::soundKeyboard();
    }while(Time::diffU16(_timeout_U16_, Time::getMilisecondsU16()) < 5000);

    setAlarm(0);
    setSuper(0);
}

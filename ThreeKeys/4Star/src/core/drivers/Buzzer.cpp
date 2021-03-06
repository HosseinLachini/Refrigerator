/*
    Pars-Refrigerator - open source firmware for a variety of Pars Refrigerators
    Copyright (C) 2018 Hossein Lachini. All right reserved.

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
#include "Buzzer.h"
#include "Settings.h"


namespace Buzzer {
    uint16_t begin_time_U16_;
    uint16_t last_value_;
    SoundType sound_;


    uint8_t getKeyboard(uint16_t time);
    uint8_t getInfo(uint16_t time);
    uint8_t getSave(uint16_t time);
    uint8_t getSelect(uint16_t time);
    uint8_t getStartProgram(uint16_t time);
    uint8_t getProgramComplete(uint16_t time);
    uint8_t getReversedPolarity(uint16_t time);
    uint8_t getError(uint16_t time);
    uint8_t getDoorError(uint16_t time);
    uint8_t getOff(uint16_t time);

    uint8_t getSound(uint16_t time);
    uint8_t setOff();


} // namespace Buzzer


void Buzzer::sound(Buzzer::SoundType s)
{
    if (settings.audioBeep == 0)
        s = Off;
    if(sound_ == s)
        return;
    sound_ = s;
    begin();
    Time::delayDoIdle(50);
}

void Buzzer::soundOff()
{
    sound_ = Off;
}


void Buzzer::soundIfSilence(Buzzer::SoundType s)
{
    if(sound_ == Off)
        sound(s);
}


void Buzzer::begin()
{
    begin_time_U16_ = Time::getInterruptsU16();
}


void Buzzer::doIdle()
{
    uint8_t v2 = getSound(Time::diffU16(begin_time_U16_, Time::getInterruptsU16()));
    if(v2 != last_value_) {
        last_value_ = v2;
        hardware::setBuzzer(last_value_);
    }
}

uint8_t Buzzer::getSound(uint16_t time)
{
    switch(sound_) {
    case Keyboard:             return getKeyboard(time);
    case Info:                 return getInfo(time);
    case Save:                 return getSave(time);
    case Select:               return getSelect(time);
    case ProgramComplete:      return getProgramComplete(time);
    case StartProgram:         return getStartProgram(time);
    case ReversedPolarity:     return getReversedPolarity(time);
    case Error:                return getError(time);
    case DoorError:            return getDoorError(time);
    case Off:
    default:
                    return getOff(time);
    }
}


uint8_t wave(int time, uint8_t size)
{
    if(time > 2*size)
        return 0;
    if(time > size)
        return 2*size - time;
    if(time > 0)
        return time;
    return 0;
}



uint8_t Buzzer::getOff(uint16_t time)
{
    return 0;
}
uint8_t Buzzer::setOff()
{
    soundOff();
    return 0;
}


uint8_t Buzzer::getKeyboard(uint16_t time)
{
    time /= 50;
    if(time > 2)
        return setOff();
    return wave(time, 1);
}


uint8_t Buzzer::getSelect(uint16_t time)
{
    int v = time/4;
    if(v > 200)
        return setOff();
    return wave(v, 10) + wave(v-40, 10);
}


uint8_t Buzzer::getInfo(uint16_t time)
{
    uint16_t v = time%2000;
    return wave(v, 10);
}

uint8_t Buzzer::getSave(uint16_t time)
{
    uint16_t v = time/10;

    if(v > 140)
        return setOff();

    return wave(v, 20);
}

uint8_t Buzzer::getStartProgram(uint16_t time)
{
    if(time > 4000)
        return setOff();
    return wave(time/500, 2);
}


uint8_t Buzzer::getProgramComplete(uint16_t time)
{
    uint16_t val = time/10;
    uint16_t v = val%40;
    uint16_t t = val/40;

    if(t > 10)
        return setOff();
    return wave(v, 20);
}

uint8_t Buzzer::getReversedPolarity(uint16_t time)
{
    uint16_t val = time/10;
    uint16_t v = val%40;

    if(val > 80)
        return setOff();
    return wave(v, 20);
}

uint8_t Buzzer::getError(uint16_t time)
{
    if((settings.audioBeep & BEEP_SENSEOR_ENABLE) == 0)
        return 0;
    if(time > 4000)
        return setOff();
    return wave(time/2000, 1);
}

uint8_t Buzzer::getDoorError(uint16_t time)
{
    if((settings.audioBeep & BEEP_DOOR_ENABLE) == 0)
        return 0;
    if(time > 2000)
        return setOff();
    return wave(time/1000, 1);
}

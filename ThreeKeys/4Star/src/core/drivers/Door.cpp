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
#include "Buzzer.h"
#include "Door.h"
#include "DisplayPanel.h"
#include "Settings.h"

namespace Door {
    uint16_t open_time_U16_;
    uint8_t counter_;
}

uint8_t Door::isOpen()
{
    return hardware::IsDoorOpen();
}


void Door::doIdle()
{
    if(isOpen())
    {
        if(open_time_U16_ == 0)
        {
            open_time_U16_ = Time::getSecondsU16();
            if(counter_ < 0xff)
                ++counter_;
            settings.audioBeep = 1;
        }
    }
    else
    {
        if(open_time_U16_)
        {
            open_time_U16_ = 0;
            DisplayPanel::setAlarm(0);
        }
    }
    if(TimeOut(60)
    //&& !TimeOut(300)
    && ((settings.state & REFRIGERATOR_STATE_ERROR) == 0))
    {
        if(((Time::getMilisecondsU16() >> 8) & 0x02))
        {
            Buzzer::soundDoorError();
            DisplayPanel::setAlarm(1);
        }
        else
            DisplayPanel::setAlarm(0);
    }
}

uint8_t Door::TimeOut(uint16_t timeout)
{
    if(open_time_U16_)
        return (Time::diffU16(open_time_U16_, Time::getSecondsU16()) > timeout);
    return 0;
}

void Door::ResetTimer()
{
    open_time_U16_ = 0;
}

void Door::ResetCounter()
{
    counter_ = 0;
}

uint8_t Door::ReadCounter()
{
    return counter_;
}

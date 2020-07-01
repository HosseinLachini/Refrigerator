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
#include "Lamp.h"
#include "Hardware.h"
#include "Buzzer.h"
#include "Door.h"

namespace Lamp
{
    uint16_t timer_U16_;
    uint8_t _door_last_state = 0;
    uint16_t readTimer()
    {
        return (timer_U16_ ? Time::diffU16(timer_U16_, Time::getSecondsU16()) : 0);
    }

    void setLamp(uint8_t val)
    {
        if(((hardware::getLamp() == 0) && val)
            || (hardware::getLamp() && (val == 0))
            )
        {
            hardware::setLamp(val);
        }

    }
}

void Lamp::doIdle()
{
    if(Door::isOpen() != _door_last_state)
    {
        if(Door::isOpen())
        {
            timer_U16_ = Time::getSecondsU16();
            setLamp(1);
        }
        else
        {
            setLamp(0);
        }
        _door_last_state = Door::isOpen();
    }
    else if(readTimer() > (5*60))
    {
        setLamp(0);
    }
}

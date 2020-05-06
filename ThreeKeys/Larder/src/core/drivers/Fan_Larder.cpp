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

/// To do : if after timeout 5' door will opened and closed
/// immediately, timeout 5' is correct?!
#include "Fan.h"
#include "Hardware.h"
#include "Door.h"
#include "Compressor.h"
#include "Sensors.h"
#include "Settings.h"

namespace Fan {
    uint16_t timer_U16_;
}

uint8_t Fan::isOn()
{
    return (hardware::getFan() !=0 );
}

void Fan::doIdle()
{
    if(Door::isOpen())
    {
        hardware::setFan(0);
        return;
    }
    if(Compressor::isON())
    {
        hardware::setFan(1);
    }
    else if(readTimer() > (2*60))
    {
        timer_U16_ = Time::getSecondsU16();
        hardware::setFan((isOn() == 0));
    }
}

uint16_t Fan::readTimer()
{
    return Time::diffU16(timer_U16_, Time::getSecondsU16());
}

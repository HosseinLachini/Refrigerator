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
#ifndef SENSORS_H_INCLUDED
#define SENSORS_H_INCLUDED

#include <stdint.h>

namespace Sensors
{
    enum SensorType { OK = 1100, FAIL = 1200 };
    void initialize();
    int16_t readEvaporator();
    int16_t readAmbiance();
    void doIdle();
};

#endif // SENSORS_H_INCLUDED

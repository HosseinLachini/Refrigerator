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

#define __STDC_LIMIT_MACROS
#include <stdint.h>

#include "AnalogInputsTypes.h"

AnalogInputs::ValueType AnalogInputs::evalI(AnalogInputs::ValueType P, AnalogInputs::ValueType U) {
    uint32_t i = P;
    i *= ANALOG_VOLT(1);
    i /= U;
    i *= ANALOG_AMP(1);
    i /= ANALOG_WATT(1);
    if (i > UINT16_MAX)
        i = UINT16_MAX;
    return i;
}
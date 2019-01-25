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
#ifndef SPEENO_H_
#define SPEENO_H_

#include <stdint.h>
#include "HardwareConfig.h"
//#include "Display.h"
#include "Time.h"
//#include "Buzzer.h"
//#include STRINGS_HEADER

namespace hardware {
    void initializePins();
    void initialize();
    uint8_t getKeyPressed();
    void delay(uint16_t t);
    void setBuzzer(uint8_t val);

    void doInterrupt();

    inline void setExternalTemperatueOutput(bool enable) {};
}


#endif /* SPEENO_H_ */

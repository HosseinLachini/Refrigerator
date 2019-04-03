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
#include "ParsRefrigerator.h"
#include "Buzzer.h"
#include "Hardware.h"
#include "cpu.h"
#include "Settings.h"

void setup()
{
    hardware::initializePins();
    cpu::init();

    hardware::initialize();
    Time::initialize();
    settings.audioBeep = 1;
    Buzzer::soundError();
    while(1)
    {
     //IO::digitalWrite(LED_PIN,HIGH);
     Time::delayDoIdle(100);
     //IO::digitalWrite(LED_PIN,LOW);
     //Buzzer::doIdle();
    }
}

int main(void)
{
    setup();
//#ifdef ENABLE_HELPER
//    helperMain();
//#else
//    eeprom::check();
//    MainMenu::run();
//#endif

    return 0;
}

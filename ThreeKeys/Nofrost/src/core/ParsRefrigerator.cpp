/**
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
**/
#include "ParsRefrigerator.h"
#include "Buzzer.h"
#include "Door.h"
#include "Hardware.h"
#include "cpu.h"
#include "Settings.h"
//#include "SoftI2C.h"
#include "DisplayPanel.h"
#include "AnalogInputsADC.h"
#include "Compressor.h"
#include "Sensors.h"
#include "MainMenu.h"
#include "memory.h"


void setup()
{
    hardware::initializePins();
    cpu::init();

    hardware::initialize();
    Time::initialize();
    // for prevent door counter auto increment and stable board
    Time::delay(1000);
    settings.power = true;
    settings.temperature_backup = settings.temperature = eeprom::read((uint16_t*)0);
    settings.audioBeep = (BEEP_DOOR_ENABLE | BEEP_SENSEOR_ENABLE);
    //Door::ResetTimer();

    settings.max_temperature = MAX_TEMPERATURE_SETTING;
    settings.min_temperature = -24;
    if((settings.temperature > ((int16_t)(settings.max_temperature) * 10)) || (settings.temperature < ((int16_t)(settings.min_temperature) * 10)))
    {
        settings.temperature_backup = settings.temperature = -200;
        eeprom::write((uint16_t*)(0), (uint16_t)(settings.temperature));
    }
    //DisplayPanel::test();
    DisplayPanel::printTemp(settings.temperature);
    settings.state = REFRIGERATOR_STATE_PLUGIN_DEFROST;
    Buzzer::soundStartProgram();
}


int main(void)
{
    setup();
#ifdef ENABLE_HELPER
   helperMain();
#else
//    eeprom::check();
    MainMenu::run();
#endif

    return 0;
}

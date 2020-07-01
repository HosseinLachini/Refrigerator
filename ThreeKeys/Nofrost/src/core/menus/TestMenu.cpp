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

#include "TestMenu.h"
#include "Time.h"
#include "Settings.h"
#include "DisplayPanel.h"
#include "Buzzer.h"
#include "Compressor.h"
#include "Lamp.h"
#include "Heater.h"
#include "Sensors.h"
#include "Hardware.h"

void TestMenu::run()
{
    uint8_t state_backup = settings.state;
    uint8_t state_audioBeep = settings.audioBeep;
    uint8_t compressor_backup = hardware::getCompressor();
    DisplayPanel::KeyType key;
    uint16_t timer_next_item_U16_ = 0;
    uint16_t timer_menu_U16 = Time::getSecondsU16();
    uint8_t index = 0;
    settings.audioBeep = 1;
    switch(settings.test_mode)
    {
        case TEST_MODE :

            hardware::setCompressor(0);
            hardware::setLamp(0);
            DisplayPanel::test();
            hardware::setCompressor(1);
            Time::delayDoIdle(5000);
            hardware::setCompressor(0);
            hardware::setHeater(1);
            Time::delayDoIdle(5000);
            hardware::setHeater(0);
            hardware::setLamp(1);
            Time::delayDoIdle(5000);
            hardware::setLamp(0);

            settings.state = state_backup;
            hardware::setCompressor(compressor_backup);
            settings.audioBeep = state_audioBeep;

            settings.test_mode = TEST_NONE;
        break;
        case TEST_STATE_MODE :
            //timer_next_item_U16_ = 0;
            //timer_menu_U16 = Time::getSecondsU16();
            #define WAIT_RELASE_KEYS    0x80
            #define WAIT_PRESS_KEYS     0x40
            do
            {
                switch(index)
                {
                    case 0:
                        DisplayPanel::printSt();
                        index |= WAIT_RELASE_KEYS;
                        break;
                    case 1:
                        DisplayPanel::printTa();
                        break;
                    case 2:
                        DisplayPanel::printTe();
                        break;
                    case 3:
                        DisplayPanel::printCo();
                        break;
                    case 4:
                        DisplayPanel::printFa();
                        break;
                    case 5:
                        DisplayPanel::printHr();
                        break;
                    default:
                        if(timer_next_item_U16_ == 0)
                        {
                            if(index & WAIT_PRESS_KEYS)
                            {
                                switch(index & ~WAIT_PRESS_KEYS)
                                {
                                    case 0:
                                        break;
                                    case 1:
                                        if(Sensors::readAmbiance() == Sensors::FAIL)
                                            DisplayPanel::printOn(0);
                                        else
                                            DisplayPanel::printTemp(Sensors::readAmbiance());
                                        break;
                                    case 2:
                                        if(Sensors::readEvaporator() == Sensors::FAIL)
                                            DisplayPanel::printOn(0);
                                        else
                                            DisplayPanel::printTemp(Sensors::readEvaporator());
                                        break;
                                    case 3:
                                        DisplayPanel::printOn(hardware::getCompressor());
                                        break;
                                    case 4:
                                        DisplayPanel::printOn(hardware::getCompressor());
                                        //DisplayPanel::printHex(settings.state);
                                        break;
                                    case 5:
                                        DisplayPanel::printOn(hardware::getHeater());
                                        break;
                                }
                                if(key == DisplayPanel::Temp)
                                {
                                    index &= ~WAIT_PRESS_KEYS;
                                    index++;
                                    if(index > 5)
                                        index = 1;
                                    timer_next_item_U16_ = Time::getMilisecondsU16();
                                }
                            }
                            if((index & WAIT_RELASE_KEYS) && (key == DisplayPanel::None))
                            {
                                index &= ~WAIT_RELASE_KEYS;
                                index |= WAIT_PRESS_KEYS;
                            }
                        }
                }
                if((Time::diffU16(timer_next_item_U16_, Time::getMilisecondsU16()) > (1*1000)) && timer_next_item_U16_)
                {
                    timer_next_item_U16_ = 0;
                    index |= WAIT_PRESS_KEYS;
                }
                Time::delayDoIdle(100);
                key = DisplayPanel::getKey();
                if(key != DisplayPanel::None)
                    timer_menu_U16 = Time::getSecondsU16();
            }while((Time::diffU16(timer_menu_U16, Time::getSecondsU16()) < (2*60)) && ((index &  WAIT_RELASE_KEYS)  || (key != DisplayPanel::State)));

            settings.test_mode = TEST_NONE;
        break;
    }
}

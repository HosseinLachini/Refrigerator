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
#include "Menu.h"
#include "Settings.h"
#include "DisplayPanel.h"
#include "Time.h"
#include "Sensors.h"
#include "Door.h"
#include "Buzzer.h"
#include "Compressor.h"
#include "memory.h"
#include "Heater.h"

namespace Menu {

    uint8_t index_;
    uint8_t error_index = 0; /// index of error to show.
    bool hold_key_ = false;
    bool waitRelease_ = false;
    bool blink_over_9 = true;
    bool manual_defrost = false;
    uint16_t timer_set_temp_mode = 0;

        DisplayPanel::KeyType run_()
        {
            DisplayPanel::KeyType button = DisplayPanel::getKey();
            DisplayPanel::KeyType button1 = button;
            uint16_t key_timer = Time::getSecondsU16();

            if(hold_key_ && (button1 != DisplayPanel::None))
                return button;
            while(((button1 == button) && (button1 != DisplayPanel::None)) && (Time::diffU16(key_timer, Time::getSecondsU16()) < 3))
            {
                button1 = DisplayPanel::getKey();
                Time::delayDoIdle(200);
            }
            if(button1 == DisplayPanel::None)
                waitRelease_ = false;
            hold_key_ = ((Time::diffU16(key_timer, Time::getSecondsU16()) >= 3));
            return button;
        }

        void Display()
        {
            if(timer_set_temp_mode && (Time::diffU16(timer_set_temp_mode, Time::getMilisecondsU16())) >= (3*1000))
            {
                eeprom::write((uint16_t*)(0), (uint16_t)(settings.temperature));
                timer_set_temp_mode = 0;
            }
            if(((settings.state & REFRIGERATOR_STATE_ERROR_AMBI) == 0) && (Sensors::readAmbiance() < -120))
                blink_over_9 = true;
            DisplayPanel::setSuper(settings.state & REFRIGERATOR_STATE_SUPER);
            //DisplayPanel::setAlarm(settings.state & REFRIGERATOR_STATE_ERROR);

            if(Door::TimeOut(60))
            {
                DisplayPanel::setAlarm((Time::getMilisecondsU16() >> 8) & 0x02);
            }
            else
            {
                DisplayPanel::setAlarm((settings.state & REFRIGERATOR_STATE_ERROR) && ((Time::getMilisecondsU16() >> 8) & 0x04));
            }

            if(timer_set_temp_mode)
            {
                if((((Time::getMilisecondsU16()) & 0x0200) == 0))
                    DisplayPanel::printTemp(settings.temperature_backup);
                else
                    DisplayPanel::printBlank();
                if(settings.state & REFRIGERATOR_STATE_ERROR)
                    Buzzer::soundError();
            }
            else
            {
            if(settings.state & REFRIGERATOR_STATE_ERROR)
            {
                Buzzer::soundError();
                if((Time::getMilisecondsU16() >> 8) & 0x08)
                {
                    error_index |= 0x80;
                    if(error_index == 0x80){
                        if(settings.state & REFRIGERATOR_STATE_ERROR_AMBI)
                            DisplayPanel::printErrorAmbiance();
                        else
                            error_index = 0x81;
                    }
                    if(error_index == 0x81){
                        if((settings.state & REFRIGERATOR_STATE_ERROR_EVAP))
                            DisplayPanel::printErrorEvaporator();
                        else
                            error_index = 0x82;
                    }
                    if(error_index == 0x82){
                        if((settings.state & REFRIGERATOR_STATE_ERROR_LO))
                            DisplayPanel::printErrorLO();
                        else
                            error_index = 0x83;
                    }
                    if(error_index == 0x83){
                        if((settings.state & REFRIGERATOR_STATE_ERROR_HEAT))
                            DisplayPanel::printErrorHE();
                        else
                            error_index = 0x80;
                    }
                }
            }
            /// corect this if... HE error not blinked.
            if(settings.state & REFRIGERATOR_STATE_DEFROST)
            {
                if(manual_defrost)
                {
                    if(Heater::isOn())
                        DisplayPanel::printHr();
                    else
                    {
                        manual_defrost = false;
                        DisplayPanel::printTemp(settings.temperature_backup);
                    }
                }
                else
                    DisplayPanel::printTemp(settings.temperature_backup);
            }
            else if(((settings.state & REFRIGERATOR_STATE_ERROR) == 0) || (((Time::getMilisecondsU16() >> 8) & 0x08) == 0))
            {
                if(error_index & 0x80){
                    error_index &= ~0x80;
                    error_index++;
                    if(error_index > 3)
                        error_index = 0;
                }
                switch((settings.state & REFRIGERATOR_STATE_SUPER))
                {
                    case REFRIGERATOR_STATE_SUPER :
                        if(((settings.state & REFRIGERATOR_STATE_ERROR) == 0) && (/*timer_set_temp_mode || */((Sensors::readAmbiance() > -95)  && blink_over_9)) && (((Time::getMilisecondsU16()) & 0x0200) == 0))
                            DisplayPanel::printBlank();
                        else if((((settings.state & REFRIGERATOR_STATE_ERROR) == 0) && (Sensors::readAmbiance() > -95)))
                            DisplayPanel::printTemp(Sensors::readAmbiance());
                        else
                            DisplayPanel::printSu();
                    break;
                    case REFRIGERATOR_STATE_NORMAL :
                        if(((settings.state & REFRIGERATOR_STATE_ERROR) == 0) && (timer_set_temp_mode || ((Sensors::readAmbiance() > -95)  && blink_over_9)) && (((Time::getMilisecondsU16()) & 0x0200) == 0))
                            DisplayPanel::printBlank();
                        else
                            DisplayPanel::printTemp((((settings.state & REFRIGERATOR_STATE_ERROR) == 0) && (Sensors::readAmbiance() > -95)) ? Sensors::readAmbiance() : settings.temperature_backup);
                    break;
                }
            }
            }
        }
    }

int8_t Menu::run(bool alwaysRefresh)
{
    DisplayPanel::KeyType key;
    key = run_();

    Display();

    if(hold_key_)
    {
        switch(key){
            case DisplayPanel::Temp :
                if(!waitRelease_)
                {
                    timer_set_temp_mode = timer_set_temp_mode ? 0 : Time::getMilisecondsU16();
                    waitRelease_ = true;
                }
            break;
            case DisplayPanel::Alarm :
                if(!waitRelease_)
                {
                    settings.state ^= REFRIGERATOR_STATE_SUPER;
//                    if((settings.state & ~(REFRIGERATOR_STATE_ERROR)) == REFRIGERATOR_STATE_SUPER)
//                    {
//                        settings.temperature_backup = settings.temperature;
//                        settings.temperature = -31*10;
//                    }
//                    else
//                    {
//                        settings.state = REFRIGERATOR_STATE_NORMAL;/// | REFRIGERATOR_STATE_DEFROST;
//                        settings.temperature = settings.temperature_backup;
//                    }
                    waitRelease_ = true;
                }
            break;
            case DisplayPanel::Power :
                if(!waitRelease_)
                {
                    if(settings.power)
                    {
                        settings.power = false;
                        hardware::setBuzzer(0);
                        hardware::setCompressor(0);
                        hardware::setLamp(0);
                    }
                    else
                    {
                        settings.power = true;
                        ((void(*)(void))0)();
                    }
                    waitRelease_ = true;
                }
            break;
            case DisplayPanel::Defrost :
                if(!waitRelease_)
                {
                    if((settings.state & REFRIGERATOR_STATE_ERROR_EVAP) == 0)
                    {
                        manual_defrost = true;
                        settings.state ^= REFRIGERATOR_STATE_DEFROST;
                    }
                    waitRelease_ = true;
                }
            break;
            case DisplayPanel::Test :
                if(!waitRelease_)
                {
                    settings.test_mode = TEST_MODE;
                    waitRelease_ = true;
                    //return MENU_EXIT;
                }
            break;;
            case DisplayPanel::State :
                if(!waitRelease_)
                {
                    settings.test_mode = TEST_STATE_MODE;
                    waitRelease_ = true;
                }
            break;
        }
    }
    else if((settings.state & ~(REFRIGERATOR_STATE_ERROR)) == REFRIGERATOR_STATE_NORMAL)
    {
        switch(key){
            case DisplayPanel::Temp :
                if(timer_set_temp_mode)
                {
                    timer_set_temp_mode = Time::getMilisecondsU16();
                    settings.temperature_backup-=10;
                    if(settings.temperature_backup < ((int16_t)(settings.min_temperature) * 10))
                        settings.temperature_backup = ((int16_t)(settings.max_temperature) * 10);
                    settings.temperature = settings.temperature_backup;
                }
            break;
            case DisplayPanel::Alarm :
                if(Door::isOpen() || (settings.state & REFRIGERATOR_STATE_ERROR))
                    settings.audioBeep = 0;
                else if(blink_over_9)
                    blink_over_9 = false;
            break;
        }
    }
    else if((settings.state & ~(REFRIGERATOR_STATE_ERROR)) == REFRIGERATOR_STATE_SUPER)
    {
        switch(key){
            case DisplayPanel::Temp :
                if(timer_set_temp_mode && (manual_defrost == false))
                {
                    timer_set_temp_mode = Time::getMilisecondsU16();
                    settings.temperature_backup-=10;
                    if(settings.temperature_backup < ((int16_t)(settings.min_temperature) * 10))
                        settings.temperature_backup = ((int16_t)(settings.max_temperature) * 10);
                    //settings.temperature = settings.temperature_backup;
                }
            break;
            case DisplayPanel::Alarm :
                if(Door::isOpen() || (settings.state & REFRIGERATOR_STATE_ERROR))
                    settings.audioBeep = 0;
                else if(blink_over_9)
                    blink_over_9 = false;
            break;
        }
    }
    else if((settings.state & ~(REFRIGERATOR_STATE_ERROR)) == REFRIGERATOR_STATE_DEFROST)
    {
        switch(key){
            case DisplayPanel::Temp :
                if(timer_set_temp_mode && (manual_defrost == false))
                {
                    timer_set_temp_mode = Time::getMilisecondsU16();
                    settings.temperature_backup-=10;
                    if(settings.temperature_backup < ((int16_t)(settings.min_temperature) * 10))
                        settings.temperature_backup = ((int16_t)(settings.max_temperature) * 10);
                    settings.temperature = settings.temperature_backup;
                }
            break;
            case DisplayPanel::Alarm :
                if(Door::isOpen() || (settings.state & REFRIGERATOR_STATE_ERROR))
                    settings.audioBeep = 0;
                else if(blink_over_9)
                    blink_over_9 = false;
            break;
        }
    }
    return MENU_EXIT;
}

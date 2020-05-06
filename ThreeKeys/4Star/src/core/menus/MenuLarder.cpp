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

namespace Menu {

    uint8_t index_;
    uint8_t error_index = 0; /// index of error to show.
    bool hold_key_ = false;
    bool waitRelease_ = false;
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
            DisplayPanel::setSuper(settings.state & REFRIGERATOR_STATE_SUPER);
            DisplayPanel::setAlarm(settings.state & REFRIGERATOR_STATE_ERROR);
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
                            error_index = 0x80;
                    }
                }
            }
            if(((settings.state & REFRIGERATOR_STATE_ERROR) == 0) || (((Time::getMilisecondsU16() >> 8) & 0x08) == 0))
            {
                if(error_index & 0x80){
                    error_index &= ~0x80;
                    error_index++;
                    if(error_index > 2)
                        error_index = 0;
                }
                switch((settings.state & REFRIGERATOR_STATE_SUPER))
                {
                    case REFRIGERATOR_STATE_SUPER :
                        DisplayPanel::printSu();
                    break;
                    case REFRIGERATOR_STATE_NORMAL :
                        if(((settings.state & REFRIGERATOR_STATE_ERROR) == 0) && timer_set_temp_mode && (((Time::getMilisecondsU16()) & 0x0200) == 0))
                            DisplayPanel::printBlank();
                        else
                            DisplayPanel::printTemp(settings.temperature_backup);
                            //DisplayPanel::printTemp(Sensors::readAmbiance());
                    break;
                }
            }
        }
    }

int8_t Menu::run(bool alwaysRefresh)
{
    DisplayPanel::KeyType key;
    key = run_();

    Display();

//    if((DisplayPanel::IsChildLockIconOn()) && (key != DisplayPanel::ChildLock))
//    {
//        if(hold_key_)
//        {
//            if(DisplayPanel::IsChildLockIconOn())
//            {
//                switch(key){
//                    case DisplayPanel::Fridge :
//                    break;
//                    case DisplayPanel::SuperCool :
//                    break;
//                    case DisplayPanel::Holiday :
//                        settings.test_mode = TEST_DISPLAY;
//                    break;
//                    case DisplayPanel::Heater :
//                        settings.test_mode = TEST_FACTORY;
//                    break;
//                    case DisplayPanel::Alarm :
//                        settings.test_mode = TEST_MAIN_BOARD;
//                    break;
//                    case DisplayPanel::ChildLock :
//                    break;
//                }
//            }
//        }
//        return MENU_EXIT;
//    }
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
                    if((settings.state & ~(REFRIGERATOR_STATE_ERROR)) == REFRIGERATOR_STATE_SUPER)
                    {
                        settings.temperature_backup = settings.temperature;
                        settings.temperature = 1;
                    }
                    else
                    {
                        settings.state = REFRIGERATOR_STATE_NORMAL;/// | REFRIGERATOR_STATE_DEFROST;
                        settings.temperature = settings.temperature_backup;
                    }
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
                    settings.temperature-=10;
                    if(settings.temperature < (settings.min_temperature))
                        settings.temperature = (settings.max_temperature);
                    settings.temperature_backup = settings.temperature;
                }
            break;
            case DisplayPanel::Alarm :
                if(Door::isOpen() || (settings.state & REFRIGERATOR_STATE_ERROR))
                    settings.audioBeep = 0;
            break;
        }
    }
    else if((settings.state & ~(REFRIGERATOR_STATE_ERROR)) == REFRIGERATOR_STATE_SUPER)
    {
        switch(key){
            case DisplayPanel::Temp :
            break;
            case DisplayPanel::Alarm :
                if(Door::isOpen() || (settings.state & REFRIGERATOR_STATE_ERROR))
                    settings.audioBeep = 0;
            break;
        }
    }
    return MENU_EXIT;
}

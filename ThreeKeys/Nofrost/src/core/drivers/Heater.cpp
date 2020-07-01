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
#include "Heater.h"
#include "Hardware.h"
#include "Settings.h"
#include "Sensors.h"
#include "Compressor.h"

#define TIME_OUT_HEATER     (30*60) ///< 30 minuts
#define TIME_OUT_DROPPER    (10*60) ///< 10 minuts
#define EVA_CHECK_TIME      (1*60)  ///< 1 minuts
#define EVA_PLUGIN_CHECK_TIME      (5)  ///< 5 sec

namespace Heater
{
    uint16_t timer_U16_ = 0; /**< Heater timer */
    uint16_t timer_on_U16_ = 0; /**< Heater ON timer */
    uint16_t timer_dropper_U16_ = 0; /**< Dropper timer */
}

/**
 * it's run only at defrost mode
 *
 * set ON/OFF heater and clear defrost flag at end time of defrost.
 *
 * @param None.
 */
void Heater::doIdle()
{
    if(settings.state & (REFRIGERATOR_STATE_PLUGIN_DEFROST | REFRIGERATOR_STATE_DEFROST))
    {
        // defrost process is here.
        if(settings.state & REFRIGERATOR_STATE_ERROR_EVAP)
        {
                setHeater(0);
                settings.state &= ~(REFRIGERATOR_STATE_PLUGIN_DEFROST | REFRIGERATOR_STATE_DEFROST);
                timer_dropper_U16_ = 0;
                timer_on_U16_ = 0;
                timer_U16_ = 0;
                Compressor::resetWorkTimer();
        }
        else if(isOn())
        {
            if(settings.state & REFRIGERATOR_STATE_PLUGIN_DEFROST) /// plugin test
            {
                if((timer_on_U16_ + readTimer()) > EVA_PLUGIN_CHECK_TIME)
                {
                    settings.state &= ~REFRIGERATOR_STATE_PLUGIN_DEFROST;
                    if(Sensors::readEvaporator() < 100)
                        settings.state |= REFRIGERATOR_STATE_DEFROST;
                    else
                        {
                            setHeater(0);
                            timer_dropper_U16_ = 0;
                            timer_on_U16_ = 0;
                            timer_U16_ = 0;
                            Compressor::resetWorkTimer();
                        }
                }
            }
            else /// defrost
            {
                if((((timer_on_U16_ + readTimer()) > EVA_CHECK_TIME) && (Sensors::readEvaporator() >= 100))
                 || ((timer_on_U16_ + readTimer()) > TIME_OUT_HEATER))
                {
                    if(Sensors::readEvaporator() >= 100)
                    {
                        settings.state &= ~(REFRIGERATOR_STATE_ERROR_HEAT);
                    }
                    else
                    {
                        if(((settings.state & REFRIGERATOR_STATE_ERROR_HEAT) == 0))
                        {
                            settings.state |= (REFRIGERATOR_STATE_ERROR_HEAT);
                            settings.audioBeep = 1;
                        }
                    }
                    setHeater(0);
                }
            }
        }
        else // Heater is OFF
        {
            if(timer_on_U16_ == 0) // start of Defrost
            {
                if(Sensors::readEvaporator() >= 100) // no need to ON Heater, Evaporator is warm.
                {
                    if(settings.state & REFRIGERATOR_STATE_PLUGIN_DEFROST)
                    {
                        settings.state &= ~REFRIGERATOR_STATE_PLUGIN_DEFROST;
                        //setHeater(0);
                    }
                    else
                    {
                        timer_on_U16_ = 1;
                        timer_U16_ = Time::getSecondsU16();
                    }
                }
                else
                {
                    //if((settings.state & REFRIGERATOR_STATE_PLUGIN_DEFROST) && (timer_U16_ = 0))
                    //    timer_U16_ = Time::getSecondsU16();
                    setHeater(1); // need to ON Heater, Evaporator is coold.
                }
            }
            else if((timer_dropper_U16_ + readTimer()) > TIME_OUT_DROPPER) // end of dropper time.
            {
                settings.state &= ~REFRIGERATOR_STATE_DEFROST;
                timer_dropper_U16_ = 0;
                timer_on_U16_ = 0;
                timer_U16_ = 0;
                Compressor::resetWorkTimer();
            }
//            else if((timer_on_U16_ < TIME_OUT_HEATER) && (readTimer() > EVA_CHECK_TIME))
//            {
//                if(Sensors::readEvaporator() <= 0)
//                {
//                    setHeater(1);
//                }
//            }
        }
    }
}

/**
 * check heater is ON or OFF.
 *
 * if heater is OFF return 0, else return a number.
 *
 * @param None.
 * @return Zero for off, none Zero for ON state.
 */

uint8_t Heater::isOn()
{
    return hardware::getHeater();
}

void Heater::setHeater(uint8_t val)
{
    if((Heater::isOn() && (val == 0)) || ((Heater::isOn() == 0) && val))
    {
        hardware::setHeater(val);
        if(val)
        {
            timer_dropper_U16_ += readTimer();
            if(timer_on_U16_ == 0)   timer_on_U16_ = 1;
        }
        else
        {
            timer_on_U16_ += readTimer();
            if(timer_dropper_U16_ == 0) timer_dropper_U16_ = 1;
        }
        timer_U16_ = Time::getSecondsU16();
    }
}

uint16_t Heater::readTimer()
{
    return (timer_U16_ ? Time::diffU16(timer_U16_, Time::getSecondsU16()) : 0);
}

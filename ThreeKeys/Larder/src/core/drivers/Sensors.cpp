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
#include "Sensors.h"
#include "AnalogInputsADC.h"
#include "Hardware.h"
#include "Settings.h"

#define SENSORS_SAMPLES_INTERVAL    100
#define SENSORS_SAMPLES_NUMBERS     20

namespace Sensors
{
    int16_t _Evaporator, _Ambiance;
    int16_t sum_eva = 0, sum_amb = 0;
    uint16_t timer_U16_ = 0;
    uint8_t idx = 0;
};

void Sensors::initialize()
{
    AnalogInputsADC::initialize();
}


void Sensors::doIdle()
{
    if(Time::diffU16(timer_U16_, Time::getMilisecondsU16()) >= SENSORS_SAMPLES_INTERVAL)
    {
        timer_U16_ = Time::getMilisecondsU16();
        idx++;
        sum_amb += AnalogInputsADC::readTemp(AMBIANCE_TEMP_PIN);
        sum_eva += AnalogInputsADC::readTemp(DEFROST_TEMP_PIN);
        if(idx >= SENSORS_SAMPLES_NUMBERS)
        {
            _Ambiance   = sum_amb / SENSORS_SAMPLES_NUMBERS;
            _Evaporator = sum_eva / SENSORS_SAMPLES_NUMBERS;
            sum_amb = sum_eva = 0;
            idx = 0;

            if(_Ambiance == AnalogInputsADC::FAIL)
                settings.state |= (REFRIGERATOR_STATE_ERROR_AMBI);
            else
                settings.state &= ~(REFRIGERATOR_STATE_ERROR_AMBI);

            if(_Evaporator == AnalogInputsADC::FAIL)
                settings.state |= (REFRIGERATOR_STATE_ERROR_EVAP);
            else
                settings.state &= ~(REFRIGERATOR_STATE_ERROR_EVAP);
        }
    }
/*
    int16_t sum = 0;

    _Ambiance = AnalogInputsADC::readTemp(AMBIANCE_TEMP_PIN);
    for(uint8_t i = 0; i < SENSORS_SAMPLES_NUMBER && _Ambiance != AnalogInputsADC::FAIL; i++)
    {
        sum += _Ambiance;
        _Ambiance = AnalogInputsADC::readTemp(AMBIANCE_TEMP_PIN);
    }
    if (_Ambiance != AnalogInputsADC::FAIL)
    {
        _Ambiance = sum / SENSORS_SAMPLES_NUMBER ;
        settings.state &= ~(REFRIGERATOR_STATE_ERROR_AMBI);
    }
    else if((settings.state & REFRIGERATOR_STATE_ERROR_AMBI) == 0)
    {
        settings.state |= (REFRIGERATOR_STATE_ERROR_AMBI);
        settings.audioBeep = 1;
    }
    _Evaporator = AnalogInputsADC::readTemp(DEFROST_TEMP_PIN);
    sum = 0;
    for(uint8_t i = 0; i < SENSORS_SAMPLES_NUMBER && _Evaporator != AnalogInputsADC::FAIL; i++)
    {
        sum += _Evaporator;
        _Evaporator = AnalogInputsADC::readTemp(DEFROST_TEMP_PIN);
    }
    if (_Evaporator != AnalogInputsADC::FAIL)
    {
        _Evaporator = sum / SENSORS_SAMPLES_NUMBER;
        settings.state &= ~(REFRIGERATOR_STATE_ERROR_EVAP);
    }
    else if((settings.state & REFRIGERATOR_STATE_ERROR_EVAP) == 0)
    {
        settings.state |= (REFRIGERATOR_STATE_ERROR_EVAP);
        settings.audioBeep = 1;
    }
*/
}


int16_t Sensors::readEvaporator()
{
    return _Evaporator;
}


int16_t Sensors::readAmbiance()
{
    return _Ambiance;
}

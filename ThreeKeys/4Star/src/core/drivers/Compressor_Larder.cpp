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
#include "Compressor.h"
#include "Hardware.h"
#include "Settings.h"
#include "Time.h"
#include "Sensors.h"

#define COMPRESSOR_ON_6_HOUR                (6*60*60)   // 6 hour
#define COMPRESSOR_WORK_25_HOUR             (25*60)     // 25 hour
#define COMPRESSOR_ON_OFF_TIMEOUT           (10*60)     // 10 min
#define COMPRESSOR_TEMP_CHECK_TIMEOUT       15          // 15 sec
#if COMPRESSOR_ON_OFF_TIMEOUT<=2*COMPRESSOR_TEMP_CHECK_TIMEOUT
    #error "comperssor time is not correct"
#endif

#define COMPERSSOR_IS_NEW_COMMAND()         (((state & COMPRESSOR_STATE_CUT_IN) && (state & COMPRESSOR_STATE_ON) == 0) || (((state & COMPRESSOR_STATE_CUT_IN) == 0) && (state & COMPRESSOR_STATE_ON)))

#define COMPRESSOR_STATE_START              (1 << 0)
#define COMPRESSOR_STATE_ERROR_AMBIENCE     (1 << 1)
#define COMPRESSOR_STATE_ERROR_EVAP         (1 << 2)
#define COMPRESSOR_STATE_AMBIENCE_15SEC2    (1 << 3)
#define COMPRESSOR_STATE_EVAPORATOR         (1 << 4)
#define COMPRESSOR_STATE_TIMOUT_10MIN       (1 << 5)
#define COMPRESSOR_STATE_CUT_IN             (1 << 6)
#define COMPRESSOR_STATE_ON                 (1 << 7)

#define COMPRESSOR_STATE_START_ON_CUT_MASK  (COMPRESSOR_STATE_START | COMPRESSOR_STATE_ON | COMPRESSOR_STATE_CUT_IN)

#ifdef REFRIGERATOR_STATE_ECO
    #define COMPRESSOR_CUT_IN_CHECK(t)              ((settings.state == REFRIGERATOR_STATE_ECO) ? (t >= 9) : (t >= (settings.temperature + 3)))
    #define COMPRESSOR_CUT_OUT_CHECK(t)             ((settings.state == REFRIGERATOR_STATE_ECO) ? (t <= 7) : (t <= (settings.temperature)))
#else // REFRIGERATOR_STATE_ECO
    #define COMPRESSOR_CUT_IN_CHECK(t)              (t >= (settings.temperature + 3))
    #define COMPRESSOR_CUT_OUT_CHECK(t)             (t <= (settings.temperature))
#endif

namespace Compressor {
    uint16_t timer_U16_ = 0;
    uint16_t work_timer_U16_ = 0;
    uint16_t timer15sec_U16_;
    uint8_t state = COMPRESSOR_STATE_START;
    bool cut_in_check() // on compressor
    {
        if(settings.temperature > 50)
        {
            return (Sensors::readEvaporator() > (settings.temperature));
        }
        return (Sensors::readEvaporator() > (50-10));/// 10 degree for Mr Sharifi added!
    }

    bool cut_out_check() // off compressor
    {
        if(settings.state & REFRIGERATOR_STATE_SUPER)
            return ((Sensors::readAmbiance() <= 10) || (readTimer() >= COMPRESSOR_ON_6_HOUR));
        return (Sensors::readAmbiance() <= (settings.temperature - 10));
    }

    void setCompressor(uint8_t val)
    {
        if((((hardware::getCompressor() == 0) && val)
            || (hardware::getCompressor() && (val == 0)))
            && ((readTimer() >= (3*60)) || (state == COMPRESSOR_STATE_START))
            )
        {
            timer_U16_ = Time::getSecondsU16();
            hardware::setCompressor(val);
            state &= ~COMPRESSOR_STATE_START;
        }

    }

    void LO_error_check()
    {
        if(((settings.state & REFRIGERATOR_STATE_ERROR) != REFRIGERATOR_STATE_ERROR_AMBI)
        && (hardware::getCompressor())
        )
        {
            if(readTimer() >= (10*60*60))
            {
                if(Sensors::readAmbiance() >= 90)
                {
                    if((settings.state & REFRIGERATOR_STATE_ERROR_LO) == 0)
                    {
                        settings.state |= (REFRIGERATOR_STATE_ERROR_LO);
                        settings.audioBeep |= BEEP_SENSEOR_ENABLE;
                    }
                }
            }
        }
    }
}

void Compressor::doIdle()
{
    LO_error_check();
    if((settings.state & (REFRIGERATOR_STATE_ERROR_AMBI | REFRIGERATOR_STATE_ERROR_EVAP)) == (REFRIGERATOR_STATE_ERROR_AMBI | REFRIGERATOR_STATE_ERROR_EVAP))
    {
        if(hardware::getCompressor()
            && (readTimer() > (20*60))
        )
        {
            setCompressor(0);
            return;
        }
        if((hardware::getCompressor() == 0)
            && ((readTimer() > (30*60)) || (state == COMPRESSOR_STATE_START))
        )
        {
            setCompressor(1);
            return;
        }
        return;
    }
    if((settings.state & (REFRIGERATOR_STATE_ERROR_AMBI | REFRIGERATOR_STATE_ERROR_EVAP)) == REFRIGERATOR_STATE_ERROR_AMBI)
    {
        if(hardware::getCompressor() == 0)
        {
            state |= (COMPRESSOR_STATE_ERROR_AMBIENCE);
            if(cut_in_check())
            {
                setCompressor(1);
            }
        }
        else if((readTimer() > (20*60))
         || ((state & COMPRESSOR_STATE_ERROR_AMBIENCE) == 0)
         )
            setCompressor(0);
        state &= ~(COMPRESSOR_STATE_ERROR_EVAP);
        return;
    }
    if((settings.state & (REFRIGERATOR_STATE_ERROR_AMBI | REFRIGERATOR_STATE_ERROR_EVAP)) == REFRIGERATOR_STATE_ERROR_EVAP)
    {
        if(hardware::getCompressor())
        {
            state |= (COMPRESSOR_STATE_ERROR_EVAP);
            if(cut_out_check())
            {
                setCompressor(0);
            }
        }
        else if(((readTimer() > (20*60)) || (state == COMPRESSOR_STATE_START))
         || ((state & COMPRESSOR_STATE_ERROR_EVAP) == 0)
        // && (cut_out_check() == false)
        )
            setCompressor(1);
        state &= ~(COMPRESSOR_STATE_ERROR_AMBIENCE);
        return;
    }

    state &= ~(COMPRESSOR_STATE_ERROR_AMBIENCE | COMPRESSOR_STATE_ERROR_EVAP);

    if(hardware::getCompressor())
    {
        if(cut_out_check())
        {
            setCompressor(0);
            if(settings.state & REFRIGERATOR_STATE_SUPER)
            {
                settings.state &= ~REFRIGERATOR_STATE_SUPER;
                //settings.temperature = settings.temperature_backup;
            }
        }
        return;
    }
    if(cut_in_check()
     || ((settings.state & REFRIGERATOR_STATE_SUPER) && (Sensors::readAmbiance() > 15))
     )
    {
        setCompressor(1);
        return;
    }
}

uint16_t Compressor::readWorkTimer()
{
    return work_timer_U16_;
}

uint16_t Compressor::readTimer()
{
    return (Time::diffU16(timer_U16_, Time::getSecondsU16()));
}

uint8_t Compressor::readState()
{
    return state;
}

void Compressor::resetWorkTimer()
{
    work_timer_U16_ = 0;
}

uint8_t Compressor::isON()
{
    return hardware::getCompressor();
}

void Compressor::event()
{
    if(isON())
    {
    }
    else
    {
    }
}

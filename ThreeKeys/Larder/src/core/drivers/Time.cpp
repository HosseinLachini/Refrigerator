
#define TEST_NONE    0x03
#include "Time.h"
#include "Hardware.h"
//#include "Monitor.h"
#include "Buzzer.h"
#include "Lamp.h"
#include "Door.h"
#include "DisplayPanel.h"
#include "Fan.h"
#include "Compressor.h"
#include "Sensors.h"
#include "Heater.h"
#include "Settings.h"
//#include "Screen.h"
//#include "SerialLog.h"
//#include "AnalogInputsPrivate.h"
#include "atomic.h"

//#define ENABLE_DEBUG
//#include "debug.h"


// time measurement

namespace Time {
    volatile uint32_t interrupts_ = 0;

    uint32_t getInterrupts() {
        uint32_t v;
        ATOMIC_BLOCK(ATOMIC_RESTORESTATE) {
            v = interrupts_;
        }
        return v;
    }
    inline void doInterrupt() {
        interrupts_++;
    }

    void doIdle() {
        //Monitor::doIdle();
        //SerialLog::doIdle();
        DisplayPanel::doIdle();
        if(settings.power)
        {
            Sensors::doIdle();
            Buzzer::doIdle();
            if(settings.test_mode != TEST_MODE)
            {
                Door::doIdle();
                Lamp::doIdle();
                Fan::doIdle();
                Compressor::doIdle();
            #ifndef THREE_KEYS_LARDER
                Heater::doIdle();
            #endif // THREE_KEYS_LARDER
            }
        }
        //AnalogInputs::doIdle();
    }

    void callback() {
        static uint8_t slowInterval = TIMER_SLOW_INTERRUPT_INTERVAL;
        Time::doInterrupt();
        if(--slowInterval == 0){
            slowInterval = TIMER_SLOW_INTERRUPT_INTERVAL;
            //AnalogInputs::doSlowInterrupt();
            //Monitor::doSlowInterrupt();
        }
    }
}

uint32_t Time::getMiliseconds()
{
    uint32_t retu = getInterrupts();
#if TIMER_INTERRUPT_PERIOD_MICROSECONDS == 500
    retu /= 2;
#else
#warning "TIMER_INTERRUPT_PERIOD_MICROSECONDS != 500"
    retu *= TIMER_INTERRUPT_PERIOD_MICROSECONDS;
    retu /= 1000;
#endif

    return retu;
}

uint16_t Time::getMilisecondsU16() {
    return getMiliseconds();
}

uint16_t Time::getInterruptsU16() {
    return getInterrupts();
}


uint32_t Time::getSeconds() {
    return getMiliseconds()/1000;
}

uint16_t Time::getSecondsU16() {
    return getSeconds();
}

uint16_t Time::getMinutesU16() {
    return getMiliseconds()/60000;
}

void Time::delay(uint16_t ms)
{
    uint16_t start = getMilisecondsU16();

    while(diffU16(start, getMilisecondsU16()) < ms) {};
}

//warning: this method runs stuff in background,
//delay may take significantly longer than "ms"
void Time::delayDoIdle(uint16_t ms)
{
    uint16_t start = getMilisecondsU16();
    uint16_t delay;
    do {
        doIdle();
        delay = diffU16(start, getMilisecondsU16());
    } while (delay < ms);

    //LogDebug("delayDoIdle ms:", ms, " delay:", delay);
}


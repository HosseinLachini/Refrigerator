/*
    Pars-Refrigerator - open source firmware for a variety of Pars Refrigerators
    Copyright (C) 2018 Hossein Lachini. All right reserved.

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
#ifndef SETTINGS_H_
#define SETTINGS_H_

#include <stdint.h>
#include "AnalogInputs.h"

//#define THREE_KEYS_LARDER
#ifdef THREE_KEYS_LARDER

    #define MAX_TEMPERATURE_SETTING         8

    #define REFRIGERATOR_STATE_NORMAL       0x00
    //#define REFRIGERATOR_STATE_ECO          0x01
    #define REFRIGERATOR_STATE_SUPER        0x02
    //#define REFRIGERATOR_STATE_DEFROST      0x04
    #ifdef REFRIGERATOR_STATE_DEFROST
        #define REFRIGERATOR_STATE_PRECOOLING   0x08
    #endif // REFRIGERATOR_STATE_DEFROST
    #define REFRIGERATOR_STATE_ERROR_AMBI   0x10
    #define REFRIGERATOR_STATE_ERROR_EVAP   0x20
    #ifdef REFRIGERATOR_STATE_DEFROST
        #define REFRIGERATOR_STATE_ERROR_HEAT   0x40
        #define REFRIGERATOR_STATE_IS_DEFROST(s)    (((s) & (REFRIGERATOR_STATE_DEFROST | REFRIGERATOR_STATE_PRECOOLING)) ==  REFRIGERATOR_STATE_DEFROST)
    #endif // REFRIGERATOR_STATE_DEFROST
    #define REFRIGERATOR_STATE_ERROR_LO     0x80
#else

    #define MAX_TEMPERATURE_SETTING         70

    #define REFRIGERATOR_STATE_NORMAL       0x00
    #define REFRIGERATOR_STATE_ECO          0x01
    #define REFRIGERATOR_STATE_SUPER        0x02
    #define REFRIGERATOR_STATE_DEFROST      0x04
    #ifdef REFRIGERATOR_STATE_DEFROST
        #define REFRIGERATOR_STATE_PRECOOLING   0x08
    #endif // REFRIGERATOR_STATE_DEFROST
    #define REFRIGERATOR_STATE_ERROR_AMBI   0x10
    #define REFRIGERATOR_STATE_ERROR_EVAP   0x20
    #ifdef REFRIGERATOR_STATE_DEFROST
        #define REFRIGERATOR_STATE_ERROR_HEAT   0x40
        #define REFRIGERATOR_STATE_IS_DEFROST(s)    (((s) & (REFRIGERATOR_STATE_DEFROST | REFRIGERATOR_STATE_PRECOOLING)) ==  REFRIGERATOR_STATE_DEFROST)
    #endif // REFRIGERATOR_STATE_DEFROST
#endif // THREE_KEYS_LARDER

#ifdef REFRIGERATOR_STATE_ERROR_HEAT
    #define REFRIGERATOR_STATE_ERROR        (REFRIGERATOR_STATE_ERROR_AMBI | REFRIGERATOR_STATE_ERROR_EVAP | REFRIGERATOR_STATE_ERROR_HEAT | REFRIGERATOR_STATE_ERROR_LO)
#else
    #define REFRIGERATOR_STATE_ERROR        (REFRIGERATOR_STATE_ERROR_AMBI | REFRIGERATOR_STATE_ERROR_EVAP | REFRIGERATOR_STATE_ERROR_LO)
#endif // REFRIGERATOR_STATE_ERROR_HEAT


#define TEST_NONE       0x00
#define TEST_MODE       0x01
#define TEST_STATE_MODE 0x02


#define BEEP_SENSEOR_ENABLE     (1 << 0)
#define BEEP_DOOR_ENABLE        (1 << 1)

struct Settings {

    int16_t  temperature;
    int16_t  temperature_backup;
    bool power;
    uint8_t state;
    uint8_t test_mode;
    int8_t  max_temperature;
    int8_t  min_temperature;
    enum UARTType {Disabled, Normal,  Debug,  ExtDebug, ExtDebugAdc};
    enum FanOnType {FanDisabled, FanAlways, FanProgram, FanTemperature, FanProgramTemperature};

    enum UARTOutput {TempOutput, Separated
#ifdef ENABLE_TX_HW_SERIAL_PIN7_PIN38
        , HardwarePin7, HardwarePin38
#endif
    };
    enum MenuType  {MenuSimple, MenuAdvanced};
    enum MenuButtonsType  {MenuButtonsNormal, MenuButtonsReversed};

    static const uint16_t UARTSpeeds = 5;
    static const AnalogInputs::ValueType TempDifference = ANALOG_CELCIUS(5.12);

    uint16_t fanOn;
    AnalogInputs::ValueType fanTempOn;
    AnalogInputs::ValueType dischargeTempOff;

    uint8_t audioBeep;
    AnalogInputs::ValueType minIc;
    AnalogInputs::ValueType maxIc;
    AnalogInputs::ValueType minId;
    AnalogInputs::ValueType maxId;

    AnalogInputs::ValueType maxPc;
    AnalogInputs::ValueType maxPd;

    AnalogInputs::ValueType inputVoltageLow;
    uint16_t adcNoise;
    uint16_t UART;
    uint16_t UARTspeed;
    uint16_t UARToutput;
    uint16_t menuType;
    uint16_t menuButtons;

    void apply();
    void setDefault();
    uint32_t getUARTspeed() const;
    static const uint32_t UARTSpeedValue[];

    static void load();
    static void save();
    static void check();
    static void restoreDefault();

};

extern Settings settings;

#endif /* SETTINGS_H_ */

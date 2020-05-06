#ifndef THREE_PUSH_BUTTON_SWITCH_H
#define THREE_PUSH_BUTTON_SWITCH_H

#include <stdint.h>

namespace DisplayPanel
{
    enum KeyType { None, Power, Alarm, Temp, Test, State };
    void printTemp(int16_t temp);
    void printHex(uint8_t hex);
    void printBlank();
    void printSu(void);
    void printErrorEvaporator(void);
    void printErrorAmbiance(void);
    void setBrightness(uint8_t b);
    void setAlarm(uint8_t on);
    void setSuper(uint8_t on);
    uint8_t IsAlarmIconOn();
    uint8_t IsSuperIconOn();
    KeyType getKey(void);
    void doIdle();
    void test();
    #ifdef THREE_KEYS_LARDER
        void printErrorLO();
        void printSt();
        void printOn(uint8_t on);
        void printTa();
        void printTe();
        void printCo();
        void printFa();
    #endif // THREE_KEYS_LARDER
}

#endif // THREE_PUSH_BUTTON_SWITCH_H

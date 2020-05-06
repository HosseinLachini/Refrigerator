#ifndef SOFTI2C_H
#define SOFTI2C_H

#include <stdint.h>

namespace SoftI2C
{
    //void set_SDA(void);
    //void set_SCL(void);
    //void clear_SDA(void);
    //void clear_SCL(void);
    //unsigned char read_SDA(void);
    //unsigned char read_SCL(void);
    void start(void);
    void stop(void);
    void write(uint8_t byte);
    uint8_t read(uint8_t nack);
}

#endif // SOFTI2C_H

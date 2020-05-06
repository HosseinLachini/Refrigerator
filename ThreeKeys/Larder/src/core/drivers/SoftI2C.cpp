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
#include "SoftI2C.h"
#include "Hardware.h"
#include "Utils.h"

void I2C_delay(void)
{
    Utils::delayMicroseconds(4);
}

void set_SDA(void)
{
    hardware::setSDA(1);
}

void set_SCL(void)
{
    hardware::setSCL(1);
}

void clear_SDA(void)
{
    hardware::setSDA(0);
}

void clear_SCL(void)
{
    hardware::setSCL(0);
}

uint8_t read_SDA(void)
{
    return hardware::getSDA();
}

uint8_t read_SCL(void)
{
    return hardware::getSCL();
}

void Write_bit(uint8_t bit)
{
    hardware::setSDA(bit);
    //if(bit)
    //   set_SDA();
    //else
    //    clear_SDA();
    I2C_delay();
    set_SCL();
    I2C_delay();
    clear_SCL();
}

uint8_t Read_bit()
{
    uint8_t bit;
    set_SDA();
    I2C_delay();
    set_SCL();
    I2C_delay();
    bit = read_SDA() ? 1 : 0;
    clear_SCL();
    return bit;
}

void SoftI2C::start(void)
{
    clear_SDA();
    I2C_delay();
    clear_SCL();
}

void SoftI2C::stop(void)
{
    clear_SDA();
    I2C_delay();
    set_SCL();
    I2C_delay();
    set_SDA();
    I2C_delay();
}

void SoftI2C::write(uint8_t byte)
{
    for(uint8_t bit = 0; bit < 8; bit++)
    {
        Write_bit((byte & 0x80));
        byte <<= 1;
    }
    Write_bit(1);
}

uint8_t SoftI2C::read(uint8_t nack)
{
    uint8_t byte = 0;
    for(uint8_t bit = 0; bit < 8; bit++)
    {
        byte = (byte << 1) | Read_bit();
    }
    Write_bit(nack);
    return byte;
}



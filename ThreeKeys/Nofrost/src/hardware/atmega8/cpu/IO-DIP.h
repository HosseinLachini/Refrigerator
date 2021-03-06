/*
    Pars-Refrigerator - open source firmware for a variety of Pars Refrigerators
    Copyright (C) 2013  Tucker Kern. All right reserved.

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
#ifndef IO_H_
#define IO_H_

#include <stdint.h>
#include <avr/io.h>

// Hopefully these aren't invalid for other platforms
#define OUTPUT 1
#define INPUT 0
#define INPUT_PULLUP 3
#define HIGH 1
#define LOW 0
#define EXTERNAL 0

// A0-7 definitions
#define A0 0
#define A1 1
#define A2 2
#define A3 3
#define A4 4
#define A5 5
#define A6 6
#define A7 7
#define A14 14 // 1.30V (BandGap reference) in page 200 datasheet
#define A15 15 // 0N (GND) in page 200 datasheet

#define INLINE_ATTR __attribute__((always_inline))
//#define INLINE_ATTR

namespace IO
{
        // Core IO functions to be implemented in proper target folder
        inline void digitalWrite(uint8_t pinNumber, uint8_t value) INLINE_ATTR;
        inline uint8_t digitalRead(uint8_t pinNumber) INLINE_ATTR;
        inline void pinMode(uint8_t pinNumber, uint8_t mode) INLINE_ATTR;
        inline void analogReference(uint8_t mode) INLINE_ATTR;

        // Auxillery functions utilized for pin to port mapping
        inline volatile uint8_t* pinToPort(uint8_t pinNumber) INLINE_ATTR;
        inline uint8_t pinBitmask(uint8_t pinNumber) INLINE_ATTR;
        inline volatile uint8_t* pinToInputPort(uint8_t pinNumber) INLINE_ATTR;
        inline volatile uint8_t* pinToDDR(uint8_t pinNumber) INLINE_ATTR;

        inline volatile uint8_t pinToADC(uint8_t pinNumber) INLINE_ATTR;

        inline void digitalWrite(uint8_t pinNumber, uint8_t value)
        {
                // Grab the port and proper bitmask for the pin
                volatile uint8_t *port = pinToPort(pinNumber);
                uint8_t bitmask = pinBitmask(pinNumber);

                // Set the output
                if (value) {
                        *port |= bitmask;
                } else {
                        *port &= ~bitmask;
                }
                return;
        }

        inline uint8_t digitalRead(uint8_t pinNumber)
        {
                // Grab the port and proper bitmask for the pin
                volatile uint8_t *port = pinToInputPort(pinNumber);
                uint8_t bitmask = pinBitmask(pinNumber);

                return (*port & bitmask);
        }

        inline void pinMode(uint8_t pinNumber, uint8_t mode)
        {
                volatile uint8_t *port = pinToDDR(pinNumber);
                uint8_t bitmask = pinBitmask(pinNumber);

                if (mode == OUTPUT) {
                        // Mode is 1, Set mode to output
                        *port |= bitmask;
                }else if (mode == INPUT_PULLUP) {
                        // Mode is 2, Set mode to input pullup
                        *port &= ~bitmask;
                        digitalWrite(pinNumber, HIGH);
                }  else {
                        // Set pin to input
                        *port &= ~bitmask;
                }
        }

        inline void analogReference(uint8_t mode)
        {
            if (mode == 0){
                // External reference - Clear bits 6 & 7
                ADMUX &= ~(0b11000000);
            } else {
                // Implement other modes?
            }
        }

        // Convert physical pin number to logical port address
        inline volatile uint8_t* pinToPort(uint8_t pinNumber)
        {
                // Better way to do this?
                switch (pinNumber)
                {
                        // Port B
                        case 14:
                        case 15:
                        case 16:
                        case 17:
                        case 18:
                        case 19:
                        case 9:
                        case 10:
                        return &PORTB;

                        // Port C
                        case 23:
                        case 24:
                        case 25:
                        case 26:
                        case 27:
                        case 28:
                        case 1:
                        return &PORTC;

                        // Port D
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                        case 11:
                        case 12:
                        case 13:
                        return &PORTD;

                        default:
                        return 0;
                }
        }

        // Pin number to bitmask mapping
        inline uint8_t pinBitmask(uint8_t pinNumber)
        {
                switch (pinNumber)
                {
                        // Px0
                        case 14:
                        case 23:
                        case 2:
                        return 1<<0;

                        // Px1
                        case 15:
                        case 24:
                        case 3:
                        return 1<<1;

                        // Px2
                        case 16:
                        case 25:
                        case 4:
                        return 1<<2;

                        // Px3
                        case 17:
                        case 26:
                        case 5:
                        return 1<<3;

                        // Px4
                        case 18:
                        case 27:
                        case 6:
                        return 1<<4;

                        // Px5
                        case 19:
                        case 28:
                        case 11:
                        return 1<<5;

                        // Px6
                        case 9:
                        case 1:
                        case 12:
                        return 1<<6;

                        // Px7
                        case 10:
                        case 13:
                        return 1<<7;

                        default:
                        return 0;
                }

        }

        // Pin number to input register map
        inline volatile uint8_t* pinToInputPort(uint8_t pinNumber)
        {
                switch (pinNumber)
                {
                        // Port B
                        case 14:
                        case 15:
                        case 16:
                        case 17:
                        case 18:
                        case 19:
                        case 9:
                        case 10:
                        return &PINB;

                        // Port C
                        case 23:
                        case 24:
                        case 25:
                        case 26:
                        case 27:
                        case 28:
                        case 1:
                        return &PINC;

                        // Port D
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                        case 11:
                        case 12:
                        case 13:
                        return &PIND;

                        default:
                        return 0;
                }

        }

        // Pin number to data direction register
        inline volatile uint8_t* pinToDDR(uint8_t pinNumber)
        {
                switch (pinNumber)
                {
                        // Port B
                        case 14:
                        case 15:
                        case 16:
                        case 17:
                        case 18:
                        case 19:
                        case 9:
                        case 10:
                        return &DDRB;

                        // Port C
                        case 23:
                        case 24:
                        case 25:
                        case 26:
                        case 27:
                        case 28:
                        case 1:
                        return &DDRC;

                        // Port D
                        case 2:
                        case 3:
                        case 4:
                        case 5:
                        case 6:
                        case 11:
                        case 12:
                        case 13:
                        return &DDRD;

                        default:
                        return 0;
                }
        }

        inline volatile uint8_t pinToADC(uint8_t pinNumber)
        {
            if( pinNumber == 1)
                return A6;
            if( pinNumber == 22)
                return A7;
            if( pinNumber < 22 )
                return A15; // 0.0 V (GND)
            if( pinNumber > 28 )
                return A14; // 1.30 V (BandGap reference)
            return (pinNumber - 23);
        }


}
#endif /* IO_H_ */

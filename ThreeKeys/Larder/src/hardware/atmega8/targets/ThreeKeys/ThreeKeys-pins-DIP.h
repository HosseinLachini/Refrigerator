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
#ifndef PINS_H_
#define PINS_H_

// pin configuration ATmega8 DIP
//pin 1 - n.c
#define SEG_CA1_PIN         2
#define SEG_CA2_PIN         3
#define BUZZER_PIN		    4
#define DOOR_PIN		    5
#define FAN_PIN			    6
//pin 7 - 10 - VCC, GND, n.c, nc
#define SEG_F_PIN           11
#define SEG_A_PIN           12
#define SEG_G_PIN           13
#define SEG_E_PIN           14
#define LED_EN_PIN          15
#define	LAMP_PIN		    16
///must  be defined.
#define	HEATER_PIN		    17
//pin 17 - 22 - n.c, n.c, n.c, AVCC, AREF, GND
#define SEG_B_PIN           23
#define SEG_C_PIN           24
#define SEG_D_PIN           25
#define COMPRESSOR_PIN		26
#define DEFROST_TEMP_PIN	27
#define AMBIANCE_TEMP_PIN	28

#endif /* PINS_H_ */

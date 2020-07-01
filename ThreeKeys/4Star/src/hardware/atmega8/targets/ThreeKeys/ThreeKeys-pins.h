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

// pin configuration
#define BUZZER_PIN          1
#define LED_EN_PIN          2
//pin 3 - 6  GND, Vcc, GND, VCC
#define SEG_E_PIN		    7
#define SEG_G_PIN		    8
#define SEG_A_PIN		    9
#define SEG_F_PIN		    10
#define SEG_B_PIN           11
#define SEG_C_PIN           12
#define SEG_D_PIN		    13
#define SEG_CA2_PIN		    14
#define COMPRESSOR_PIN      15
#define SEG_CA1_PIN		    16
//pin 17 - 21 - GQ7, AVCC, n.c, AREF, GND, CN5, CN5
//#define AMBIANCE_TEMP_PIN   22
//#define DEFROST_TEMP_PIN    23
#define AMBIANCE_TEMP_PIN   24
//#define HEATER_PIN          25
#define DEFROST_TEMP_PIN    26
#define DOOR_PIN            27
//pin 28 - n.c
//pin 29 - RESET
#define FAN_PIN             30
//pin 31 - n.c
#define LAMP_PIN            32
#endif /* PINS_H_ */

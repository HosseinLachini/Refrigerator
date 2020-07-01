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

//pin 1 - 6 - n.c, n.c, GND, Vcc, GND, VCC
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
//pin 17 - 22 - GQ7, AVCC, GQ5, AREF, GND, CN5
#define AMBIANCE_TEMP_PIN   23
#define DEFROST_TEMP_PIN    24
#define DOOR_PIN            25
#define BUZZER_PIN          26
#define FAN_PIN             27
#define LAMP_PIN            28
//pin 29 - RESET
#define HEATER_PIN          30
//pin 31 - 32 - n.c, n.c
#define LED_EN_PIN          32

#endif /* PINS_H_ */


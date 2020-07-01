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
#define COMPRESSOR_PIN		1
//pin 2 - 6 - n.c, GND, Vcc, GND, VCC
#define DISP_SDA		7 // SDA SM1612
#define DISP_SCL		8 // SCL SM1612
#define DOOR_PIN		9
//pin 10 - n.c
#define LED_PIN			11
//pin 12 - n.c
#define BUZZER_PIN		13
//pin 14 - 17 - n.c, MOSI, MISO, SCK
//pin 18 - 23 - AVCC, n.c, AREF, GND, n.c, n.c

//pin 24 - A1
#define AMBIANCE_TEMP_PIN	24
//pin 25 - A2
#define DEFROST_TEMP_PIN	25
//pin 26 - 29 - n.c, n.c, n.c, RESET
#define	LAMP_PIN		30
#define FAN_PIN			31
#define	HEATER_PIN		32

#endif /* PINS_H_ */

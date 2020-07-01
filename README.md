# Welcome to Lachini Refrigerator
This project is an alternative firmware for a variety of [PARS](http://parsappliance.com/) Refrigerators,

![PARS Refrigerator](PARS_LOGO.png)

Don't use it if You don't need to
(not everything is implemented yet).

Three Keys Display Panel
------------------------
Old model of digital temperature controller -thermostat- with **three keys display panel** designed by [ITW APPLIANCE REFRIGERATION PRODUCTS GROUP](http://www.itwappliance.com/novosite/products.php?FhIdCategoria=1), this temperature controller based on PIC microcontroller([PIC16F872](http://ww1.microchip.com/downloads/en/devicedoc/30221c.pdf)). This board used for Larder, 4-Star and Nofrost products in PARS company. I changed MCU to AVR microcontroller and wirte cross-platform program with C++ to easly change MCU to any microcontroller for example you can use an ARM.

![PARS Three Keys Display Panel](/ThreeKeys/Larder/doc/Photo/DisplayPanel.png)


Pars Larder Fridge
--------
![PARS Larder Refrigerator](/ThreeKeys/Larder/doc/Photo/pars%20larder1700.jpg)


Pars 4-Star Fridge
--------
![PARS 4-Star Refrigerator](/ThreeKeys/4Star/doc/Photo/4-Star.jpg)


Pars Nofrost Freezer
--------
![Pars Nofrost Freezer](/ThreeKeys/Nofrost/doc/Photo/F1700-Open-L.jpg)


Features
--------
- Larder Fridge
  - 3 keys : Temp.Setting, Super, ON-OFF
  - Two digits seven-segment
  - Alarm LED
  - Super LED
  - Super mode
  - Ambiance sensor fail mode
  - Evaporator sensor fail mode
  - Ambiance + Evaporator sensors fail mode
  - NTC 2k7 sensor
  - Setpoints 1~8 C
  - Auto test mode
  - State mode
  - Compressor, Fan, Lamp control
- 4-Stars Fridge
  - 3 keys : Temp.Setting, Super, ON-OFF
  - Two digits seven-segment
  - Alarm LED
  - Super LED
  - Super mode
  - Ambiance sensor fail mode
  - Evaporator sensor fail mode
  - Ambiance + Evaporator sensors fail mode
  - NTC 2k7 sensor
  - Setpoints 1~8 C
  - Auto test mode
  - State mode
  - Compressor, Lamp control
- No-Frost Freezer
  - 3 keys : Temp.Setting, Super, ON-OFF
  - Two digits seven-segment
  - Alarm LED
  - Super LED
  - Super mode
  - Defrost mode
  - Ambiance sensor fail mode
  - NTC 2k7 sensor
  - Setpoints -16~-24 C
  - Auto test mode
  - State mode
  - Compressor, Heater, Lamp control
  
  
Touch Display Panel
------
Touch display panel used for Boran product in PARS Mfg. I worked on two model's.The first touch display panel based on the [SM1612](http://pdf.dzsc.com/99999/2016111142339896.pdf) China chip with China languge datasheet! SM1612 sacn LED's and scan touch Key's with [TS08N](http://www.touchsemi.com/Pages/20_Standard_e/TS08_%288CH_Sensor%29_e/TS08N.pdf), MCU connect via I2C to SM1612 for read touch key's and write to LED's.

![PARS Touch Display Panel](/TouchPanel/SM1612/Boran/doc/Photo/DisplayPanel.jpg)

The second touch display panel based on [HOLTEK](https://www.holtek.com) MCU ([BS86D20A-3](https://www.holtek.com/documents/10179/116711/BS86B12A-3_C16A-3_D20A-3v140.pdf) Touch A/D Flash MCU with LED/LCD Driver).

WARNING
-------

Hardware
--------

Usage:
------

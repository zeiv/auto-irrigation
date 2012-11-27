# auto-irrigation #
---------------------------------

Water plants automatically based on the dryness of the soil or at regular intervals using the MSP430 microprocessor.

## Usage ##

Pre-built flashable binaries exist for the following MSP430 devices:
 * MSP430G2553

They exist as \[Build\]/auto-irrigation.txt and can be flashed using Elptronic's Lite FET-Pro430
which can be downloaded from here: [http://www.elprotronic.com/files/FET-Pro430-Lite-Setup.zip](http://www.elprotronic.com/files/FET-Pro430-Lite-Setup.zip)

Note that the pre-built binaries use the default settings listed below.

To use this software on any other devices, you must configure and build the project yourself using Code Composer Studio.

## Configuration & Building ##

There are several variables at the beginning of main.c which can be adjusted to better suit your application:

 * *WATER* - Output pin to control water flow.  Default is P1.6: `BIT6`
 * *PROBE* - Analog input for monitoring soil moisture.  Defult is P1.5/A5: `BIT5`
 * *MIN_DRY* - Minimum ADC reading at which point water flow is triggered.  A value of 0 represents a perfect connection between the probe and ground, so higher values mean dryer soil.  Default is `0x0220`
 * *HYDR* - The difference in ADC levels between MIN_DRY and when water flow is stopped.  I higher number means that water will flow for a longer time.  Note that this value cannot exceed MIN_DRY.  Default is `0x150`
 * *BTN* - Input pin which is connected to a button. Default is P1.3: `BIT3`

To build this project for a device other than the MSP430G2553, create a new project in Code Composer Studio and set up the environment for your device in the new project wizard.  Then, import the source code and change line 13 of main.c to reflect your device.

**NOT ALL MSP430 DEVICES SUPPORT ADC**
Please ensure that your device supports ADC10 defore attempting to build this project.  Some devices may require that you make further adjustments to the source code.

## License ##

**The MIT License (MIT)**

Copyright (c) 2012 Xavier Bick

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

# Lab 0: Hello World

Lab 0 covered the basics of using the MPLAB X IDE with our Uno32 microcontroller. In this lab we coded simple programs that utilized the LEDs and buttons of our [basic I/O shield](https://www.microchip.com/DevelopmentTools/ProductDetails/TDGL005). 

## Overview

The folder hierarchy follows the segments presented in the Lab0 manual.

- **Hello World** - a basic program that turns on LEDs based on buttons being pressed on our I/O shield. A custom button library and LED library was created for this process.
- **NOP** - a basic NOP loop was created and tested.
- **Improved Hello World** - a program that counts up in binary using LEDs. the count resets once all LEDs are lit up. Pressing any of the buttons on the I/O shield also resets the program.
- **External LEDs** - a program that drives to external LEDs on a breadboard with two different frequencies ( 5Hz and 10Hz).
- **Bus Contention** - program that generates a 10Hz square wave signal using an open drain output pin with an external voltage source of 5V. It then checks using another input pin if a button is being pressed


# Lab 2: Servo Controller

Lab 2 covers the serial peripheral interface (SPI), Input Captures (IC), and Output Captures (OC) on the PIC32 microcontroller. Using these special registers we created a program that controlled the speed of an R/C Servo motor using either a rotary encoder or an ultrasonic proximity sensor.

## Overview

The main program is featured inside the **/ServoController.X** directory. This program uses the encoder, ping sensor,  protocol, and RC servo drivers to communicate with the python console program. Using the python GUI, the user can control whether a proximity sensor's distance or the rotary encoder's angle determines the direction and speed of our RC motor.

**Libraries featured in this lab:**

- /include/RotaryEncoder.h
- /include/PingSensor.h
- /include/RCServo.h
- /src/RotaryEncoder.c
- /src/PingSensor.c
- /src/RCServo.c


 

# Lab 5: Complete Closed Loop Control

Lab 5 builds on the main program from lab 4 with an added EEPROM memory bank. It combines most of the libraries we've developed through the course of this class. Instead of maintaining the angular rate, the user sets an angular position in the python lab interface and the PID controller maintains that position using measured data from the rotary encoder. The PID gains are stored in memory using the EEPROM module on the Uno32.  **Please note: Due to COVID-19, we were unable to have access to the tools that made the IR sensor sections of this lab possible. Because of this, my implementation for the SENSOR MODE is present but arbitrarily uses a potentiometer wiper instead of IR to control the position of the DC motor.** See the Lab 5 manual for more information. Due to the lack of time and similarities to lab 4, a lab report was not created for this lab.

## Overview

The main program loads any pre-existing gain values to the microcontroller before waiting for a serial packet from the python lab interface. Using the lab interface, users can set the microcontroller to be either be controlled directly by the console or using a sensor (in this case, just a potentiometer).  Given a commanded position, the microcontroller maintains the position using a PID control loop.

**The following libraries were used for this lab:**

- /include/DCMotorDrive.h
- /include/RotaryEncoder.h
- /include/ADCFilter.h
- /include/NonVolatileMemory.h
- /src/DCMotorDrive.c
- /src/RotaryEncoder.c
- /src/ADCFilter.c
- /src/NonVolatileMemory.c


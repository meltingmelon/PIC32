# Lab 4: PID Control Loop

Lab 4 covers the implementation of a proportional integral derivative controller for maintaining the rate of a 5V DC motor using measured angular rate from a rotary encoder attached to the motor.

## Overview

The main program implements a PID control loop used to maintain the set angular rate of our motor with the measured error calculated from our raw angle captured from our rotary encoder. Using the python lab interface, a user can change the gain values set on our PIC32 microcontroller as well as the rate we'd like our DC motor to maintain. See the Lab 4 manual and Lab 4 report for further details on the project and my specific implementation.

**The library files designed for this program are the following:**

- /include/RotaryEncoder.h
- /include/DCMotorDrive.h
- /include/FeedbackControl.h
- /src/RotaryEncoder.c
- /src/DCMotorDrive.c
- /src/FeedbackControl.c


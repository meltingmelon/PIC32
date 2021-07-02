# Lab 3: Analog-to-Digital Conversion Filters

Lab 3 explores the Inter-Interface Circuit (I2C), writing to EEPROM, and digitally filtering analog-to-digital  (ADC) signals using a Finite Impulse Response (FIR) filter. 

## Overview

In the main program of our lab, our microcontroller loads an initial set of filter waits from memory and applies it to the set of ADC channels designated (A0-A3). The user can then use the python lab interface to update these weights. The 4 switches on the [basic I/O shield](https://www.microchip.com/DevelopmentTools/ProductDetails/TDGL005) are then used to change which channel and filter is being used. Please see section 8 of the lab manual for more information on how this program is configured.

**The following libraries were designed for this lab:**

- /include/ADCFilter.h
- /include/NonVolatileMemory.h
- /src/ADCFilter.c
- /src/NonVolatileMemory.c


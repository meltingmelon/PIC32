# Lab 1: Serial Communication

Lab 1 covers serial communication protocols. In this lab, we designed a custom serial communication protocol using a circular buffer data structure and the UART1 registers of our PIC32 microcontroller. This communication protocol was then used a core component in running future labs. **Please note that the directory listed here is for compiling in MPLABX IDE only. See /include and /src files for more details of its implementation.**

## Overview

In this lab, a custom communication protocol was designed to interface with a python serial GUI written by our professor. We had to create a circular buffer datatype, design a specific packet schema, perform endian conversions, and configure our UART1 registers of our controller to have a baud rate of 115200, 8 data bit, no parity, and 1 stop bit. 

Please refer to the following source and header files to see my implementation:

**Packet and Circular buffer data structures**:

- /include/CircularBuffer.h
- /src/CircularBuffer.c

**Communication Protocol:**

- /include/Protocol.h
- /src/Protocol.c


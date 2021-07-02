/* 
 * File:  PingSensor.h
 * Author: Mel Ho
 * Brief:  A library for the HC-SR04 ultrasonic proximity sensor:
 * https://cdn.sparkfun.com/datasheets/Sensors/Proximity/HCSR04.pdf. To capture and calculate distance
 * in cm, the IC3 register was used to determine time of flight with its interrupt triggering on every edge. Please
 * refer to the PIC32 FRM Sect. 15 Input Capture for more information on how it is configured. State machine for interrupt
 * logic is inside the Lab 2 Report of this Github repo.
 */


#ifndef PINGSENSOR_H
#define	PINGSENSOR_H

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function PingSensor_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes hardware for PingSensor with the needed interrupts */
int PingSensor_Init(void);

/**
 * @Function int PingSensor_GetDistance(void)
 * @param None
 * @return Unsigned Short corresponding to distance in millimeters */
unsigned short PingSensor_GetDistance(void);


#endif	/* ROTARYENCODER_H */


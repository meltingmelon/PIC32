/* 
 * File:   RCServo.h
 * Author: Mel Ho
 * Brief: A library for the SG90 Micro RC servo. The RC servo is controlled using the OC3 registers of the PIC32.
 * See The PIC32 FRM Sect. 16 Output Compare for more information. Using timer 3 the OC3 pin is set to high every 50ms
 * and pulses continuously. The pulse width determines the position of the servo with it defaulting at the center on intialization.
 */

#ifndef RCSERVO_H
#define	RCSERVO_H

/*******************************************************************************
 * PUBLIC #DEFINES                                                            *
 ******************************************************************************/

#define RC_SERVO_MIN_PULSE 600
#define RC_SERVO_CENTER_PULSE 1500
#define RC_SERVO_MAX_PULSE 2400

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function RCServo_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes hardware required and set it to the CENTER PULSE */
int RCServo_Init(void);

/**
 * @Function int RCServo_SetPulse(unsigned int inPulse)
 * @param inPulse, integer representing number of microseconds
 * @return SUCCESS or ERROR
 * @brief takes in microsecond count, converts to ticks and updates the internal variables
 * @warning This will update the timing for the next pulse, not the current one */
int RCServo_SetPulse(unsigned int inPulse);

/**
 * @Function int RCServo_GetPulse(void)
 * @param None
 * @return Pulse in microseconds currently set */
unsigned int RCServo_GetPulse(void);

/**
 * @Function int RCServo_GetRawTicks(void)
 * @param None
 * @return raw timer ticks required to generate current pulse. */
unsigned int RCServo_GetRawTicks(void);

#endif	/* RCSERVO_H */


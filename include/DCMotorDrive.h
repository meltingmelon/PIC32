/*
 * File:   DCMotorDrive.h
 * Author: Mel Ho
 * Brief: This library provides functions to set and get the motorspeed of an RC Servo
 * motor using the output control (OC) register. Please see PIC32 FRM Sect. 16 for more information
 * about the OC registers.
 */

#ifndef DCMOTORDRIVE_H
#define	DCMOTORDRIVE_H

/*******************************************************************************
 * PUBLIC  #DEFINES                                                            *
 ******************************************************************************/
#define MAXMOTORSPEED 1000

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
/**
 * @Function DCMotorDrive_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes timer3 to 2Khz and set up the pins
 * @warning you will need 3 pins to correctly drive the motor  */
int DCMotorDrive_Init(void);


/**
 * @Function DCMotorDrive_SetMotorSpeed(int32_t newMotorSpeed)
 * @param newMotorSpeed, in units of Duty Cycle (+/- 1000)
 * @return SUCCESS or ERROR
 * @brief Sets the new duty cycle for the motor, 0%->0, 100%->1000 */
int DCMotorDrive_SetMotorSpeed(int newMotorSpeed);


/**
 * @Function DCMotorControl_GetMotorSpeed(void)
 * @param None
 * @return duty cycle of motor 
 * @brief returns speed in units of Duty Cycle (+/- 1000) */
int DCMotorDrive_GetMotorSpeed(void);


/**
 * @Function DCMotorDrive_SetBrake(void)
 * @param None
 * @return SUCCESS or FAILURE
 * @brief set the brake on the motor for faster stop */
int DCMotorDrive_SetBrake(void);


#endif	/* DCMOTORDRIVE_H */


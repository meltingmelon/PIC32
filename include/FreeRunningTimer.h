/*
 * File:   FreeRunningTimer.h
 * Author: Mel Ho
 * Brief: Free running timer library. Used to perform an action on the UNO32 microcontroller with
 * a certain frequency without blocking code operations. Please PIC32 FRM Sect. 14 Timers for more information
 * on timer registers.
 */
#ifndef FREERUNNINGTIMER_H
#define	FREERUNNINGTIMER_H

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
/**
 * @Function FreeRunningTimer_Init(void)
 * @param none
 * @return None.
 * @brief  Initializes the timer module */
void FreeRunningTimer_Init(void);

/**
 * Function: FreeRunningTimer_GetMilliSeconds
 * @param None
 * @return the current MilliSecond Count
   */
unsigned int FreeRunningTimer_GetMilliSeconds(void);

/**
 * Function: FreeRunningTimer_GetMicroSeconds
 * @param None
 * @return the current MicroSecond Count
   */
unsigned int FreeRunningTimer_GetMicroSeconds(void);
#endif	/* FREERUNNINGTIMER_H */


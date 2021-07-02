

#ifndef ROTARYENCODER_H
#define	ROTARYENCODER_H

/*******************************************************************************
 * PUBLIC #DEFINES                                                            *
 ******************************************************************************/

#define ENCODER_BLOCKING_MODE 0
#define ENCODER_INTERRUPT_MODE 1

#define TICK_RATE 2

#define MAX_RATED_RPM 12000
#define GEARBOX_RATIO 84 // 84:1
#define SECONDS_IN_MIN 60
#define FULL_REV 0x3FFF
#define TIMER_FREQ_1000 1000

//#define MAX_RATE ((MAX_RATED_RPM / SECONDS_IN_MIN) * FULL_REV * TICK_RATE) / TIMER_FREQ_1000
#define MAX_RATE 8000
/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function RotaryEncoder_Init(char interfaceMode)
 * @param interfaceMode, one of the two #defines determining the interface
 * @return SUCCESS or ERROR
 * @brief initializes hardware in appropriate mode along with the needed interrupts */
int RotaryEncoder_Init(char interfaceMode);

/**
 * @Function int RotaryEncoder_ReadRawAngle(void)
 * @param None
 * @return 14-bit number representing the raw encoder angle (0-16384) */
unsigned short RotaryEncoder_ReadRawAngle(void);


#endif	/* ROTARYENCODER_H */


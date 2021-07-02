/* 
 * File:   RCServo.c
 * Author: Mel Ho
 * Brief: 
 * Created on <month> <day>, <year>, <hour> <pm/am>
 * Modified on <month> <day>, <year>, <hour> <pm/am>
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/

#include "RCServo.h" // The header file for this source file.
#include "BOARD.h"
#include <xc.h>
#include <sys/attribs.h>
#include "Protocol.h"
#include "FreeRunningTimer.h"
#include "string.h"
#include "stdio.h"
#include "MessageIDs.h"
#include "delays.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define FPB (BOARD_GetPBClock())
#define TIMEFREQ_TR3 20 //20Hz
#define TIMER_PRESCALE_32 32 // 1:32

#define FALSE 0
#define TRUE 1

#define TIMER_3 1


#define TIMER_3_PULSE FPB / TIMEFREQ_TR3 / TIMER_PRESCALE_32 // 62500 ticks
#define MS_PERIOD 50 // 50ms

#define CENTER_PULSE (TIMER_3_PULSE / MS_PERIOD) * (RC_SERVO_CENTER_PULSE * .001)
#define MIN_PULSE (TIMER_3_PULSE / MS_PERIOD) * (RC_SERVO_MIN_PULSE * .001)
#define MAX_PULSE (TIMER_3_PULSE / MS_PERIOD) * (RC_SERVO_MAX_PULSE * .001)

/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/
//#define SERVO_TEST 1

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
static unsigned int prevPulse;
static unsigned int inPulseStatus = RC_SERVO_CENTER_PULSE;

unsigned int status;

static char message[MAXPAYLOADLENGTH];

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
/**
 * @Function RCServo_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes hardware required and set it to the CENTER PULSE */
int RCServo_Init(void)
{
    T3CON = 0;
    TMR3 = 0;
    
    T3CONbits.TCKPS = 0b101; // 1:32 prescale
    PR3 = TIMER_3_PULSE;
    
    IPC3bits.T3IP = 3; // Timer 3 priority
    IPC3bits.T3IS = 2;
    
    OC3CON = 0;
    OC3CONbits.OCTSEL = TIMER_3;
    OC3CONbits.OCM = 0b101;
    OC3RS = CENTER_PULSE; //1500us default orientation is center
    
    IFS0bits.OC3IF = 0; //OC3 priority
    IEC0bits.OC3IE = 1;
    
    T3CONbits.TON = TRUE;
    OC3CONbits.ON = TRUE;
    return SUCCESS;
    
}

/**
 * @Function int RCServo_SetPulse(unsigned int inPulse)
 * @param inPulse, integer representing number of microseconds
 * @return SUCCESS or ERROR
 * @brief takes in microsecond count, converts to ticks and updates the internal variables
 * @warning This will update the timing for the next pulse, not the current one */
int RCServo_SetPulse(unsigned int inPulse)
{
    if (inPulse > RC_SERVO_MAX_PULSE || inPulse < RC_SERVO_MIN_PULSE)
        return ERROR;
    
    if (inPulse != prevPulse)
    {
        prevPulse = inPulse;
        
        // conversion from timer ticks to microseconds
        OC3RS = (TIMER_3_PULSE / MS_PERIOD) * (inPulse * 0.001);
    }

    
    return SUCCESS;
}

/**
 * @Function int RCServo_GetPulse(void)
 * @param None
 * @return Pulse in microseconds currently set */
unsigned int RCServo_GetPulse(void)
{
    return prevPulse;
}

/**
 * @Function int RCServo_GetRawTicks(void)
 * @param None
 * @return raw timer ticks required to generate current pulse. */
unsigned int RCServo_GetRawTicks(void)
{
    return (TIMER_3_PULSE / (MS_PERIOD * (prevPulse * .001)));
}

void __ISR(_OUTPUT_COMPARE_3_VECTOR) __OC3Interrupt(void)
{
    IFS0bits.OC3IF = 0;
}

/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/

#ifdef SERVO_TEST
int main(void)
{
    BOARD_Init();
    Protocol_Init();
    RCServo_Init();
    
    sprintf(message, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);

    unsigned int inPulse;
    while(1)
    {
        if (Protocol_ReadNextID() == ID_COMMAND_SERVO_PULSE)
        {
            Protocol_GetPayload(&inPulse);
            inPulse = Protocol_IntEndednessConversion(inPulse);
            RCServo_SetPulse(inPulse);
            
            inPulseStatus = RCServo_GetPulse();
            inPulseStatus = Protocol_IntEndednessConversion(inPulseStatus);
            Protocol_SendMessage(4, ID_SERVO_RESPONSE, &inPulseStatus);
            
        }
    }
    
}
#endif 

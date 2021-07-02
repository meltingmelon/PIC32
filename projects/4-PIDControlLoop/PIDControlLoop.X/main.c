/*
 * File:   main.c
 * Author: Mel Ho
 * Brief: This program uses a proportional integral derivative controller to maintain the angular rate of a DC motor. The desired angular rate and PID gain values are set using the python lab interface and the microcontroller responds back
 * with data on the measured rate and error.
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include <proc/p32mx340f512h.h>
#include <xc.h>
#include <BOARD.h>
#include "FreeRunningTimer.h"
#include "Protocol.h"
#include "RotaryEncoder.h"
#include "string.h"
#include "stdio.h"
#include "delays.h"
#include "MessageIDs.h"
#include "FeedbackControl.h"
#include "DCMotorDrive.h"
#include <sys/attribs.h>
#include <stdlib.h>

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define PACKET_SIZE 12 // three 4 byte ints
#define FEEDBACK_SCALE_FACTOR 13
#define W_SCALE_FACTOR 5.83
#define W_SCALE_DIVIDE_FACTOR .1715

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
static char message[MAXPAYLOADLENGTH];
static unsigned char messageID;

static unsigned int milliseconds = 0;
static unsigned int hz_5ms = 5;
static unsigned int hz_2ms = TICK_RATE;

static signed int sensor = 0;
static signed int reference = 0;
static int operationStatus = 1;

union PIDGains {
    struct 
    {
        int Kp;
        int Ki;
        int Kd;
    }; char asChar[PACKET_SIZE];
};

union feedbackReport {
    struct
    {
        int error;
        int signed currentRate;
        int u;
    }; char asChar[PACKET_SIZE];
};

/*******************************************************************************
 * MAIN  PROGRAM                                                               *
 ******************************************************************************/
int main(void)
{
    BOARD_Init();
    DCMotorDrive_Init();
    FeedbackControl_Init();
    Protocol_Init();
    RotaryEncoder_Init(ENCODER_INTERRUPT_MODE);
    FreeRunningTimer_Init();

    
    sprintf(message, "Lab 4 Interface Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    
    union PIDGains gains;
    union feedbackReport feedback;
      
    feedback.error = 0;
    feedback.currentRate = 0;
    feedback.u = 0;
    

    while(1)
    {
        if (Protocol_IsMessageAvailable())
            {
                messageID = Protocol_ReadNextID();
                switch(messageID)
                {
                    case (ID_FEEDBACK_SET_GAINS):
                        sprintf(message, "Setting Feedback Gains");
                        Protocol_SendDebugMessage(message);
                        
                        Protocol_GetPayload(&gains);
    
                        gains.Kp = Protocol_IntEndednessConversion(gains.Kp);
                        gains.Ki = Protocol_IntEndednessConversion(gains.Ki);
                        gains.Kd = Protocol_IntEndednessConversion(gains.Kd);
                        
                        operationStatus = FeedbackControl_SetProportionalGain(gains.Kp);
                        operationStatus = FeedbackControl_SetIntegralGain(gains.Ki);
                        operationStatus = FeedbackControl_SetDerivativeGain(gains.Kd);
                        operationStatus = FeedbackControl_ResetController();
                        
                        if (operationStatus == SUCCESS)
                            Protocol_SendMessage(1, ID_FEEDBACK_SET_GAINS_RESP, &operationStatus);
                        
                        break;
                        
                    case (ID_COMMANDED_RATE):
                        sprintf(message, "Set New Commanded Rate");
                        Protocol_SendDebugMessage(message);
                        
                        Protocol_GetPayload(&reference);
                        reference = Protocol_IntEndednessConversion(reference);
                        DCMotorDrive_SetMotorSpeed(reference);
                        break;
                }
            }
        milliseconds = FreeRunningTimer_GetMilliSeconds();
        if (milliseconds > hz_2ms)
        {
            hz_2ms += TICK_RATE;
            // calculate motor speed
            feedback.currentRate = RotaryEncoder_CheckRate(milliseconds);
        }
        
        if (milliseconds > hz_5ms)
        {
            hz_5ms += 100;
            
            // Run PID loop
            feedback.u = FeedbackControl_Update((reference * W_SCALE_FACTOR), feedback.currentRate);
            
            // calculate the error
            feedback.error = (reference * W_SCALE_FACTOR) - feedback.currentRate;
            
            // Scale the output of the PID loop appropriately
            feedback.u = (feedback.currentRate * W_SCALE_DIVIDE_FACTOR) + (feedback.u >> FEEDBACK_SCALE_FACTOR);

            // set motor speed
            DCMotorDrive_SetMotorSpeed(feedback.u);
            
            // Send Data over
            feedback.u = Protocol_IntEndednessConversion(feedback.u);
            feedback.currentRate = Protocol_IntEndednessConversion(feedback.currentRate);
            feedback.error = Protocol_IntEndednessConversion(feedback.error);
            Protocol_SendMessage(sizeof(feedback.asChar), ID_REPORT_FEEDBACK,&feedback.asChar);
        }


    }
    
}

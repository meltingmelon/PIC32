/*
 * File:   FeedbackControl.c
 * Author: Mel Ho
 * Brief: Proportional Integratral Derivative Control library for a DC Motor.
 */


/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include <proc/p32mx340f512h.h>
#include <xc.h>
#include "NonVolatileMemory.h"
#include <BOARD.h>
#include "FreeRunningTimer.h"
#include "Protocol.h"
#include "RotaryEncoder.h"
#include "string.h"
#include "stdio.h"
#include "delays.h"
#include "MessageIDs.h"
#include "FeedbackControl.h"
#include <sys/attribs.h>
#include <stdlib.h>

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define DELTA_T 1
#define PACKET_SIZE 12 // three 4 byte ints

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
static int32_t Kp;
static int32_t Ki;
static int32_t Kd;

static int32_t u;

static int32_t error;
static int32_t accumulator;
static int32_t derivative;
static int32_t lastSensorVal;
static char message[MAXPAYLOADLENGTH];

union gainData {
    struct
    {
        int32_t Kp;
        int32_t Ki;
        int32_t Kd;
    };
    unsigned char asChar[12];
};

union PIDGains {
    struct 
    {
        int32_t Kp;
        int32_t Ki;
        int32_t Kd;
    }; char asChar[PACKET_SIZE];
};

union feedbackReport {
    struct
    {
        int32_t error;
        int32_t currentRate;
        int32_t u;
    }; char asChar[PACKET_SIZE];
};

union feedbackUpdate {
    struct
    {
        int32_t reference;
        int32_t sensor;
    }; char asChar[8];
};

/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/
//#define FEEDBACK_TEST 1
//#define SET_GAINS 1


/*******************************************************************************
 * PUBLIC FUNCTIONS                                                           *
 ******************************************************************************/

/**
 * @Function FeedbackControl_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes the controller to the default values and (P,I,D)->(1, 0, 0)*/
int32_t FeedbackControl_Init(void)
{
    Kp = 1;
    Ki = 0;
    Kd = 0;
}

/**
 * @Function FeedbackControl_SetProportionalGain(int32_t newGain);
 * @param newGain, integer proportional gain
 * @return SUCCESS or ERROR
 * @brief sets the new P gain for controller */
int32_t FeedbackControl_SetProportionalGain(int32_t newGain)
{
    Kp = newGain;
    return SUCCESS;
}

/**
 * @Function FeedbackControl_SetIntegralGain(int32_t newGain);
 * @param newGain, integer integral gain
 * @return SUCCESS or ERROR
 * @brief sets the new I gain for controller */
int32_t FeedbackControl_SetIntegralGain(int32_t newGain)
{
    Ki = newGain;
    return SUCCESS;
}

/**
 * @Function FeedbackControl_SetDerivativeGain(int32_t newGain);
 * @param newGain, integer derivative gain
 * @return SUCCESS or ERROR
 * @brief sets the new D gain for controller */
int32_t FeedbackControl_SetDerivativeGain(int32_t newGain)
{
    Kd = newGain;
    return SUCCESS;
}

/**
 * @Function FeedbackControl_GetPorportionalGain(void)
 * @param None
 * @return Proportional Gain
 * @brief retrieves requested gain */
int32_t FeedbackControl_GetProportionalGain(void)
{
    return Kp;
}

/**
 * @Function FeedbackControl_GetIntegralGain(void)
 * @param None
 * @return Integral Gain
 * @brief retrieves requested gain */
int32_t FeedbackControl_GetIntegralGain(void)
{
    return Ki;
}

/**
 * @Function FeedbackControl_GetDerivativeGain(void)
 * @param None
 * @return Derivative Gain
 * @brief retrieves requested gain */
int32_t FeedbackControl_GetDerivativeGain(void)
{
    return Kd;
}

/**
 * @Function FeedbackControl_Update(int32_t referenceValue, int32_t sensorValue)
 * @param referenceValue, wanted reference
 * @param sensorValue, current sensor value
 * @brief performs feedback step according to algorithm in lab manual */
int32_t FeedbackControl_Update(int32_t referenceValue, int32_t sensorValue)
{
    error = referenceValue - sensorValue;
    accumulator = accumulator + (error * DELTA_T); // deltaT = 1
    derivative = -1 * (sensorValue - lastSensorVal) / DELTA_T;
    lastSensorVal = sensorValue;
    
    u = (Kp * error) + (Ki * accumulator) + (Kd * derivative);
    
    if (u > MAX_CONTROL_OUTPUT)
    {
        u = MAX_CONTROL_OUTPUT; // clips control input to max value
        accumulator = accumulator - (error * DELTA_T); // anti-windup
    }
    else if (u < (-1 * MAX_CONTROL_OUTPUT))
    {
        u = (-1 * MAX_CONTROL_OUTPUT); // clips control input to min value
        accumulator = accumulator - (error * DELTA_T); // anti-windup
    }
    
    return u;
    
}

/**
 * @Function FeedbackControl_ResetController(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief resets integrator and last sensor value to zero */
int32_t FeedbackControl_ResetController(void)
{
    accumulator = 0;
    lastSensorVal = 0;
    return SUCCESS;
}


#ifdef FEEDBACK_TEST
int32_t main(void)
{
    BOARD_Init();
    FeedbackControl_Init();
    RotaryEncoder_Init(ENCODER_INTERRUPT_MODE);
    DCMotorDrive_Init();
    FreeRunningTimer_Init();
    Protocol_Init();
    
    unsigned int32_t milliseconds = 0;
    unsigned int32_t hz = 100;
    unsigned char messageID;
    unsigned int32_t operationStatus = 1;
    
    
    
    sprintf(message, "Feedback Controller Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    
    union PIDGains gains;
    union feedbackReport feedback;
    union feedbackUpdate feedbackSig;
    
    int32_t w = 0;
    int32_t trash = 0;
    
    feedback.error = 0;
    feedback.currentRate = 0;
    feedback.u = 0;
    

    while(1)
    {
        milliseconds = FreeRunningTimer_GetMilliSeconds();
        if (milliseconds > hz)
        {
            hz += 100;
            if (Protocol_IsMessageAvailable())
            {
                messageID = Protocol_ReadNextID();
                sprintf(message, "message ID: %i", messageID);
                Protocol_SendDebugMessage(message);
                switch(messageID)
                {
                    case (ID_FEEDBACK_SET_GAINS):
                        sprintf(message, "Setting Gains");
                        Protocol_SendDebugMessage(message);
                        Protocol_GetPayload(&gains);
                        gains.Kp = Protocol_IntEndednessConversion(gains.Kp);
                        gains.Ki = Protocol_IntEndednessConversion(gains.Ki);
                        gains.Kd = Protocol_IntEndednessConversion(gains.Kd);


                        operationStatus = FeedbackControl_SetProportionalGain(gains.Kp);
                        operationStatus = FeedbackControl_SetIntegralGain(gains.Ki);
                        operationStatus = FeedbackControl_SetDerivativeGain(gains.Kd);

                        if (operationStatus == SUCCESS)
                            Protocol_SendMessage(1, ID_FEEDBACK_SET_GAINS_RESP, &operationStatus);
                        break;

                    case (ID_FEEDBACK_RESET_CONTROLLER):
                        Protocol_GetPayload(&trash);
                        sprintf(message, "Resetting Controller");
                        Protocol_SendDebugMessage(message);
                        operationStatus = FeedbackControl_ResetController();
                        if (operationStatus == SUCCESS)
                            Protocol_SendMessage(1, ID_FEEDBACK_RESET_CONTROLLER_RESP, &operationStatus);
                        break;

                    case (ID_FEEDBACK_UPDATE):
                       sprintf(message, "Feedback Update");
                       Protocol_SendDebugMessage(message);
                       Protocol_GetPayload(&feedbackSig);
                       feedbackSig.reference = Protocol_IntEndednessConversion(feedbackSig.reference);
                       feedbackSig.sensor = Protocol_IntEndednessConversion(feedbackSig.sensor);

                       w = FeedbackControl_Update(feedbackSig.reference, feedbackSig.sensor);                   
                       w = Protocol_IntEndednessConversion(w);
                       Protocol_SendMessage(sizeof(w), ID_FEEDBACK_UPDATE_OUTPUT, &w);

                       break;



                default:break;

                }
            }
        }

    }
    
}
#endif

#ifdef SET_GAINS
int32_t main(void)
{
    BOARD_Init();
    NonVolatileMemory_Init();
    FeedbackControl_Init();
    RotaryEncoder_Init(ENCODER_INTERRUPT_MODE);
    DCMotorDrive_Init();
    FreeRunningTimer_Init();
    Protocol_Init();
    
    unsigned int32_t milliseconds = 0;
    unsigned int32_t hz = 10;
    unsigned char messageID;
    unsigned int32_t operationStatus = 1;
    unsigned int32_t payload;
    
    union gainData gains;
    gains.Kp = 1000;
    gains.Ki = 400;
    gains.Kd = 50;
    
    union gainData readGains;
    
    sprintf(message, "Feedback Controller Set Gains Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    
    NonVolatileMemory_WritePage(5, sizeof(gains.asChar), gains.asChar);
    sprintf(message, "Wrote to page");
    Protocol_SendDebugMessage(message);
    delay_ms(100);
    sprintf(message, "Reading from page");
    Protocol_SendDebugMessage(message);
    
    NonVolatileMemory_ReadPage(5, sizeof(readGains.asChar), readGains.asChar);
//    delay_ms(100);
    sprintf(message, "Read from page");
    Protocol_SendDebugMessage(message);
    FeedbackControl_SetProportionalGain(readGains.Kp);
    FeedbackControl_SetIntegralGain(readGains.Ki);
    FeedbackControl_SetDerivativeGain(readGains.Kd);
    
    sprintf(message, "Kp: %i", Kp);
    Protocol_SendDebugMessage(message);
    sprintf(message, "Ki: %i", Ki);
    Protocol_SendDebugMessage(message);
    sprintf(message, "Kd: %i", Kd);
    Protocol_SendDebugMessage(message);
    
    readGains.Kp = Protocol_IntEndednessConversion(readGains.Kp);
    readGains.Ki = Protocol_IntEndednessConversion(readGains.Ki);
    readGains.Kp = Protocol_IntEndednessConversion(readGains.Kd);
    Protocol_SendMessage(sizeof(readGains.asChar), ID_DEBUG, &readGains.asChar);
}
#endif
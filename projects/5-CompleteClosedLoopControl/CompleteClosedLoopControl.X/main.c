/*
 * File:   main.c
 * Author: Mel Ho
 * Brief: This program uses a proportional integral derivative controller to maintain the angular position of a DC motor. The desired angular position and PID gain values are set using the python lab interface and the microcontroller responds back
 * with data on the measured rate and error. In addition, gains are also stored in memory using the EEPROM module which loads the controller's gains at start up.
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include <proc/p32mx340f512h.h>
#include <xc.h>
#include <BOARD.h>
#include "FreeRunningTimer.h"
#include "ADCFilter.h"
#include "NonVolatileMemory.h"
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
#include <stdint.h>

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define POTENTIOMETER 0
#define FILTER_LOCATION 3
#define GAINS_LOCATION_COMMAND 1
#define GAINS_LOCATION_SENSOR 2
#define GAINS_LENGTH 3
#define PACKET_LENGTH 12
#define LAB_REPORT_PACKETLENGTH 16
#define RANGE_LIMIT 100000

#define FALSE 0
#define TRUE 1

#define COMMAND_MODE 0
#define SENSOR_MODE 1

/*******************************************************************************
 * STATIC VARIABLES                                                            *
 ******************************************************************************/
static int16_t lowpass[FILTERLENGTH];
static char message[MAXPAYLOADLENGTH];
static unsigned char messageID;
static unsigned char PIDMode;
static int16_t i;

union gainData {
    struct
    {
        int16_t Kp;
        int16_t Ki;
        int16_t Kd;
    };
    unsigned char asChar[PACKET_LENGTH];
};

union lab5Report {
    struct
    {
        int16_t currentError;
        int16_t refSignal;
        int16_t sensorSignal;
        int16_t commandedPosition;
    };
    unsigned char asChar[LAB_REPORT_PACKETLENGTH];
};

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
void motorSafetyLimit(signed int16_t accumAngle, signed int16_t u)
{
    if ((accumAngle > RANGE_LIMIT) && (u > 0))
        DCMotorDrive_SetBrake();
    else if ((accumAngle < -RANGE_LIMIT) && (u < 0))
        DCMotorDrive_SetBrake();
    else
        DCMotorDrive_SetMotorSpeed(u);
}

/*******************************************************************************
 * MAIN  PROGRAM                                                               *
 ******************************************************************************/
int main(void)
{
    BOARD_Init();
    DCMotorDrive_Init();
    FeedbackControl_Init();
    Protocol_Init();
    NonVolatileMemory_Init();
    ADCFilter_Init();
    RotaryEncoder_Init(ENCODER_INTERRUPT_MODE);
    FreeRunningTimer_Init();
    
    union gainData gains;
    union lab5Report labReport;
    uint16_t hz_10ms = 10;
    uint16_t hz_2ms = 2;
    uint16_t milliseconds;
    uint16_t payload;
    uint16_t setStatus = FALSE;
    int16_t reference;
    int16_t operationStatus;
    int16_t u = 0;
    
    
    sprintf(message, "Lab 5 Interface Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    
    NonVolatileMemory_ReadPage(GAINS_LOCATION_COMMAND,sizeof(gains.asChar), gains.asChar);
    FeedbackControl_SetProportionalGain(gains.Kp);
    FeedbackControl_SetIntegralGain(gains.Ki);
    FeedbackControl_SetDerivativeGain(gains.Kd);
    
    NonVolatileMemory_ReadPage(FILTER_LOCATION, sizeof(lowpass), (unsigned char *) lowpass);
    operationStatus = ADCFilter_SetWeights(POTENTIOMETER, lowpass);
    
    PIDMode = COMMAND_MODE;
    // Send current mode
    Protocol_SendMessage(sizeof(PIDMode), ID_LAB5_CUR_MODE, &PIDMode);
    
    // Send current gains
    gains.Kp = Protocol_IntEndednessConversion(gains.Kp);
    gains.Ki = Protocol_IntEndednessConversion(gains.Ki);
    gains.Kd = Protocol_IntEndednessConversion(gains.Kd);
    operationStatus = FeedbackControl_ResetController();
    
    Protocol_SendMessage(sizeof(gains.asChar), ID_FEEDBACK_CUR_GAINS, &gains.asChar);
    delay_ms(20);
    reference = RotaryEncoder_AccumulatedAngle();
    labReport.sensorSignal = reference;
    
    sprintf(message, "Current Commanded Position: %i", reference);
    Protocol_SendDebugMessage(message);
    
    sprintf(message, "Current u: %i", u);
    Protocol_SendDebugMessage(message);
    while (1)
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

                    // Set gains from lab interface
                    gains.Kp = Protocol_IntEndednessConversion(gains.Kp);
                    gains.Ki = Protocol_IntEndednessConversion(gains.Ki);
                    gains.Kd = Protocol_IntEndednessConversion(gains.Kd);
                    
                    operationStatus = FeedbackControl_SetProportionalGain(gains.Kp);
                    operationStatus = FeedbackControl_SetIntegralGain(gains.Ki);
                    operationStatus = FeedbackControl_SetDerivativeGain(gains.Kd);
                    operationStatus = FeedbackControl_ResetController();
                    
                    if (PIDMode == COMMAND_MODE)
                        NonVolatileMemory_WritePage(GAINS_LOCATION_COMMAND, sizeof(gains.asChar), gains.asChar);
                    else if (PIDMode == SENSOR_MODE)
                        NonVolatileMemory_WritePage(GAINS_LOCATION_SENSOR, sizeof(gains.asChar), gains.asChar);
                    
                    if (operationStatus == SUCCESS)
                        Protocol_SendMessage(1, ID_FEEDBACK_SET_GAINS_RESP, &operationStatus);
                    
                    setStatus = TRUE;

                    break;
                    
                case (ID_FEEDBACK_REQ_GAINS):
                    sprintf(message, "Sending Gain Values");
                    Protocol_SendDebugMessage(message);
                    Protocol_GetPayload(&payload);
                    
                    if (setStatus == TRUE)
                    {
                        gains.Kp = Protocol_IntEndednessConversion(gains.Kp);
                        gains.Ki = Protocol_IntEndednessConversion(gains.Ki);
                        gains.Kd = Protocol_IntEndednessConversion(gains.Kd);
                        
                        setStatus = FALSE;
                    }
                    
                    Protocol_SendMessage(sizeof(gains.asChar), ID_FEEDBACK_CUR_GAINS, &gains.asChar);
                    
                    break;
                    
                case (ID_LAB5_SET_MODE):
                    sprintf(message, "Setting PID Mode");
                    Protocol_SendDebugMessage(message);
                    Protocol_GetPayload(&PIDMode);
                    
                    if (PIDMode == COMMAND_MODE)
                        NonVolatileMemory_ReadPage(GAINS_LOCATION_COMMAND, sizeof(gains.asChar), gains.asChar);
                    else if (PIDMode == SENSOR_MODE)
                        NonVolatileMemory_ReadPage(GAINS_LOCATION_SENSOR, sizeof(gains.asChar), gains.asChar);

                    operationStatus = FeedbackControl_SetProportionalGain(gains.Kp);
                    operationStatus = FeedbackControl_SetIntegralGain(gains.Ki);
                    operationStatus = FeedbackControl_SetDerivativeGain(gains.Kd);
                    operationStatus = FeedbackControl_ResetController();
                    
                    gains.Kp = Protocol_IntEndednessConversion(gains.Kp);
                    gains.Ki = Protocol_IntEndednessConversion(gains.Ki);
                    gains.Kd = Protocol_IntEndednessConversion(gains.Kd);
                    
                    if (operationStatus == SUCCESS)
                        Protocol_SendMessage(sizeof(gains.asChar), ID_FEEDBACK_CUR_GAINS,&gains.asChar);
                    
                    setStatus = TRUE;
                    
                    break;
                    
                case (ID_LAB5_REQ_MODE):
                    sprintf(message, "Sending Current Mode");
                    Protocol_SendDebugMessage(message);
                    Protocol_GetPayload(&payload);
                    
                    Protocol_SendMessage(sizeof(PIDMode), ID_LAB5_CUR_MODE, &PIDMode);
                    
                    break;
                    
                    
                case (ID_COMMANDED_POSITION):
                    sprintf(message, "Setting New Commanded Position");
                    Protocol_SendDebugMessage(message);
                    
                    Protocol_GetPayload(&reference);
                    reference = Protocol_IntEndednessConversion(reference);
                    operationStatus = FeedbackControl_ResetController();
                    
                    sprintf(message, "New Commanded Position:%i", reference);
                    Protocol_SendDebugMessage(message);
                    
                    if (operationStatus == SUCCESS)
                    {
                        sprintf(message, "Reset Controller");
                        Protocol_SendDebugMessage(message);
                    }
                    
                    break;
                
                case (ID_ADC_FILTER_VALUES):
                    sprintf(message, "Setting Filter Values");
                    Protocol_SendDebugMessage(message);
                    
                    Protocol_GetPayload(&lowpass);

                    for (i=0; i < FILTERLENGTH; i++)
                    {
                        lowpass[i] = Protocol_ShortEndednessConversion(lowpass[i]);
                    }
                    operationStatus = ADCFilter_SetWeights(POTENTIOMETER, lowpass);
                    NonVolatileMemory_WritePage(FILTER_LOCATION, sizeof(lowpass), (unsigned char *)lowpass);
                    if (operationStatus == SUCCESS)
                        Protocol_SendMessage(sizeof(operationStatus), ID_ADC_FILTER_VALUES_RESP, &operationStatus);
                    break;

            }
        }
        
        
        milliseconds = FreeRunningTimer_GetMilliSeconds();
        if (milliseconds > hz_10ms)
        {
            hz_10ms += 10;
            if (PIDMode == SENSOR_MODE)
            {
                reference = ADCFilter_FilteredReading(POTENTIOMETER);
            }
            labReport.sensorSignal = RotaryEncoder_AccumulatedAngle();
            
            u = FeedbackControl_Update(reference, labReport.sensorSignal);
            u = ((int64_t)u * 1000) >> FEEDBACK_MAXOUTPUT_POWER;
            
            motorSafetyLimit(labReport.sensorSignal, u);

            labReport.currentError = reference - labReport.sensorSignal;
            labReport.refSignal = u;
            labReport.commandedPosition = reference;
            
            labReport.currentError = Protocol_IntEndednessConversion(labReport.currentError);
            labReport.refSignal = Protocol_IntEndednessConversion(labReport.refSignal);
            labReport.commandedPosition = Protocol_IntEndednessConversion(labReport.commandedPosition);
            labReport.sensorSignal = Protocol_IntEndednessConversion(labReport.sensorSignal);
            
            Protocol_SendMessage(sizeof(labReport.asChar), ID_LAB5_REPORT, &labReport.asChar);
        }
       
    }
    
    return 0;
}

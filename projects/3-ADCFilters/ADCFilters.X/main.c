/*
 * File:   main.c
 * Author: Mel Ho
 * Brief: This program loads saved filter weights for the FIR algorithm in the EEPROM memory of the Uno32 microcontroller. It then applies a filter to the scanned inputs of 
 * analog input pins, A0-A3. This program is intended to be used in conjunction with a signal generator.
 */


#include "xc.h"
#include "BOARD.h"
#include "Protocol.h"
#include <stdio.h>
#include "ADCFilter.h"
#include "NonVolatileMemory.h"
#include "FreeRunningTimer.h"
#include "FrequencyGenerator.h"
#include "MessageIDs.h"
#include <strings.h>

static char message[MAXPAYLOADLENGTH];

#define INPUT 1
#define OUTPUT 0

#define MEASUREMENT_WINDOW 100

#define LOW 0
#define HIGH 1

#define MAX_DATA_SIZE 64
#define PAGE_PACKET_SIZE (MAX_DATA_SIZE + ADDRESS_SIZE)
#define BYTE_PACKET_SIZE (ADDRESS_SIZE + BYTE_SIZE)

#define READ_LIMIT 10
#define ADDRESS_SIZE 4
#define BYTE_SIZE 1

#define CHANNEL_0 0
#define CHANNEL_1 1
#define CHANNEL_2 2
#define CHANNEL_3 3

#define FILTER_0 0
#define FILTER_1 1

#define ABSOLUTE_VALUE 0
#define PEAK_TO_PEAK 1

#define SWITCH_1_INIT (TRISDbits.TRISD8 = INPUT) // PIN 2
#define SWITCH_1 (PORTDbits.RD8)
#define SWITCH_2_INIT (TRISDbits.TRISD9 = INPUT) // PIN 7
#define SWITCH_2 (PORTDbits.RD9)
#define SWITCH_3_INIT (TRISDbits.TRISD10 = INPUT) // PIN 8
#define SWITCH_3 (PORTDbits.RD10)
#define SWITCH_4_INIT (TRISDbits.TRISD11 = INPUT) // PIN 35
#define SWITCH_4 (PORTDbits.RD11)

#define LEDS_MIN 0
#define LEDS_MAX 255

#define LERP_FRAC_1261 .000793  // 1/1261: used to create lerp ratio
#define LERP_FRAC_1023 .0009775 // 1/1023: used to create lerp ratio

static short displayMode;
static short prevChannel = 0;
static short prevFilter = 0;
static short rawReading;
static unsigned char messageID;

static int operationStatus;

static unsigned int milliseconds;
static unsigned int hz = 10;
static unsigned int measurementCount = 0;

static unsigned char channelFilter;

static signed short filterWeights[FILTERLENGTH];
static signed short filteredReadings[MEASUREMENT_WINDOW];

unsigned char pageData[MAX_DATA_SIZE];

static signed short min = 0;
static signed short max = 0;
static unsigned short absoluteRead;
static unsigned int i;

static unsigned int page = 0;
static unsigned int pin = 0;

union ADCReadings
{
    struct {
        short unfiltered;
        short filtered;
    };
    char adc[4];
};

union NVMData {
    struct {
        unsigned int address;
        unsigned char data;
    };
    char asChar[BYTE_PACKET_SIZE]; // size of 5 bytes
};

union NVMPageData {
    struct {
        int page;
        unsigned char data[MAX_DATA_SIZE];
    };
    char asChar[PAGE_PACKET_SIZE]; // size of 68 bytes
};

static char message[MAXPAYLOADLENGTH];

/**
 * @Function lerp(uint min, uint max, float ratio)
 * @param min, max, ratio
 * @return uint 
 * @brief Linear interpolation function used to map our encoder angles into a range between 600-2400 motor ticks.*/
unsigned int lerp(unsigned int min, unsigned int max, float ratio)
{
    return min + ratio * (max - min);
}

int main(void)
{
    BOARD_Init();
    Protocol_Init();
    ADCFilter_Init();
    NonVolatileMemory_Init();
    FreeRunningTimer_Init();
    FrequencyGenerator_Init();
    
    union ADCReadings ADCReading;
    ADCReading.filtered = 0;
    ADCReading.unfiltered = 0;
    
    unsigned char channel = CHANNEL_0;
    unsigned char filter = FILTER_0;
    union NVMData byteData;
    union NVMPageData pageInfo;
    unsigned int payload = 0;
    unsigned char byte = 0;
    
    unsigned char channelFilter = 0;
    float lerpRatio = 0;
    unsigned int scaledValue;
    signed int peakToPeak;
    signed int max = 0;
    signed int min = 0;
    
    unsigned int currentFrequency = 0;
    unsigned int freqState = 0;
    
    SWITCH_1_INIT;
    SWITCH_2_INIT;
    SWITCH_3_INIT;
    SWITCH_4_INIT;
    
    LEDS_INIT();
    
    sprintf(message, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
    
    // initializes channel and filter values to 0.
    channelFilter = channel << 4 | filter; 
    Protocol_SendMessage(sizeof(channelFilter), ID_LAB3_CHANNEL_FILTER, &channelFilter);
    
    while (1)
    {
        milliseconds = FreeRunningTimer_GetMilliSeconds();
        if (milliseconds > hz)
        {
            hz += 10;
            messageID = Protocol_ReadNextID();
            
            /********************
            * CHANNEL SELECTION *
             ********************/           
            // Determine what channel is selected based on SW1-SW2
            if (SWITCH_1 == LOW && SWITCH_2 == LOW)
                channel = CHANNEL_0;
            
            else if (SWITCH_1 == HIGH && SWITCH_2 == LOW)
                channel = CHANNEL_1;

            
            else if (SWITCH_1 == LOW && SWITCH_2 == HIGH)
                channel = CHANNEL_2;

            
            else if (SWITCH_1 == HIGH && SWITCH_2 == HIGH)
                channel = CHANNEL_3;

            
            /*********************
            *  FILTER SELECTION  *
             *********************/  
            // Determine which filter is selected using SW3         
            if (SWITCH_3 == LOW)
                filter = FILTER_0;
            
            else if (SWITCH_3 = HIGH)
                filter = FILTER_1;
            
            
            // updates channel and filter configurations
            if (channel != prevChannel || filter != prevFilter)
            {
                channelFilter = channel << 4 | filter; //combines channel filter values and sends them over
                sprintf(message, "Sending new channel filter configuration data.");
                Protocol_SendDebugMessage(message);
                Protocol_SendMessage(sizeof(channelFilter), ID_LAB3_CHANNEL_FILTER, &channelFilter);
                
                prevChannel = channel;
                prevFilter = filter;
            }
            
            /********************
            *   DISPLAY MODE    *
             ********************/  
            // Determine which display mode is selected using SW4
            if (SWITCH_4 == LOW)
                displayMode = ABSOLUTE_VALUE;
            else if (SWITCH_4 == HIGH)
                displayMode = PEAK_TO_PEAK;
            
            /********************
            *     MESSAGE ID    *
             ********************/  
            
            switch(messageID)
            {
            case (ID_ADC_FILTER_VALUES):
                Protocol_GetPayload(&filterWeights);
                
                for (i=0; i < FILTERLENGTH; i++)
                {
                    filterWeights[i] = Protocol_ShortEndednessConversion(filterWeights[i]);
                }
                
                operationStatus = ADCFilter_SetWeights(channel, filterWeights);
                if (operationStatus == SUCCESS)
                {
                    Protocol_SendMessage(sizeof(operationStatus), ID_ADC_FILTER_VALUES_RESP, &operationStatus);
                    
                    // Store new filter coefficients
                    if (filter == FILTER_1){
                        NonVolatileMemory_WritePage((channel * 2) + 1, FILTERLENGTH, (unsigned char *) filterWeights);
                    }
                    if (filter == FILTER_0)
                    {
                        NonVolatileMemory_WritePage((channel * 2), FILTERLENGTH, (unsigned char *) filterWeights);
                    }
                } 
                
                break;
                
            case (ID_LAB3_SET_FREQUENCY):
                Protocol_GetPayload(&currentFrequency);
                currentFrequency = Protocol_ShortEndednessConversion(currentFrequency);
                FrequencyGenerator_SetFrequency(currentFrequency);
                break;
                
            case (ID_LAB3_FREQUENCY_ONOFF):       
                Protocol_GetPayload(&freqState);
                if (freqState)
                    FrequencyGenerator_On();
                else
                FrequencyGenerator_Off();
                
                break;
                
                case (ID_NVM_READ_BYTE):          
                  sprintf(message, "Read byte");
                  Protocol_SendDebugMessage(message);
                  Protocol_GetPayload(&payload);
                  payload = Protocol_IntEndednessConversion(payload);                    
                  byte = NonVolatileMemory_ReadByte(payload);
                  Protocol_SendMessage(sizeof(byte), ID_NVM_READ_BYTE_RESP, &byte);
                break;

                case (ID_NVM_WRITE_BYTE):
                    sprintf(message, "Write byte");
                    Protocol_SendDebugMessage(message);
                    Protocol_GetPayload(&byteData);
                    byteData.address = Protocol_IntEndednessConversion(byteData.address);
                    operationStatus = NonVolatileMemory_WriteByte(byteData.address, byteData.data);
                    if (operationStatus == SUCCESS)
                        Protocol_SendMessage(sizeof(operationStatus), ID_NVM_WRITE_BYTE_ACK, &operationStatus);
                    break;
//
                case (ID_NVM_READ_PAGE):
                      sprintf(message, "Read Page");
                      Protocol_SendDebugMessage(message);
                      Protocol_GetPayload(&payload);
                      payload = Protocol_IntEndednessConversion(payload);
                      NonVolatileMemory_ReadPage(payload, sizeof(pageData), pageData);

                      Protocol_SendMessage(sizeof(pageData), ID_DEBUG, &pageData);
                      Protocol_SendMessage(sizeof(pageData), ID_NVM_READ_PAGE_RESP, &pageData);
                break;

                case (ID_NVM_WRITE_PAGE):
                    sprintf(message, "Write Page");
                    Protocol_SendDebugMessage(message);
                    Protocol_GetPayload(&pageInfo);
                    sprintf(message, "data size: %i", sizeof(pageInfo.data));
                    Protocol_SendDebugMessage(message);
                    pageInfo.page = Protocol_IntEndednessConversion(pageInfo.page);
                    operationStatus = NonVolatileMemory_WritePage(pageInfo.page, sizeof(pageInfo.data), pageInfo.data);
                    if (operationStatus = SUCCESS)
                        Protocol_SendMessage(sizeof(operationStatus), ID_NVM_WRITE_PAGE_ACK, &operationStatus);
                    break;

                
            }

            ADCReading.unfiltered = Protocol_ShortEndednessConversion(ADCFilter_RawReading(channel));
            ADCReading.filtered = ADCFilter_FilteredReading(channel);
            
            filteredReadings[measurementCount] = ADCReading.filtered;

            
            switch (displayMode)
            {
                
            case (PEAK_TO_PEAK):
               // take min and max of filter readings over some window of measurements.
                if (measurementCount % MEASUREMENT_WINDOW == 0)
                {
                    LEDS_SET(0x0);
                    max = -9999;
                    min = 9999;
                    for (i=0; i < MEASUREMENT_WINDOW; i++)
                    {
                        if (filteredReadings[i] > max)
                        {
                            max = filteredReadings[i];
                        }
                        
                        
                        if (filteredReadings[i] < min)
                            min = filteredReadings[i];
                    }
                    
                    peakToPeak = max - min;
                    
                    // Use linear interpolation to scale values from 0-1261 (highest peak to peak value) -> 0-255
                    lerpRatio = peakToPeak * LERP_FRAC_1261;
                    scaledValue = lerp(LEDS_MIN,LEDS_MAX,lerpRatio);
                    LEDS_SET(scaledValue);
                       
                }
                break;
                
            case (ABSOLUTE_VALUE):
                LEDS_SET(0x0);
                
                absoluteRead = ADCReading.filtered;
                
                if (ADCReading.filtered < 0)
                    absoluteRead = (ADCReading.filtered * -1);
                
                // Use linear interpolation to scale values from 0-1023 -> 0-255
                lerpRatio = absoluteRead * LERP_FRAC_1023;
                scaledValue = lerp(LEDS_MIN, LEDS_MAX, lerpRatio);
                LEDS_SET(scaledValue);
                
                break;
            
            default:break;
            }
            
            // NVM load filters in. Currently commented  due to finnicky nature of page read. To check and see if the filters successfully write
            // please switch to Non Volatile Memory and click read page for pages 0-8. It appears that page reading in every 10ms seems to overwhelm
            // I2C EEPROM and then it pretty much hangs. Haven't figured out a great work that consistently fixes this issue :( 
            
           switch(filter)
           {
           case (FILTER_0):
               NonVolatileMemory_ReadPage((channel * 2), FILTERLENGTH, (unsigned char *) filterWeights);
               ADCFilter_SetWeights(channel, filterWeights);
               break;
            
           case (FILTER_1):
               NonVolatileMemory_ReadPage((channel * 2) + 1, FILTERLENGTH, (unsigned char *) filterWeights);
               ADCFilter_SetWeights(channel, filterWeights);
               break;
           }
            
            measurementCount = (measurementCount + 1) % MEASUREMENT_WINDOW;
            ADCReading.filtered = Protocol_ShortEndednessConversion(ADCReading.filtered);
            Protocol_SendMessage(sizeof(ADCReading.adc), ID_ADC_READING, ADCReading.adc);
            

        }
    }
    return 0;
}

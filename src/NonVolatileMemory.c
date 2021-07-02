/* 
 * File:   NonVolatileMemory.c
 * Author: Mel Ho
 * Brief: I2C NonVolatile Memory library. This library includes reading and writing
 * a byte or page in the AT93C46D Serial EEPROM module of the UNO32 microcontroller
 * See PIC32 FRM Sect 24 Inter-Integrated Circuit for more information on how to set the I2C registers.
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include <proc/p32mx340f512h.h>
#include <xc.h>
#include "NonVolatileMemory.h"
#include "BOARD.h"
#include "Protocol.h"
#include "delays.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define EEPROM_24LC256_ADDRESS 0b1010000


#define FPB (BOARD_GetPBClock())
#define I2C_CLOCK_RATE 100000

#define BAUDRATE_VALUE 0x00C5 //see FRM table 24-2
#define ACK 0
#define NACK 1

#define WRITE_BIT 0
#define READ_BIT 1

#define PAGE_SIZE 64

#define FALSE 0
#define TRUE 1

#define LOW 0
#define HIGH 1

#define READ_LIMIT 10
#define MAX_DATA_SIZE 64
#define ADDRESS_SIZE 4
#define BYTE_SIZE 1

#define PAGE_PACKET_SIZE (MAX_DATA_SIZE + ADDRESS_SIZE)
#define BYTE_PACKET_SIZE (ADDRESS_SIZE + BYTE_SIZE)

/*******************************************************************************
 * TEST HARNESSES                                                              *
 ******************************************************************************/
//#define NVM_TEST


/*******************************************************************************
 * PRIVATE TYPEDEFS                                                            *
 ******************************************************************************/
union NVMData {
    struct {
       uint32_t address;
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
/*******************************************************************************
 * PRIVATE FUNCTIONS PROTOTYPES                                                 *
 ******************************************************************************/
void I2C_Idle(void);
void I2C_Start(void);
void I2C_Stop(void);
void I2C_Restart(void);
void I2C_Write(unsigned char byte);
unsigned char I2C_Read(void);
void I2C_SendACK(void);
void I2C_SendNACK(void);

/*******************************************************************************
 * PUBLIC FUNCTION IMPLEMENTATIONS                                             *
 ******************************************************************************/

/*******************************************************************************
 * I2C FUNCTION IMPLEMENTATIONS                                                *
 ******************************************************************************/
void I2C_Idle(void)
{
    while(I2C1CON & 0x1F | I2C1STATbits.TRSTAT);
}

void I2C_Start(void)
{
    I2C_Idle();
    I2C1CONbits.SEN = HIGH;           // START condition enabled
    while(I2C1CONbits.SEN == HIGH);   // wait for START to end
}

void I2C_Stop(void)
{
    I2C1CONbits.PEN = HIGH;          // STOP condition enabled
    while(I2C1CONbits.PEN == HIGH);  // wait for STOP to end
}

void I2C_Restart(void)
{
    I2C1CONbits.RSEN = HIGH;          // RESTART condition enabled
    while(I2C1CONbits.RSEN == HIGH);  // wait for RESTART to end
}

void I2C_Send(unsigned char byte)
{
    I2C1TRN = byte;
    while(I2C1STATbits.TRSTAT == HIGH);  // wait for transmission to finish
    while(I2C1STATbits.TBF == HIGH);     // wait for transmission buffer to be empty
    
    while(I2C1STATbits.ACKSTAT == NACK); // wait until we receive an ACK from slave
}

unsigned char I2C_Receive(void)
{
    I2C1CONbits.RCEN = HIGH;
    while(I2C1CONbits.RCEN == HIGH);
    return I2C1RCV;                      // return buffer value
}

void I2C_SendACK(void)
{
    I2C_Idle();
    I2C1CONbits.ACKDT = ACK;            // set to ACK
    I2C1CONbits.ACKEN = HIGH;        // send ACK
    
    while(I2C1CONbits.ACKEN);           // wait until cleared
}

void I2C_SendNACK(void)
{
    I2C_Idle();
    I2C1CONbits.ACKDT = NACK;        // set to NACK
    I2C1CONbits.ACKEN = HIGH;     // send NACK
    
    while(I2C1CONbits.ACKEN);        // wait until cleared
}

/*******************************************************************************
 * NVM FUNCTION IMPLEMENTATIONS                                                *
 ******************************************************************************/

/**
 * @Function NonVolatileMemory_Init(void)
 * @param None
 * @return SUCCESS or ERROR
 * @brief initializes I2C for usage */
int NonVolatileMemory_Init(void)
{
    I2C1CON = 0;
    I2C1CONbits.DISSLW = HIGH;
    I2C1BRG = BAUDRATE_VALUE;      // as shown in FRM table 24-2 for FPB = 40MHz
    I2C1CONbits.ON = HIGH;
    
    I2C_Stop();
    
    return SUCCESS;
}

/**
 * @Function NonVolatileMemory_ReadByte(int address)
 * @param address, device address to read from
 * @return value at said address
 * @brief reads one byte from device
 * @warning Default value for this EEPROM is 0xFF */
unsigned char NonVolatileMemory_ReadByte(int address)
{
    unsigned char byte = 0;
    
    I2C_Idle();
    I2C_Start();  // START

    I2C_Send((EEPROM_24LC256_ADDRESS << 1) | WRITE_BIT);  // loads EEPROM address with write

    I2C_Send((address >> 8) & 0xFF);                     // Memory Address High Byte

    I2C_Send(address & 0xFF);                           // Memory Address Low Byte
    
    I2C_Restart();

    I2C_Send((EEPROM_24LC256_ADDRESS << 1) | READ_BIT); // loads EEPROM address with read
    
    byte = I2C_Receive();

    I2C_SendNACK();
    I2C_Stop();
    
    delay_ms(50);  // delay to prevent overwhelming
    return byte;
}

/**
 * @Function char NonVolatileMemory_WriteByte(int address, unsigned char data)
 * @param address, device address to write to
 * @param data, value to write at said address
 * @return SUCCESS or ERROR
 * @brief writes one byte to device */
char NonVolatileMemory_WriteByte(int address, unsigned char data)
{
    I2C_Idle();
    I2C_Start();
    
    I2C_Send((EEPROM_24LC256_ADDRESS << 1) | WRITE_BIT); // loads address with write
    
    I2C_Send((address >> 8) & 0xFF);    // Memory Address High Byte

    I2C_Send(address & 0xFF);          // Memory Address Low Byte
    
    I2C_Send(data);
    
    I2C_Stop();
    
    delay_ms(20);     // delay to prevent overwhelming
    return SUCCESS;
}

/**
 * @Function int NonVolatileMemory_ReadPage(int page, char length, unsigned char data[])
 * @param page, page value to read from
 * @param length, value between 1 and 64 bytes to read
 * @param data, array to store values into
 * @return SUCCESS or ERROR
 * @brief reads bytes in page mode, up to 64 at once
 * @warning Default value for this EEPROM is 0xFF */
int NonVolatileMemory_ReadPage(int page, char length, unsigned char data[])
{
    int address = (page << 6);              // move to the next 64 byte block (page)
    I2C1CONbits.SEN = HIGH;
    while (I2C1CONbits.SEN == HIGH);        // wait until start is done


    I2C1TRN = EEPROM_24LC256_ADDRESS << 1;
    while (I2C1STATbits.TRSTAT == HIGH);   // wait until transmission finishes
    while (I2C1STATbits.TBF == HIGH);      // wait until transmit buffer is empty 

    
    while (I2C1STATbits.ACKSTAT == HIGH);  // wait for ACK
    I2C1TRN = (address & 0xFF00) >> 8;     // sending the upper byte
    while (I2C1STATbits.TRSTAT == HIGH);
    while (I2C1STATbits.TBF == HIGH);
    while (I2C1STATbits.ACKSTAT == HIGH);  // wait for ACK

    I2C1TRN = (address & 0xFF);            // sending the lower byte
    while (I2C1STATbits.TRSTAT == HIGH);
    while (I2C1STATbits.TBF == HIGH);
    while (I2C1STATbits.ACKSTAT == HIGH);  // wait for ACK


    I2C1CONbits.RSEN = HIGH;              // Restart
    while (I2C1CONbits.RSEN == HIGH);

    I2C1TRN = (EEPROM_24LC256_ADDRESS << 1) | READ_BIT;  // send the EE_Address with a READ bit
    while (I2C1STATbits.TRSTAT == HIGH); 
    while (I2C1STATbits.TBF == HIGH);

    while (I2C1STATbits.ACKSTAT == HIGH);  // if ACKSTAT returns 1, then the acknowledge wasn't received

    int i;
    for (i = 0; i != length; i++) {

        I2C1CONbits.RCEN = HIGH;
        while (I2C1CONbits.RCEN == HIGH);  // wait for the data from the register
        while (!(I2C1STATbits.RBF));
        data[i] = I2C1RCV;                 // store the data

        if ((i + 1) == length)
            I2C1CONbits.ACKDT = HIGH;     // send NACK if done
        else
            I2C1CONbits.ACKDT = LOW;      // send ACK if not done
        
        I2C1CONbits.ACKEN = HIGH; 
        while (I2C1CONbits.ACKEN == HIGH);
    }

    I2C1CONbits.PEN = HIGH;
    while (I2C1CONbits.PEN == HIGH);

    delay_ms(50);

    return SUCCESS;
}

/**
 * @Function char int NonVolatileMemory_WritePage(int page, char length, unsigned char data[])
 * @param address, device address to write to
 * @param data, value to write at said address
 * @return SUCCESS or ERROR
 * @brief writes one byte to device */
int NonVolatileMemory_WritePage(int page, char length, unsigned char data[])
{
    int address = (page << 6);               // move to the next 64 byte block (page)
    I2C1CONbits.SEN = HIGH;
    while (I2C1CONbits.SEN == HIGH);         // wait until start is done

    I2C1TRN = EEPROM_24LC256_ADDRESS << 1;   // send the EE_Address with a write bit
    while (I2C1STATbits.TRSTAT == HIGH);
    while (I2C1STATbits.TBF == HIGH);


    if (I2C1STATbits.ACKSTAT == 1) {
        return ERROR;
    }


    I2C1TRN = (address & 0xFF00) >> 8;       // sending the upper byte
    while (I2C1STATbits.TRSTAT == HIGH);
    while (I2C1STATbits.TBF == HIGH);


    if (I2C1STATbits.ACKSTAT == HIGH) {
        return ERROR;
    }


    I2C1TRN = (address & 0xFF);               // sending the lower byte
    while (I2C1STATbits.TRSTAT == HIGH);
    while (I2C1STATbits.TBF == HIGH);

    if (I2C1STATbits.ACKSTAT == HIGH) {
        return ERROR;
    }


    int i = 0;
    for (i = 0; i != length; i++) {
        I2C1TRN = data[i];                    // send the data
        while (I2C1STATbits.TRSTAT == HIGH);
        while (I2C1STATbits.TBF == HIGH);

        if (I2C1STATbits.ACKSTAT == HIGH)    // break out if ACK is not received
            return ERROR;
    }


    I2C1CONbits.PEN = HIGH;
    while (I2C1CONbits.PEN == HIGH);


    delay_ms(20);
    
    return SUCCESS;    
}

#ifdef NVM_TEST
#include "xc.h"
#include "BOARD.h"

#include <stdio.h>
#include <strings.h>
#include "MessageIDs.h"
#include "FreeRunningTimer.h"

int main(void)
{
    BOARD_Init();
    Protocol_Init();
    FreeRunningTimer_Init();
    NonVolatileMemory_Init();

    
    unsigned char messageID;
    unsigned char byte = 0;
    unsigned char operationStatus;
    unsigned char pageData[MAX_DATA_SIZE];
    

    uint32_t payload = 0;
    uint32_t milliseconds = 0;
    uint32_t hz = 10;
    union NVMData byteData;
    union NVMPageData pageInfo;
    
    uint32_t i = 0;
    uint32_t check = 0;
    uint32_t byteSize = 0;
    
    sprintf(message, "Protocol Test Compiled at %s %s", __DATE__, __TIME__);
    Protocol_SendDebugMessage(message);
//    
    while(TRUE)
    {       
            messageID = Protocol_ReadNextID();
            milliseconds = FreeRunningTimer_GetMilliSeconds();
            
            if (milliseconds > hz)
            {
                hz += 10;
                if (Protocol_IsMessageAvailable())
                {
                    switch(messageID)
                    {
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

                    default:break;
                    }
                }
            }
    }

   return 0;
}

#endif



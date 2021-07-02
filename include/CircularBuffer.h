/* 
 * File:   CircularBuffer.h
 * Author: Mel Ho
 * Brief: Circular buffer library used to to handle TX and RX buffer operations in our Protocol library.
 *  Users will never interface with this library directly.
 */
#ifndef CIRCULARBUFFER_H
#define	CIRCULARBUFFER_H

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
#define PACKETBUFFERSIZE 5  // how many payloads the buffer has to store, not bytes
#define MAXPAYLOADLENGTH 128 // note that this does include the ID

#define HEAD 204
#define TAIL 185


/*******************************************************************************
 * DATATYPES                                                                   *
 ******************************************************************************/

/* Packet
 * Struct that contains the message ID,
 * Payload data, and length of the message
 */
typedef struct packet{
    unsigned char buffer[MAXPAYLOADLENGTH];
    MessageIDS_t ID;
    unsigned int32_t length;
} packet;

// Pointer to packet struct
typedef struct packet* packetPointer;


/* circBuffer
 * Struct that handles a standard circular buffer
 * Used to send messages (TX buffer) or 
 * receive raw data (receiver buffer)
 */
typedef struct circBuffer {
    unsigned char buffer[MAXPAYLOADLENGTH];
    int32_t head;
    int32_t tail;
    uint32_t length;
} circBuffer;

// Pointer to circBuffer struct
typedef struct circBuffer* cbPointer;

/* circReceiverBuffer
 * Specialized circular buffer for receiving packets
 * from the python console. Probably the ugliest, most
 * redundant code I've ever written for a use case.
 */
typedef struct circReceiverBuffer {
    struct packet payloads[PACKETBUFFERSIZE];
    int32_t head;
    int32_t tail;
    int32_t error;
    uint32_t length;
} circReceiverBuffer;

// Pointer to circReceiverBuffer
typedef struct circReceiverBuffer* cbRPointer;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
/**
 * @Function newPacket(void)
 * @param None
 * @return packet, a new packet buffer
 * @brief Initializes a new packet for the circular buffer
 * */
packet newPacket(void);

/**
 * @Function isInvalidCharacter(unsigned char charIn)
 * @param charIn, unsigned char
 * @return TRUE or FALSE (1 or 0)
 * @brief Checks to see if a character is equal to HEAD or TAIL.
 * */
int32_t isInvalidCharacter(unsigned char charIn);

/**
 * @Function newCircReceiverBuffer(cbRPointer circRBuff)
 * @param circBuff, RX circular buffer pointer
 * @return None
 * @brief Initializes RX circular buffer
 * */
void newCircReceiverBuffer(cbRPointer circRBuff);

/**
 * @Function newCircReceiverBuffer(cbRPointer circRBuff)
 * @param circBuff, circular buffer pointer
 * @return None
 * @brief Initializes circular buffer
 * */
void newCircBuffer(cbPointer circBuff);

/**
 * @Function getBuffLength(cbPointer circBuff)
 * @param circBuff, circular buffer pointer
 * @return buffer length or NULL_VAL_ERROR
 * @brief Returns the length of the buffer. If the buffer is NULL
 * return a NULL_VAL_ERROR
 * */
uint32_t getBuffLength(cbPointer circBuff);

/**
 * @Function getBuffLength(cbPointer circRBuff)
 * @param circRBuff, circular buffer pointer
 * @return buffer length or NULL_VAL_ERROR
 * @brief Returns the length of the RX buffer. If the buffer is NULL
 * return a NULL_VAL_ERROR
 * */
uint32_t getReceiverBuffLength(cbRPointer circRBuff);

/**
 * @Function isEmpty(cbPointer circBuff)
 * @param circBuff, circular buffer pointer
 * @return TRUE or FALSE (1 or 0)
 * @brief Checks to see if circular buffer is empty
 * */
uint32_t isEmpty(cbPointer circBuff);

/**
 * @Function isFull(cbPointer circBuff)
 * @param circBuff, circular buffer pointer
 * @return TRUE or FALSE (1 or 0)
 * @brief Checks to see if circular buffer is full
 * */
uint32_t isFull(cbPointer circBuff);

/**
 * @Function read(void* payload)
 * @param circBuff, circular buffer pointer
 * @return payload, character from buffer
 * @brief Returns characters stored at the HEAD of the circular buffer
 * and increments HEAD */
unsigned char read(cbPointer circBuff, unsigned char data);

/**
 * @Function write(void* payload)
 * @param circBuff, data,  circular buffer and payload data
 * @return None
 * @brief Writes data into the TAIL of the circular buffer
 * and increments TAIL*/
void write(cbPointer circBuff, unsigned char data);

#endif	/* CIRCULARBUFFER_H */
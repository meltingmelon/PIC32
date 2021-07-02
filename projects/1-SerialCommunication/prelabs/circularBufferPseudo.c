// Circular Buffer 
// Pseudo-code
//
#define TRUE 1
#define FALSE 1

struct circBuffer
{
        uint8_t head;
        uint8_t tail;
        uint8_t data[BUFF_LEN];
}   circBuffer;

// init
void circBuffer_Init()
{
        circBuffer.head = 0;
        circBuffer.tail = 0;
}

// Enqueue
void enqueue(uint8_t inData)
{
        if bufferIsFull()
                return -1;
        circBuffer.data[circBuffer.tail] = inData;
        circBuffer.tail = (circBuffer.tail + 1) % BUFF_LENGTH;
}

// Dequeue
uint8_t dequeue()
{
        if bufferIsEmpty()
                return -1;
        data = circBuffer.data[circBuffer.head];
        circBuffer.head = (circBuffer.head + 1) % MAX_BUFFER_LENGTH;
        return data;
}

// Test if circular buffer is empty
int bufferIsEmpty()
{
        return (circBuffer.head == circBuffer.tail)
}
// Test if circular buffer is full
int bufferIsFull()
{
        return circBuffer.head == ((circBuffer.tail + 1) % BUFF_LEN)
}
// Return the number of items in the circular buffer
int bufferNum()
{
        int tail = circBuffer.tail
        if (circBuffer.tail < circBuffer.head)
                tail =(BUFF_LEN + circBuffer.tail);

        return (tail - circBuffer.head);
}


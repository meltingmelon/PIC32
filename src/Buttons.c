/*
 * File:   Buttons.c
 * Author: Mel Ho
 * Brief: This library provides an interface to the 4 pushbuttons on the Digilent Basic IO shield for their
 * Uno32 development board platform.
 *
 * This library relies on continuously polling the pins connected to the pushbuttons. It then
 * provides an event-based interface for use. The resultant output are either UpEvents or DownEvents
 * corresponding to whether the button has been pushed-down or released. This h file was originally written by
 * my professor Max Dunne and was refactored to fit styling conventions
 */

/*******************************************************************************
 * #INCLUDES                                                                   *
 ******************************************************************************/
#include <stdint.h>
#include <xc.h>
#include "Buttons.h"
#include "BOARD.h"
#include "leds.h"

/*******************************************************************************
 * PRIVATE #DEFINES                                                            *
 ******************************************************************************/
#define DOWN 0x000F
#define UP 0x0010
#define TRUNCATE 0x00E0

/*******************************************************************************
 * PRIVATE VARIABLES                                                           *
 ******************************************************************************/
unsigned char bufferBTN1 = 0x0000;
unsigned char bufferBTN2 = 0x0000;
unsigned char bufferBTN3 = 0x0000;
unsigned char bufferBTN4 = 0x0000;


unsigned char btn1State = BUTTON_EVENT_1UP;
unsigned char btn2State = BUTTON_EVENT_2UP;
unsigned char btn3State = BUTTON_EVENT_3UP;
unsigned char btn4State = BUTTON_EVENT_4UP;

/*******************************************************************************
 * PUBLIC FUNCTIONS                                                            *
 ******************************************************************************/
/**
 * @Function BUTTONS_Init(void)
 * @param None
 * @return None
 * @brief This function initializes the proper pins such that the buttons 1-4 may be used by modifying
 * the necessary bits in TRISD/TRISF. Only the bits necessary to enable the 1-4 buttons are
 * modified so that this library does not interfere with other libraries.*/
void BUTTONS_Init(void)
{
    TRISD |= 0x00E0;
    TRISF |= 0x0002;
}

/**
 * @Function BUTTONS_Check_Events(void)
 * @param None
 * @return buttonConfig, an unsigned char with the state of each button
 * @brief  This function checks the button states and returns any events that have occured since the last
 * call. In the case of the first call to ButtonsCheckEvents() after ButtonsInit(), the function
 * should assume that the buttons start in an off state with value 0. Therefore if no buttons are
 * pressed when ButtonsCheckEvents() is first called, BUTTONS_EVENT_NONE should be returned. The
 * events are listed in the ButtonEventFlags enum at the top of this file. This function should be
 * called repeatedly.
 *
 * This function also performs debouncing of the buttons. Every time ButtonsCheckEvents() is called,
 * all button values are saved, up to the 4th sample in the past, so 4 past samples and the present
 * values. A button event is triggered if the newest 4 samples are the same and different from the
 * oldest sample. For example, if button 1 was originally down, button 1 must appear to be up for 4
 * samples and the last BTN3 event was BUTTON_EVENT_3DOWN before a BUTTON_EVENT_1UP is triggered. 
 * This eliminates button bounce, where the button may quickly oscillate between the ON and OFF state
 * as it's being pressed or released.*/
unsigned char BUTTONS_Check_Events(void)
{
    bufferBTN1 = bufferBTN1 << 1;
    bufferBTN1 |= BTN1;
    bufferBTN1 &= ~TRUNCATE;
    
    bufferBTN2 = bufferBTN2 << 1;
    bufferBTN2 |= BTN2;
    bufferBTN2 &= ~TRUNCATE;
    
    bufferBTN3 = bufferBTN3 << 1;
    bufferBTN3 |= BTN3;
    bufferBTN3 &= ~TRUNCATE;
    
    bufferBTN4 = bufferBTN4 << 1;
    bufferBTN4 |= BTN4;
    bufferBTN4 &= ~TRUNCATE;
    
    if ((bufferBTN1) == DOWN) {
      btn1State = BUTTON_EVENT_1DOWN;
    }
    if ((bufferBTN1) == UP) {
      btn1State = BUTTON_EVENT_1UP;
    }
    
    if ((bufferBTN2) == DOWN) {
      btn2State = BUTTON_EVENT_2DOWN;
    }
    if ((bufferBTN2) == UP) {
      btn2State = BUTTON_EVENT_2UP;
    }
    
    if ((bufferBTN3) == DOWN) {
      btn3State = BUTTON_EVENT_3DOWN;
    }
    if ((bufferBTN3) == UP) {
      btn3State = BUTTON_EVENT_3UP;
    }
    
    if ((bufferBTN4) == DOWN) {
      btn4State = BUTTON_EVENT_4DOWN;
    }
    if ((bufferBTN4) == UP) {
      btn4State = BUTTON_EVENT_4UP;
    }
    
    unsigned char buttonConfig = BUTTON_EVENT_NONE;

    buttonConfig = buttonConfig | btn1State | btn2State | btn3State | btn4State;

    return buttonConfig;
}
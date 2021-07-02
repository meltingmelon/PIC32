/* 
 * File:   Buttons.h
 * Author: Mel Ho
 * Brief: This library provides an interface to the 4 pushbuttons on the Digilent Basic IO shield for their
 * Uno32 development board platform.
 *
 * This library relies on continuously polling the pins connected to the pushbuttons. It then
 * provides an event-based interface for use. The resultant output are either UpEvents or DownEvents
 * corresponding to whether the button has been pushed-down or released. This h file was originally written by
 * my professor Max Dunne and was refactored to fit styling conventions
 */

#ifndef BUTTONS_H
#define BUTTONS_H

#include <stdint.h>
#include "BOARD.h"

/*******************************************************************************
 * PUBLIC #DEFINES                                                             *
 ******************************************************************************/
#define BUTTONS_DEBOUNCE_PERIOD 4

/*******************************************************************************
 * PUBLIC VARIABLES                                                            *
 ******************************************************************************/
/**
 * This enum{} lists all of the possible button events that could occur. Each event constants were
 * chosen so that multiple button events can be recorded in a single call to ButtonsCheckEvents().
 * All possible event flags will also fit into a char datatype.
 *
 * In order to check for events occuring use bit-masking as the following code demonstrates:
 * 
 * uint8_t bEvent = ButtonsCheckEvents();
 * if (bEvent & BUTTON_EVENT_1DOWN) {
 *   // Button 1 was pressed
 * }
 */
typedef enum {
    BUTTON_EVENT_NONE = 0x00,
    BUTTON_EVENT_1UP = 0x01,
    BUTTON_EVENT_1DOWN = 0x02,
    BUTTON_EVENT_2UP = 0x04,
    BUTTON_EVENT_2DOWN = 0x08,
    BUTTON_EVENT_3UP = 0x10,
    BUTTON_EVENT_3DOWN = 0x20,
    BUTTON_EVENT_4UP = 0x40,
    BUTTON_EVENT_4DOWN = 0x80
} ButtonEventFlags;

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
void BUTTONS_Init(void);

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
unsigned char BUTTONS_Check_Events(void);


#endif // BUTTONS_H

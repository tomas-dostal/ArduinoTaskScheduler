/*
 * Copyright 2011 Alan Burlison, alan@bleaklow.com.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Simple task classes.
 */
#include "Task.h"
/*********************************************************************************
*  Class:    Blinker
* Task Type:  TimedTask (runs when a specific time interval is reached)
* Purpose:  Blinks the on-board Arduino LED (LED_BLINKER: Pin 13) at a specific rate
*       (RATE_BLINKER_BLINK) and utilizes the pointer ptrDebugger 
*       to print simple debug messages to the Serial Monitor 
*       
*       (e.g. "BLINKER: ON" and "BLINKER_OFF")
*       
*********************************************************************************/

// ***
// *** Define the Blinker Class as type TimedTask
// ***
class Temperature : public TimedTask
{
public:



  // Create a new blinker for the specified pin and rate.
  //AdvancedBlinker(uint8_t _pin, uint32_t _time_on, uint32_t _time_off, Debugger *_ptrDebugger);
    Temperature(uint8_t _pin, uint32_t _refresh_rate, Debugger *_ptrDebugger, LCD * _ptr_lcd);

  virtual void run(uint32_t now);
  virtual void update(uint32_t now){};

private:
  uint8_t pin;        // LED pin.
  uint32_t refresh_rate;
  Debugger *ptrDebugger;    // Pointer to debugger
  LCD * ptr_lcd;

};

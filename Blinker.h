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
class Blinker : public TimedTask
{
public:
  // Create a new blinker for the specified pin and rate.
  Blinker(uint8_t _pin, uint32_t _rate, Debugger *_ptrDebugger);
  
  Blinker(uint8_t _pin, uint32_t _rate);

  virtual void run(uint32_t now);
  virtual void run(DateTime dt_now);

  virtual void update(uint32_t now){};
  virtual void update(DateTime dt_now){};
  
 // virtual bool canUpdate(uint32_t now){return false;};
 // virtual bool canUpdate(DateTime dt_now){return false;};

private:
  uint8_t pin;        // LED pin.
  uint32_t rate;        // Blink rate.
  bool on;          // Current state of the LED.
  Debugger *ptrDebugger;    // Pointer to debugger
  bool debug_enabled; 
};

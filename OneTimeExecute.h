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
class OneTimeExecute : public TriggeredTimeTask
{
public:
  // Create a new blinker for the specified pin and rate.
  OneTimeExecute(String _taskname, uint8_t _pin, uint32_t _active_time, Debugger *_ptrDebugger, LCD * _ptr_lcd);
  //OneTimeExecute(String _taskname, uint8_t _pin, uint32_t _active_time, Debugger *_ptrDebugger); 
  //OneTimeExecute(String _taskname, uint8_t _pin, Debugger *_ptrDebugger); 
    
  OneTimeExecute(String _taskname, uint8_t _pin, DateTime * _dt_startTime, TimeSpan *_dt_activeTime, Debugger *_ptrDebugger, LCD * _ptr_lcd);

  virtual void run(uint32_t now);
  virtual void update(uint32_t now);

  virtual void run(DateTime dt_now);
  virtual void update(DateTime dt_now);

  virtual bool isRtcTask(){return RtcTask;};


private:

  String taskname;
  bool on = false;          // Current state of the LED.
  uint8_t pin;        // LED pin.

  uint32_t started; 
  uint32_t active_time;        // Blink rate.
  // OR 
  //DateTime * dt_runTime;
  //TimeSpan * dt_activeTime;
  
  bool RtcTask;           // Current state of the LED.
  
  bool lcd_printout;          // is LCD defined
  Debugger *ptrDebugger;    // Pointer to debugger
  LCD *ptr_lcd;    // Pointer to debugger
};

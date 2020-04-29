/*
 * Copyright 2011 Alan Burlison, alan@bleaklow.com.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Simple task classes.
 */
#include "Task.h"

class Fader : public TimedTask
{
  public:
    Fader(uint8_t _pin, uint8_t _increment, uint32_t _rate, Debugger *_ptrDebugger);
    Fader(uint8_t _pin, uint8_t _increment, uint32_t _rate, Debugger *_ptrDebugger, LCD * _ptr_lcd);
    
    virtual void update(uint32_t now){};
    virtual void run(uint32_t now);
  private:
    uint8_t pin;
    uint8_t increment;
    uint32_t rate;
    uint8_t brightness;
    LCD * ptr_lcd;
    bool display_connedted = false; 
    Debugger *ptrDebugger;
};

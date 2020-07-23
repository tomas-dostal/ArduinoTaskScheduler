/*********************************************************************************
*  Class:    Button
*  Task Type:  TimedTask (runs when a specific time interval is reached)
*  Purpose:  Controls button(s) and runs specific defined actions 
* + writes debug messages
*       
*       (e.g. "Button: short press" and "Button: long press")
*       
*  code inspired by https://arduinogetstarted.com/tutorials/arduino-button-long-press-short-press 
*       
*********************************************************************************/

#include <Arduino.h>

#include "Task.h"
#include "Debugger.h"
#include "Button.h"

Button::Button(uint8_t _pin, TriggeredTimeTask* _task_to_run, Debugger* _ptrDebugger)
    : TimedTask(millis())
    , buttonPin(_pin)
    , debug_enabled(true)
    , ptrDebugger(_ptrDebugger)
    , ptrTaskToRun(_task_to_run)
{
    pinMode(_pin, INPUT_PULLUP);

    Serial.println("Button constructor");
    Serial.println("run at " + String(runTime));

    setRunTime(20); // set update interval
}

// ***
// *** Button::run() <--executed by TaskScheduler as a result of canRun() returning true.
// ***
void Button::run(uint32_t now)
{
    // read the state of the switch/button:
    currentState = digitalRead(buttonPin);

    if (lastState == HIGH && currentState == LOW) // button is pressed
        pressedTime = millis();

    else if (lastState == LOW && currentState == HIGH) { // button is released
        releasedTime = millis();

        long pressDuration = releasedTime - pressedTime;

        if (pressDuration < shortPressTime && pressDuration > debounce) {
            // replace with your own call
            Serial.println("A short press is detected");
            ptrTaskToRun->disablePeriod();
        }

        else if (pressDuration > longPressTime && pressDuration > debounce) {
            // replace with your own call
            Serial.println("A long press is detected");
            ptrTaskToRun->enablePeriod();
            //ptrTaskToRun->setRunnable();
            ptrTaskToRun->runAgainAt(now);
        }
    }

    // save the the last state
    lastState = currentState;
}

void Button::run(DateTime dt_now)
{
    run(millis());
}

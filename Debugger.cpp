/**************************************************************************************
  Description:  ArduinoTaskSheduller Debugger class
  Type:         Simple Task
  Purpose:      Universal interface for debugging

  Based on: Kevin Gagnon https://github.com/gadgetstogrow/TaskScheduler

  Original copyright: Copyright 2011 Alan Burlison, alan@bleaklow.com.
  All rights reserved. Use is subject to license terms.

  Modified by: Tomas Dostal <t@xn--dostl-0qa.eu> <t@dostál.eu> https://xn--dostl-0qa.eu

**************************************************************************************/
/*****************************************************************************************
*  Class:    Debugger
* Task Type:  Task (always runs)
* Purpose:  This expands on Alan Burlison's original example code which demonstrates
*       a task that reads from the serial port and echoes to the Serial Monitor.
*       I've expanded it so that other classes use a pointer to the debugger object
*       to output simple debug messages while this example executes.
*       Classes that use the debugger object are passed a reference to &debugger
*       in their respective constructors.
*
*       For example: Blinker(uint8_t _pin, uint32_t _rate, Debugger *_ptrDebugger);
*
*       To output debug information use: ptrDebugger->debugWrite("debug info");
*
* Notes:    Yeah, I lazily used the String() function in this demonstration. Suedfbvbvfbfvvvvvvvb
*me.
******************************************************************************************/


#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif

// *** Define the Debugger Class as type Task
// ***
#include "Task.h"
#include "Debugger.h"


// ***
// *** Debugger Constructor
// ***

Debugger::Debugger(int costumBaudRate)
    : Task()
{
    Serial.begin(costumBaudRate);
    Serial.println("Debugger costumBaudRate OK");
}
Debugger::Debugger()
    : Task()
{
    Serial.begin(115200);
    Serial.println("Debugger OK");
}

bool Debugger::canRun(uint32_t now)
{
    return Serial.available() > 0;
}
bool Debugger::canRun(DateTime dt_now)
{
    return Serial.available() > 0;
}

// there is no need to update since it is a simple Task
void Debugger::update(uint32_t now)
{
}

// there is no need to update since it is a simple Task
void Debugger::update(DateTime dt_now)
{
}


// Debugger::run() <--executed by TaskScheduler as a result of canRun() returning true.
void Debugger::run(DateTime dt_now)
{
    run(millis());
}
void Debugger::run(uint32_t now)
{
    uint16_t byteCount = 0;

    Serial.println("-----------------");
    Serial.println("Input Received...");
    Serial.println("-----------------");
    while (Serial.available() > 0)
    {
        int byte = Serial.read();
        Serial.print("'");
        Serial.print(char(byte));
        Serial.print("' = ");
        Serial.print(byte, DEC);
        Serial.println(" ");
        if (byte == '\r')
        {
            Serial.print('\n', DEC);
        }

        byteCount++;
    }

    Serial.println("-----------------");
    Serial.print("Bytes Received: ");
    Serial.println(String(byteCount));
    Serial.println("-----------------");
}

// Debugger::debugWrite() Used to write debug messages from other tasks
void Debugger::debugWrite(String debugMsg)
{
    Serial.println(debugMsg);
}

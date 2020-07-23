/**************************************************************************************
  Description:  ArduinoTaskSheduller Display class
  Type:         Simple Task
  Purpose:      Something like universal interface modified to work properly with 
                20x4 I2C display.

                The idea is, that this class would be the only one who controlls where 
                the text is displayed to prevent overwriting

                You can see several ideas how the screen could look like here: 

//  |----------------|
//  |# HH:MM:SS #100%| #... battery status
//  |#35,3°C #35,3°C | #... temperature char
//  |----------------|

//  |--------------------|
//  |DD:MM:SS DD:MM:RR   |
//  |#35,3°C #100%       |
//  |####Progressbar#####|
//  |  CURRENT_STATUS ##1| status (0,15) connected clients (16,19) [ ##1],
//  OTHERWISE status (0,19)
//  |--------------------|

  
  Modified by: Tomas Dostal <t@xn--dostl-0qa.eu> <t@dostál.eu> https://xn--dostl-0qa.eu
  
  I see no reason to place directly here original copyright. If I am wrong, please feel
  free to contact me. 
 
**************************************************************************************/
 
#if ARDUINO < 100 
    #include<WProgram.h>
#else #include<Arduino.h>

#endif 

#include "Display.h"

#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 20, 4);

// *** Define the Debugger Class as type Task
// ***
#include "Task.h"

LCD::LCD(): Task(),
p_lcd(&lcd)
{
    p_lcd -> init(); 
    p_lcd -> backlight();
    p_lcd -> clear();
    writeString("asdfDFDF");
    // p_lcd->print("stoj");

    //  p_lcd->print("String");
    p_lcd -> createChar(0, bar1);
    p_lcd -> createChar(1, bar2);
    p_lcd -> createChar(2, bar3);
    p_lcd -> createChar(3, bar4);
    p_lcd -> createChar(4, bar5);

    p_lcd -> createChar(5, temperature);
    p_lcd -> createChar(6, humidity);     // version 1
    p_lcd -> createChar(7, humidity_2);   // version 2

    p_lcd->createChar(8, wifi_part1);
    p_lcd->createChar(9, wifi_part2);
    p_lcd->createChar(10, lock_part1);
    p_lcd->createChar(11, lock_part2);
    p_lcd->createChar(12, user);

    p_lcd -> clear();
}

void LCD::writeUserChar()
{
    p_lcd -> setCursor(18, 0);
    p_lcd -> write(12); // print user char

}

void LCD::writeWifiChar()
{
    p_lcd -> setCursor(15, 1);
    p_lcd -> write(8); // print wifi_part1 char
    p_lcd -> setCursor(16, 1);
    p_lcd -> write(9); // print wifi_part2 char
}
void LCD::writeLockChar()
{
    p_lcd -> setCursor(18, 0);
    p_lcd -> write(10); // print lock_part1 char
    p_lcd -> setCursor(19, 0);
    p_lcd -> write(11); // print lock_part2 char
}
void LCD::writeTemperatureChar(int row, int index)
{
    p_lcd -> setCursor(index, row);
    p_lcd -> write(5); // print temperature char
}
void LCD::writeHumidityChar(int row, int index)
{
    p_lcd -> setCursor(index, row);
    p_lcd -> write(7); // print humidity1 char 
}
void LCD::run(uint32_t now)
{
    // p_lcd->print("test");
    // Serial.println("-----------------");
    // Serial.print("LCD ");
    // Serial.println("-----------------");
}
void LCD::run(DateTime dt_now)
{
    run(millis()); // call existing contructor
}
void LCD::drawProcessName(String processName)
{
    clearLine(2);
    writeLineFrom(2, 0, processName);
}

void LCD::drawProgressBar(int task_id, uint32_t var, uint32_t minVal, uint32_t maxVal)
{
    if (isAvailable())
        setOwner(task_id);

    if (!isAvailable() && task_id != task_using)
    {
        Serial.println("[err] drawProgressBar used, can not print");
        return;
    }

    Serial.println("drawProgressBar ");
    Serial.print("var - " + String(var));
    Serial.print(" minval - " + String(minVal));
    Serial.println(" maxVal - " + String(maxVal));

    int row = 3;

    int block = map(var, minVal, maxVal, 0, 20); // Block represent the current
    // LCD space (modify the map
    // setting to fit your LCD)
    int line = map(var, minVal, maxVal, 0,
        8); // Line represent the theoretical lines that should be printed
    int bar = map(var, minVal, maxVal, 0, 20 * 5);
    bar -= block * 5;
    // Bar represent the actual lines that will be printed

    /* LCD Progress Bar Characters, create your custom bars */

    for (int x = 0; x < block; x++) // Print all the filled blocks
    {
        p_lcd -> setCursor(x, row);
        p_lcd -> write(1023);
    }

    p_lcd -> setCursor(block, row); // Set the cursor at the current block and
    // print the numbers of line needed
    if (bar != 0)
        p_lcd -> write(bar);
    if (block == 0 && line == 0)
        p_lcd -> write(1022); // Unless there is nothing to print, in this case
    // show blank

    for (int x = 20; x > block; x--) // Print all the blank blocks
    {
        p_lcd -> write(1022);
    }
}
// some other functions that might me also useful, however it would be much better
// if this class would be the only one who controlls where the text is displayed 
// to prevent overwriting

void LCD::writeString(String text)
{
    p_lcd -> setCursor(0, 0);
    p_lcd -> print(text);
}
void LCD::writeLine(int row, String text)
{
    p_lcd -> setCursor(0, row);
    p_lcd -> print(text);
}
void LCD::writeLineFrom(int row, int index, String text)
{
    p_lcd -> setCursor(index, row);
    p_lcd -> print(text);
}
void LCD::clearLine(int line)
{
    p_lcd -> setCursor(0, line);
    p_lcd -> print("                    ");
}

void LCD::clearAll()
{
    p_lcd -> clear();
}

/*
 * Copyright 2011 Alan Burlison, alan@bleaklow.com.  All rights reserved.
 * Use is subject to license terms.
 */


#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif
#include "Display.h"

#include <LiquidCrystal_I2C.h>
#include <LcdBarGraph.h> // no need

LiquidCrystal_I2C lcd(0x3F, 20, 4);
LcdBarGraph lbg(&lcd, 4);  // -- creating

  byte bar1[8] = { 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10};
  byte bar2[8] = { 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18};
  byte bar3[8] = { 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C};
  byte bar4[8] = { 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E};
  byte bar5[8] = { 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F};


// *** Define the Debugger Class as type Task
// ***
#include "Task.h"


//  |----------------|
//  |# HH:MM:SS #100%| #... battery status
//  |#35,3°C #35,3°C | #... temperature char
//  |----------------|

//  |--------------------|
//  |HH:MM:SS  DD:MM:RRRR|
//  |#35,3°C #35,3°C #100|
//  | #
//  |  CURRENT_STATUS ##1| status (0,15) connected clients (16,19) [ ##1], OTHERWISE status (0,19)
//  |--------------------|



LCD::LCD(): Task()
{
    p_lcd = &lcd; 
    p_lcd->init();// inicializuje displej
    p_lcd->backlight(); // zapne podsvětlení  
    p_lcd->clear();
    writeString("asdfDFDF"); 
    //p_lcd->print("stoj");
    
    //  p_lcd->print("String");
   p_lcd->createChar(0, bar1);
   p_lcd->createChar(1, bar2);
   p_lcd->createChar(2, bar3);
   p_lcd->createChar(3, bar4);
   p_lcd->createChar(4, bar5);
   
   p_lcd->createChar(5, temperature);
   p_lcd->createChar(6, humidity);
   p_lcd->createChar(7, humidity_2);
   /*p_lcd->createChar(3, wifi_part1);
   p_lcd->createChar(4, wifi_part2);
   p_lcd->createChar(5, lock_part1);
   p_lcd->createChar(6, lock_part2);
   p_lcd->createChar(7, user);
   */

  
}

void LCD::update(uint32_t now){};



bool LCD::canRun(uint32_t now)
{
  return Serial.available() > 0;
}

void LCD::writeCharTemperature(int row, int index)
{
  p_lcd->setCursor(index, row); 
  p_lcd->write(5); // print temperature char
}
void LCD::writeCharHumidity(int row, int index)
{
  p_lcd->setCursor(index, row); 
  p_lcd->write(7); // print temperature char
}
void LCD::run(uint32_t now)
{
  p_lcd->print("test");
  Serial.println("-----------------");
  Serial.print("LCD "); 
  Serial.println("-----------------");
  
}

void LCD::drawProgressBar(int row, uint32_t var, uint32_t minVal, uint32_t maxVal)
{
  
  int block = map(var, minVal, maxVal, 0, 20);   // Block represent the current LCD space (modify the map setting to fit your LCD)
  int line = map(var, minVal, maxVal, 0, 8);     // Line represent the theoretical lines that should be printed
  int bar =  map(var, minVal, maxVal, 0, 20*5);  
  bar -= block * 5;                      
                                                  // Bar represent the actual lines that will be printed

  /* LCD Progress Bar Characters, create your custom bars */


 
  for (int x = 0; x < block; x++)                        // Print all the filled blocks
  {
    p_lcd->setCursor (x, row);
    p_lcd->write (1023);
  }
 
  p_lcd->setCursor (block, row);                            // Set the cursor at the current block and print the numbers of line needed
  if (bar != 0) p_lcd->write (bar);
  if (block == 0 && line == 0) p_lcd->write (1022);   // Unless there is nothing to print, in this case show blank
 
  for (int x = 20; x > block; x--)                       // Print all the blank blocks
  {  
   p_lcd->write (1022); 
  }
}
void LCD::writeString(String text)
{
  p_lcd->setCursor(0,0); 
  p_lcd->print(text); // vypíše text

}
void LCD::writeLine(int row, String text)
{
  p_lcd->setCursor(0, row); 
  p_lcd->print(text); // vypíše text

}
void LCD::writeLineFrom(int row, int index, String text)
{
  p_lcd->setCursor(index, row); 
  p_lcd->print(text); // vypíše text

}
void LCD::clearLine(int line)
{
  p_lcd->setCursor(0, line); 
  p_lcd->print("                    "); // vypíše text
}

void LCD::clearAll()
{
  p_lcd->clear();
}

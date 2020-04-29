/*
 * Copyright 2011 Alan Burlison, alan@bleaklow.com.  All rights reserved.
 * Use is subject to license terms.
 */

/*
 * Simple task classes.
 */
#include "Task.h"

// ***
// *** Define the Debugger Class as type Task
// ***
class Debugger : public Task
{
public:
  Debugger();
  void debugWrite(String debugMsg); //Used for simple debugging of other tasks
  virtual void run(uint32_t now);   //Override the run() method
  virtual void update(uint32_t now);   //Override the run() method

  virtual bool canRun(uint32_t now);  //Override the canRun() method
  virtual bool canUpdate(uint32_t now) {return false;};  //Override the canRun() method

};

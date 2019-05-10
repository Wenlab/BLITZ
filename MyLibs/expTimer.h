/*
* Copyright 2019 Wenbin Yang <bysin7@gmail.com> (This project started from Jan., 2018.)
* This file is part of [BLITZ (Behavioral Learning In The Zebrafish)](https://github.com/Wenlab/BLITZ),
* which is adapted from MindControl (Andrew Leifer et al., 2011) <leifer@fas.harvard.edu>
*
* BLITZ is a free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the license, or
* (at your option) any later version.
*
* Filename: expTimer.h
* Abstract: this file contains all function declarations that
*			used to count time and other chronocial variables for
*     experiments
*
* Current Version: 3.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Apr. 19, 2019
* Modified on: Apr. 20, 2019
*/

// Include 3rd party libraries from Song Ho Ahn, check the file for the license
#include "../3rdPartyLibs/Utilities/Timer.h"

/* Class to cout seconds, idxFrame, experimet phase and any other chronocial variables */
class ExpTimer : public Timer
{
private:
  ;// nothing for now
public:
  ExpTimer()
  {
    count = -1;
    expPhase = -1;
    sElapsed = -1;
    msRemElapsed = -1;
  }
  /* Inherited the following methods:
  void   start();                             // start timer
	void   stop();                              // stop the timer
	double getElapsedTime();                    // get elapsed time in second
	double getElapsedTimeInSec();               // get elapsed time in second (same as getElapsedTime)
	double getElapsedTimeInMilliSec();          // get elapsed time in milli-second
	double getElapsedTimeInMicroSec();          // get elapsed time in micro-second
  */

  /* Get both sElapsed and msRemElapsed */
  void getTime();
  // Properties

  int count; // count events
  int expPhase; // indicate which experiment phase this frame is
  int sElapsed; // seconds that elapsed
  int msRemElapsed; // remianed mili-seconds that elapsed

};

// Global functions


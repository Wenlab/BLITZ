/*
* Copyright 2019 Wenbin Yang <bysin7@gmail.com>
* This file is part of BLITZ (Behavioral Learning In The Zebrafish),
* which is adapted from MindControl (Andrew Leifer et al <leifer@fas.harvard.edu>
* Leifer, A.M., Fang-Yen, C., Gershow, M., Alkema, M., and Samuel A. D.T.,
* 	"Optogenetic manipulation of neural activity with high spatial resolution in
*	freely moving Caenorhabditis elegans," Nature Methods, Submitted (2010).
*
* BLITZ is a free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, either version 3 of the license, or
* (at your option) any later version.
*
* Filename: expTimer.h
* Abstract: this file contains all function definitions
*			used to count time and other chronocial variables for
*     experiments
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Apr. 19, 2019


*/

// Include 3rd party libraries
#include "../3rdPartyLibs/Utilities/Timer.h"


class ExpTimer : public Timer
{
private:
  ;// nothing for now
public:
  ExpTimer()
  {

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

  int idxFrame;
  int expPhase; // indicate which experiment phase this frame is
  int sElapsed; // seconds that elapsed
  int msRemElapsed; // remianed mili-seconds that elapsed

}
// Global functions
/* Get the end index of frames from the end time, start from 0 */
int getIdxFrame(int frameRate, int endTime, int idxStart=0);

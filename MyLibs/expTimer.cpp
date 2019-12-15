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
* Filename: expTimer.cpp
* Abstract: this file contains all function implementations
*			used to count time and other chronocial variables for
*     experiments
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Apr. 19, 2019
*/
#include "expTimer.h"

void ExpTimer::getTime()
{
	sElapsed = getElapsedTimeInSec();
	msRemElapsed = (int)getElapsedTimeInMilliSec() % 1000;
}


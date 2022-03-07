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
* Filename: experiment.h
* Abstract: this file contains all classes and function declarations
*		used in constructing final behavioral learning experiment in zebrafish
*
* Current Version: 3.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 15, 2018
* Modified on: Apr. 20, 2019
*/

#ifndef _GUARD_EXPERIMENT_H
#define _GUARD_EXPERIMENT_H



// Include user-defined libraries
#include "userInterface.h"
#include "expTimer.h"
#include "talk2screen_cv.h"
#include "talk2relay.h"
#include "fileWriter.h"
#include "fishAnalysis.h"
#include "TCP-server.h"
#include "talk2FCamera.h"


// User-defined macros
#define COM_NUM 3
#define blackPatch 2
#define fullPatch 3
class Experiment
{
public:
	// methods
	Experiment()
	{
		// empty forever
	}
	/* Initialize the experiment */
	void initializeTest_2();
	void initializeTest_3();
	/* Run the entire operant learning procedure */

	void runOLexptest_2();
	void runOLexptest_3();



	void runAntiOLexp();
	/* Run unpaired training in the operant learning procedure */


private:
	// properties
	int experimentStage;//0=baseline;1=train;2=test
	  // Functional module objects
	UserInterface UIobj; // object that receive user inputs via the command line
	ExpTimer timerObj; // object that count time and index of frames
	TCP_server TCPobj;
	Fish fishObj;
	Screen screenObj; // object that shows pattern via a projector
	Screen screenObj2;
	string window1 = "test";
	string window2 = "test2";

	Relay relayObj; // object that controls a 16-channel relay
	FileWriter fileWriterObj;// object that save and write out data
	FCamera fCameraObj;
};



#endif _GUARD_EXPERIMENT_H


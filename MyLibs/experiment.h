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
#include "talk2camera.h"
#include "fishAnalysis.h"
#include "talk2screen.h"
#include "talk2relay.h"
#include "fileWriter.h"




// User-defined macros
#define COM_NUM 4


class Experiment
{
public:
	// methods
	Experiment()
	{
		// empty forever
	}
	/* Initialize the experiment */
	void initialize(); // TODO: make every module can be enabled separately via adding openStatus boolean array

	/* Run the entire operant learning procedure */
	void runOLexp(); // TODO: rewrite the implementation with new methods. Be sure to align the abstraction level.

	/* Run unpaired training in the operant learning procedure */
	void runUnpairedOLexp(); // TODO: rewrite the implementation with new methods. Be sure to align the abstraction level.

	/* Run the experiment to do whether fish invisible to the blue pattern */
	void runBlueTest(); // TODO: rewrite the implementation with new methods. Be sure to align the abstraction level.

private:
  // properties
	// Functional module objects
	UserInterface UIobj; // object that receive user inputs via the command line
	ExpTimer timerObj; // object that count time and index of frames
	MultiUSBCameras camerasObj; //object that captures images from camera, TODO: use generic function to indicate different device?
	FishAnalysis fishAnalysisObj; // object that processes images
	Screen screenObj; // object that shows pattern via a projector
	Relay relayObj; // object that controls a 16-channel relay
	FileWriter fileWriterObj;

};



#endif _GUARD_EXPERIMENT_H

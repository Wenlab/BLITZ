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

#ifndef _GUARD_EXPERIMENT_CV_H
#define _GUARD_EXPERIMENT_CV_H



// Include user-defined libraries
#include "userInterface.h"
#include "expTimer.h"
#include "talk2camera.h"
#include "fishAnalysis.h"
#include "talk2screen_cv.h"
#include "talk2relay.h"
#include "fileWriter.h"




// User-defined macros
#define COM_NUM 5
#define blackPatch 2

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


	void runAntiOLexp();
	/* Run unpaired training in the operant learning procedure */
	void runUnpairedOLexp(); // TODO: rewrite the implementation with new methods. Be sure to align the abstraction level.

	/* */
	std::vector<bool> RNGsampleFrames2giveShock(
		int numShocks, // number of shocks to give
		int sampleDuration, // the entire time that to sample from
		int startTime = 0 // start from when
	);
	void test1();
	void test2();
	void videoclip();
	void ShadowDetection();
private:
	// properties
	  // Functional module objects
	UserInterface UIobj; // object that receive user inputs via the command line
	ExpTimer timerObj; // object that count time and index of frames
	SingleCamera cameraObj; //object that captures images from camera, TODO: use generic function to indicate different device?
	Arena ArenaObj; // object that processes images
	Screen screenObj; // object that shows pattern via a projector
	Relay relayObj; // object that controls a 16-channel relay
	FileWriter fileWriterObj;// object that save and write out data

};



#endif _GUARD_EXPERIMENT_CV_H

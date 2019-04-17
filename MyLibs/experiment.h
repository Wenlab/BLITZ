  /*
* Copyright 2018 Wenbin Yang <bysin7@gmail.com>
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
* Filename: experiment.h
* Abstract: this file contains all classes and function declarations
*		used in constructing final behavioral learning experiment in zebrafish
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: Apr. 28, 2018

* Replaced Version: 1.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
*/

#ifndef _GUARD_EXPERIMENT_H
#define _GUARD_EXPERIMENT_H
// Include 3rd party libraries
#include "../3rdPartyLibs/Utilities/Timer.h"
// Include user-defined libraries
#include "fishAnalysis.h"
#include "talk2camera.h"
#include "talk2relay.h"
#include "talk2screen.h"
#include "fileWriter.h"
#include "userInterface.h"

// User-defined macros

// TODO: make it can deal with the situations that width does not eqaul height
#define WIDTH 784 // frame width for all cameras and video files
#define HEIGHT 784 // frame height for all cameras and video files
#define FRAMERATE 10 // frame rate for all cameras to capture videos

// TODO: rename this class, drop "Data"
class Experiment
{
private:
	;
public:
	// methods
	ExperimentData(std::string pName)
		:pathName(pName)
	{
		numCameras = 0;
		idxFrame = -1;
		sElapsed = -1; // TODO: do we need this, if we already have idxFrame?
		msRemElapsed = 0; // TODO: useful for calibrations and plotting, but requires better insulation
		expPhase = -1;
	}
	/* Initialize the experiment */
	bool initialize(); // TODO: make every module can be enabled separately
	// TODO: add openStatus boolean array

	/* run a specific experiment */
	void runXXexp();

	/* Run unpaired training in the operant learning procedure */
	void runUnpairedOLexp();

	/* Run the entire operant learning procedure */
	void runOLexp();

	/* Run the experiment to do whether fish invisible to the blue pattern */
	void runBlueTest();

	/* Write out info of a frame to disk */
	void writeOutFrame(); // TODO: consider to move this method to fileWriter class

	/* Get current time */
	bool getTime(); //TODO: wrap timer class in a new class and move this method to that new class.

	/* Get experiment start local time */
	void get_current_date_time(); // TODO: consider to move this method

  // properties

	// constant ones
	std::vector<std::string> CSpatterns; // TODO: move this into the WritOut module
	const std::string pathName; // TODO: move this into the WritOut module
	int numCameras; // TODO: change this into openStatus array which allows more flexible options for users

	int idxFrame;
	int sElapsed;
	int msRemElapsed;
	int expPhase;




	// Functional module objects
	Timer timerObj; // TODO: consider to add a new class to wrap the 3rd party lib?
	Cameras camerasObj; //TODO: CameraData -> Cameras;
	FishAnalysis fishAnalysisObj;
	Screen screenObj; // TODO: ScreenData -> Screen;
	Relay relayObj; // TODO: update the lines that use this object.
	FileWriter fileWriterObj;
	UserInterface UIobj;
};


#endif _GUARD_EXPERIMENT_H

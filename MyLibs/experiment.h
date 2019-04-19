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

// TODO: rename this class, drop "Data"
class Experiment
{
private:
	;
public:
	// methods
	ExperimentData()
	{

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

  // properties

	// Functional module objects
	ExpTimer timerObj; // TODO: consider to add a new class to wrap the 3rd party lib?
	Cameras camerasObj; //TODO: CameraData -> Cameras;
	FishAnalysis fishAnalysisObj;
	Screen screenObj; // TODO: ScreenData -> Screen;
	Relay relayObj; // TODO: update the lines that use this object.
	FileWriter fileWriterObj;
	UserInterface UIobj;
};


#endif _GUARD_EXPERIMENT_H

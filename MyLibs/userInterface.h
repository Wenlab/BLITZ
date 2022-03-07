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
* Filename: userInterface.h
* Abstract: this file contains all function definitions
*			used to communicate with users through an command line interface
*
* Current Version: 3.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 15, 2018
* Modified on: Apr. 20, 2019
*/

#ifndef USERINTERFACE_H_DEF
#define USERINTERFACE_H_DEF

// Include standard libraries
#include <iostream>
#include <vector>
#include <sstream>
#include <string>
#include <ctime> // to get the current date and time

#include "errorHandling.h"
// TODO: make a GUI, alternative1: GMU readline command line interface
/* Talk to users via command line interface */
class UserInterface
{
private:
	int numDevices;
	; //nothing for now
public:
	UserInterface()
	{

	}

	/* Ask the user about the experiment infos */
	void enquireInfoFromUser();

	/* Ask for what strain of Fish is using,
		 assume all fish are the same strain */
	void enquireFishStrain(std::istream& is);



	/* Ask for the age for all fish,
		assume all fish are at the same age */
	void enquireFishAge(std::istream& is);


	/* Ask for what experiment task for fish */
	void enquireExpTask(std::istream& is);


	/* Generate basenames for all output files */
	void generateBasenames();


	/* Generate basenames for the output files */
	std::string generateBasename(int idxFile);


	// Properties
	// TODO: get the number of cameras and pass it to fileWriterObj
	//std::vector<bool> devices2use; // status array to indicate which devices to use
	// 1. relay, 2. projector, 3. cameras
	//std::vector<bool> cameras2open; // status array to indicate which cameras to open
	//std::vector<std::vector<int>> allFishIDs; // fish IDs for all arenas
	int fishAge; // one age for all fish
	std::string strainName; //
	//std::vector<int> arenaIDs; // which arena the fish is in
	std::string expTask; // task to run for animals
	//std::string visPattern; // one pattern for all
	std::string startTimeStr; // string of the start time of an experiment, YYYYMMDD-HHMM
	std::string baseNames;
	//int numOpenCameras;

};


// Global functions
/* Show software description and welcome messages to user */
void showWelcomeMsg();

/* Show the diagram of fish */
//void showFishPosDiagram();

/* Get string vector from command line */
std::vector<std::string> getStrVecFromCMD(std::istream& is);

//TODO: write a better description and note the dependencies
/* Get current date and time string from chrono system clock,
	 depends on <ctime> */
std::string getCurDateTime();


#endif // USERINTERFACE_H_DEF


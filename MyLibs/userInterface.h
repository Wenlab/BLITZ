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
* Filename: userInterface.h
* Abstract: this file contains all function definitions
*			used to communicate with users through an command line interface
*
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>

* Created on: Apr. 15, 2018
*/

#ifndef USERINTERFACE_H_DEF
#define USERINTERFACE_H_DEF

// Include standard libraries
#include <ctime> // to get the current date and time


#define MAX_CAMERAS 3




class UserInterface
{
private:
	; //nothing for now
public:
	UserInterface()
	{
		numOpenCameras = 0;
		cameras2open(MAX_CAMERAS);
	}

	/* Ask the user about the experiment infos */
	void enquireInfoFromUser();

	/* Ask which cameras to use. */
	void enquireCameras2use();

	/* Ask how many fish under a camera */
	int enquireNumFishForACam(int idxCamera);

	/* Generate fish IDs for all fish */
	std::vector<int> generateFishIDs(int numFish);

	/* Ask for what strain of Fish is using,
	 	 assume all fish are the same strain */
	void enquireFishStrain();

	/* Ask for the age for all fish,
	 	assume all fish are at the same age */
	void enquireFishAge();

	/* Ask for what experiment task for poor fish */
	void enquireExpTask();



	/* Get basename for the output files */
	std::string getBasename(int idxFile);


	// Properties
	// TODO: get the number of cameras and pass it to fileWriterObj
	std::vector<bool> cameras2open; // status array to indicate which cameras to open
	// TODO: consider to generate fishIDs automatically, combine some time info
	std::vector<std::vector<std::string>> fishIDs; // fish IDs for all arenas
	int fishAge; // one age for all fish
	std::string strainName; // TODO: consider to have a single strain name
	std::vector<int> arenaIDs; // which arena the fish is in
	std::string expTask; // task to run for animals
	std::vector<std::string> CSstrs; // CS patterns
	std::string startTimeStr; // string of the start time of an experiment, YYYYMMDD-HHMM
	int numOpenCameras;

}


// Global functions
/* Show software description and welcome messages to user */
void showWelcomeMsg();

/* Show the diagram of fish */
void showFishPosDiagram();

/* Get string vector from command line */
std::vector<std::string> getStrVecFromCMD();

//TODO: write a better description and note the dependencies
/* Get current date and time string from chrono system clock */
string getCurDateTime();







#endif // USERINTERFACE_H_DEF

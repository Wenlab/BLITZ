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

class UserInterface
{
	// TODO: consider to remove the following variables
	int fishAge; // one age for all fish
	std::string expTask;
	std::vector<std::vector<std::string>> fishIDs; // fish IDs for all arenas

	// derivated info, such as the strain name
	std::vector<std::string> strainNames;
	std::vector<std::string> baseNames; // filenames without extensions
	std::string timeStr;
	std::vector<std::string> CSstrs;

  /* Get CSpatterns from the basenames */
	std::vector<std::string> get_CS_patterns(std::vector<std::string> CS_strs); // TODO: consider to move this into userInterface

	/* Ask the user about the experiment infos */
	int enquireInfoFromUser();

	/* Ask the number of cameras to use. */
	void enquireNumCams();

	/* Ask for what strain of Fish is using */
	void enquireStrainNames();

	/* Ask for the age for all fish */
	void enquireFishAge();

	/* Ask for what experiment task for poor fish */
	void enquireExpTask();

	/* Ask for fish IDs for all arenas */
	void enquireFishIDs();

	/* Ask for fish IDs in the arena */
	std::vector<std::string> enquireFishIDs(int arenaIdx);

	/* Get strain names of fish in all arenas */
	void getStrainNames();

	/* Get strain name of fish in the arena
	   fishIDs are IDs of fish in one arena */
	std::string getStrainName(std::vector<std::string> fishIDs);

	/* Get basenames for all output files */
	void getBasenames();

	/* Get basename for the output files */
	std::string getBasename(int arenaIdx);

	/* Get all basenames from user */
	void enquireAllBasenames();

	/* Get basename in one area */
	std::string enquireBasename(int areaIdx);

	/* Get CS strings for all arena */
	void get_CS_strings(std::vector<const char*>);

	/* Get the CS string for the arena */
	std::string get_CS_string(const char*);

	/* Show software description and welcome messages to user */
	void showWelcomeMsg();

	/* Show the diagram of fish */
	void showFishPosDiagram();

	/* Extract the pattern name from the filename */
	std::string extractPatternName(const char*);

}














#endif // USERINTERFACE_H_DEF

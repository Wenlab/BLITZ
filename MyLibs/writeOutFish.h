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
* Filename: writeOutFish.h
* Abstract: this file contains all classes and functions' declarations
*			used to write experiment info and extracted motion parameters
*			into yaml files and save videos for each camera
*
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: Apr. 28, 2018

* Replaced Version: 1.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
*/


#ifndef _GUARD_WRITEOUTFISH_H
#define _GUARD_WRITEOUTFISH_H

// Include OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>

// Include standard libraries
#include <iostream>


class WriteOutData
{
private:
	;// nothing for now
public:
	// methods
	WriteOutData()
	{

	}
	bool initialize(std::string contentName, int width, int height, int frameRate);
	
	
	/* Write out the context information for the experiment, only once */
	void writeExpContext(int numFiles);


	// template functions
	/* Write out key value pairs */
	template <typename T>
	void writeKeyValuePair(std::string vName, T var, int idxFile)
	{
		yamlVec[idxFile] << vName << var;
	}
	/* Write out key-value pair in a line*/
	template <typename T>
	void writeKeyValueInline(std::string vName, T var, int idxFile)
	{
		yamlVec[idxFile] << "{:" << vName << var << "}";
	};

	
	// properties
 

	std::string expTask;


	std::vector<cv::FileStorage> yamlVec;
	std::vector<cv::VideoWriter> videoVec;
};

/* Show software description and welcome messages to user */
void showWelcomeMsg();

/* Get strain name of fish */
std::string get_strainName(char firstChar);

/* Get CS string to append to the filenames of yaml and video files */
std::string get_CS_string(std::string CSpattern);

/* Get experiment start local time */
std::string get_current_date_time();

int enquireNumCams();

int enquireFishAge();
std::string enquireExpTask();

/* Ask for fish IDs in the arena */
std::vector<std::string> enquireFishIDs(int arenaIdx);

/* convert string vector to int-vector like formatted output */
std::string strVec2str(std::vector<std::string> strVec);


/* Case insensitive comparasion 
Adapted from Timmmm, https://stackoverflow.com/a/4119881
*/
bool iequals(const std::string& a, const std::string& b);

#endif // !_GUARD_WRITEOUTFISH_H


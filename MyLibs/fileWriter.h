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

// TODO: redefine this class, think it clear the relationship between
// this WriteOut class and the ExperimentData class
// TODO: unify the input from one place (file, script or command window)
// TODO: create a yaml config file and corresponding functions
// TODO: rewrite the WriteOutData class as the FileIO class
// I have to set the level of variables that users can access


#ifndef _GUARD_WRITEOUTFISH_H
#define _GUARD_WRITEOUTFISH_H

// Include user-defined libraries
#include "errorHandling.h"
#include "fishAnalysis.h"
#include "talk2camera.h"
#include "userInterface.h"

// Include OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>

// Include standard libraries
#include <iostream>

/* TODO: include fishAnalysis and another class (userInterface?) for experiment settings
 then pass the header of the objects to function
 #include "fishAnalysis.h"
 #include "userInterface.h"
*/

/* All information to write to files
## Write once
- FishIDs: vector<string>, from userInterface
- FishAge: int, from userInterface
- FishStrain: string, from userInterface
- Arena: int, auto-generate from userInterface
- Task: string, from userInterface
- CSpattern: string, from userInterface
- ExpStartTime: string, auto-generate from system time
- FrameRate: int, from cameras class
- FrameSize: vector<int>? Point(x,y)?, from cameras class
- ImgSeg: vector<int>, from fishAnalysis class
- yDivide: vector<int>, from fishAnalysis class


## Write every frame
- FrameNum: int, from experiment, encapsulate in new ExpTimer class
- ExpPhase: int, from experiment, encapsulate in new ExpTimer class
- sElapsed: int, from timer, encapsulate in new ExpTimer class
- msRemElapsed: int, from timer
- FishIdx: int, from fishAnalysis class
- Head: cv::Point
- Tail: cv::Point
- Center: cv::Point
- HeadingAngle: int
- ShockOn: bool
- idxCase: int
- ... another fish's data



*/


class FileWriter
{
private:
	;// nothing for now
public:
	// methods
	FileWriter()
	{
		path2save = "F:/FishExpData/";
	}
	// methods

	/* initialize yaml- and video- writers */
	void initialize(UserInterface& UIobj); // object that contains user's input


	/* Write out experiment settings as the header for files
		only write once */
	void writeOutExpSettings(
		UserInterface& UIobj, // object that contains user's input
		Cameras& camerasObj, // object that contains cameras' info (e.g., FRAMERATE)
		FishAnalysis& fishAnalysisObj // object that contains image processing info
		)

	/* Write out info that updated every frame to files (YAMLs and videos) */
	void writeOutFrame(
		ExpTimer& timerObj, // object that counts time and index of frames
		FishAnalysis& fishAnalysisObj, // object that contains image processing info
		int idxFile // which file to write
		)

	
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
	}

	// properties
	std::vector<cv::FileStorage> yamlVec;
	std::vector<cv::VideoWriter> videoVec;

	// user input infos
	int numFiles;
	string timeStr;

	std::vector<std::string> CSpatterns;
	const std::string path2save;

};


/* convert string vector to int-vector like formatted output */
std::string strVec2str(std::vector<std::string> strVec);

/* Case insensitive comparasion
Adapted from Timmmm, https://stackoverflow.com/a/4119881
*/
bool iequals(const std::string& a, const std::string& b);

#endif // !_GUARD_WRITEOUTFISH_H

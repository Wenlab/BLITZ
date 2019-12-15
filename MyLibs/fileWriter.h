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
* Filename: fileWriter.h
* Abstract: this file contains all function declarations
*			used to write experiment info and extracted motion parameters
*			into yaml files and save videos for each camera
*
*
* Current Version: 3.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 15, 2018
* Modified on: Apr. 20, 2019
*/

#ifndef _FILEWRITER_H_DEF
#define _FILEWRITER_H_DEF

// Include user-defined libraries

#include "fishAnalysis.h"
#include "prepare.h"
#include "expTimer.h"
#include "talk2camera.h"


// Include OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>

// Include standard libraries
#include <iostream>
#include <vector>


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

/* Class to write out YAMLs and videos with the basic OpenCV methods */
class FileWriter
{
private: // TODO: set the level of variables that users can access
	;// nothing for now
public:
	// methods
	FileWriter()
	{
		path2save = "F:/FishExpData/";
	}
	// methods


	/* initialize a single OpenCV video-writer */
	void initialize(
		std::string baseName,
		cv::Size frameSize, // size of frames, set parameters for writing videos
		float frameRate=140
	);

	/* Write out experiment settings as the header for files
		only write once */
	void writeOutExpSettings(
		Prepare& prepareObj,
		Camera& camerasObj, // object that contains cameras' info (e.g., FRAMERATE)
		FixedFish& fishAnalysisObj // object that contains image processing info
	);

	/* Write out info that updated every frame to files (YAMLs and videos) */
	void writeOutFrame(
		ExpTimer& timerObj, // object that counts time and index of frames
		FixedFish& fishAnalysisObj // object that contains image processing info
		
	);

	void writeOutBout(
		ExpTimer& timerObj,
		FixedFish& FixedFishObj
		);

	// template functions
	/* Write out key value pairs */
	template <typename T>
	void writeKeyValuePair(std::string vName, T var)
	{
		yamlVec << vName << var;
	}
	/* Write out key-value pair in a line*/
	template <typename T>
	void writeKeyValueInline(std::string vName, T var)
	{
		yamlVec << "{:" << vName << var << "}";
	}

	// properties
	// TODO: consider to convert these variables to private
	cv::FileStorage yamlVec;
	cv::VideoWriter videoVec;

	// user input infos
	std::string path2save;
	int numFiles;
	std::string timeStr;

};

// Global functions
/* convert string vector to int-vector like formatted output */
std::string strVec2str(std::vector<std::string> strVec);

/* convert int vector to int-vector like formatted output */
std::string intVec2str(std::vector<int> intVec);


#endif // !_GUARD_FILEWRITER_H


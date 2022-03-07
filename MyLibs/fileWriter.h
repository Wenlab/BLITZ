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
#include "errorHandling.h"
#include "fishAnalysis.h"
#include "userInterface.h"
#include "expTimer.h"
#include "Talk2FCamera.h"


// Include OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>

// Include standard libraries
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include <mat.h>


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




class fishData {
public:
	fishData(int size, int FishID) :
		arraySize(size),
		FishIdx(FishID)
	{
		HeadX = new int[arraySize];
		totalFishData.push_back(HeadX);
		HeadY = new int[arraySize];
		totalFishData.push_back(HeadY);
		ShockOn = new int[arraySize];
		totalFishData.push_back(ShockOn);
		PatternIdx = new int[arraySize];
		totalFishData.push_back(PatternIdx);
		ExcitationIdx = new int[arraySize];
		totalFishData.push_back(ExcitationIdx);
	}

	int FishIdx;
	int* HeadX;
	int* HeadY;
	int* ShockOn;
	int* PatternIdx;
	int* ExcitationIdx;
	int arraySize;
	std::vector<int*> totalFishData;
};

/*
Store all the data and write it out as .mat file at the end of the experiment
*/

class saveData {
public:
	saveData()
	{
		path2save = "F:/FishExpData/";
	}

	void initialize(int totalTrialTime, int totalTrials, int frameRate, std::string baseName);

	void saveMat(std::string baseName);
	void test_ini(int arraySize, std::string baseName);
	void test_frame(ExpTimer& timerObj, FCamera& FcameraObj,
		int FrameNum);
	void test_save(std::string baseName);


	int* HeadX;
	int* HeadY;
	int* HeadingAngle;
	int* Sum_curv;


	int arraySize;
	int trialSize;
	int numFish;
	std::string path2save;
	int* FrameNum;
	int* sElapsed;
	int* msRemElapsed;
	int* trialCount;
	int* experimentStage;
	int* totalData;
	int** Fcamera;
	std::vector<fishData> allFishData;
	std::vector<cv::VideoWriter> videoVec;
};
// Global functions
/* convert string vector to int-vector like formatted output */
std::string strVec2str(std::vector<std::string> strVec);

/* convert int vector to int-vector like formatted output */
std::string intVec2str(std::vector<int> intVec);


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

	void writeOutExpSettings(UserInterface& UIobj);


	/* initialize a saveData class */
	void initialize(
		std::string baseName, int totalTrialTime, int totalTrials, int frameRate
	);


	void saveFrame(ExpTimer& timerObj, Fish& fishObj, FCamera& FcameraObj,
		int idxFile, int trialCount, int experimentStage);

	void writeOutData(std::string baseNames);
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
	// TODO: consider to convert these variables to private
	std::vector<cv::FileStorage> yamlVec;
	std::vector<cv::VideoWriter> videoVec;
	std::vector<saveData> saveDataVec;
	// user input infos
	std::string path2save;
	int numFiles;
	std::string timeStr;
	//saveData saveDataObj;
};




#endif // !_GUARD_FILEWRITER_H

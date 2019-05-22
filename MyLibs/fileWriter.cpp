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
* Filename: fileWriter.h
* Abstract: this file contains all function definitions
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

// Include user-defined libraries
#include "fileWriter.h"

// Include standard libraries

#include <algorithm>  // include the algorithm reverse


using namespace std;
using namespace cv;

void FileWriter::initialize(vector<string> baseNames, Size frameSize, float frameRate = 10)
{
	numFiles = baseNames.size();
	for (string& baseName : baseNames)
	{
		initialize(baseName);
		initialize(baseName, frameSize, frameRate);
	}

}

void FileWriter::initialize(string baseName)
{
	string yamlName = path2save + baseName + ".yaml";
	FileStorage fObj(yamlName, FileStorage::WRITE);
	tryCatchFalse(fObj.isOpened(), "YAML file writer CANNOT be opened!");

	yamlVec.push_back(fObj);
	
}

void FileWriter::initialize(string baseName, Size frameSize, float frameRate = 10)
{
	string videoName = path2save + baseName + ".yaml";
	VideoWriter vObj(videoName, CV_FOURCC('D', 'I', 'V', 'X'), frameRate, frameSize, false);
	tryCatchFalse(vObj.isOpened(), "Video file writer CANNOT be opened!");

	videoVec.push_back(vObj);

}

void FileWriter::writeOutExpSettings(UserInterface& UIobj, MultiUSBCameras& camerasObj,
	FishAnalysis& fishAnalysisObj)
{
	for (int i = 0; i < numFiles; i++)
	{
		writeKeyValuePair("FishIDs", intVec2str(UIobj.allFishIDs[i]), i);
		writeKeyValuePair("FishAge", UIobj.fishAge, i);
		writeKeyValuePair("FishStrain", UIobj.strainName, i);
		writeKeyValuePair("Arena", i + 1, i); // record which arena is in use
		writeKeyValuePair("Task", UIobj.expTask, i);
		writeKeyValuePair("CSpattern", UIobj.visPattern, i);
		writeKeyValuePair("ExpStartTime", timeStr, i);
		writeKeyValuePair("FrameRate", camerasObj.frameRate, i);// TODO: is this cross-file used macro a good practice?
		writeKeyValuePair("FrameSize", Size(camerasObj.frameWidth, camerasObj.frameHeight), i);
		// TODO: decide where to save x_, y_cut? FishAnalysis or Arena?
		//writeKeyValuePair("ImgSeg", Size(fishAnalysisObj.x_cut,fishAnalysisObj.y_cut), i);
		//writeKeyValuePair("yDivide", fishAnalysisObj.yDivs[i], i);
	}

}

void FileWriter::writeOutFrame(ExpTimer& timerObj, FishAnalysis& fishAnalysisObj,
		int idxFile)
{
	// TODO: align the abstraction level
	videoVec[idxFile] << fishAnalysisObj.allArenas[idxFile].opencvImg; // write image to disk

	// write the custom class to disk
	yamlVec[idxFile] << "Frames" << "[";
	// Python-like inline compact form
	// general experimental info
	writeKeyValueInline("FrameNum", timerObj.count / numFiles, idxFile);
	writeKeyValueInline("ExpPhase", timerObj.expPhase, idxFile);
	writeKeyValueInline("sElapsed", timerObj.sElapsed, idxFile);
	writeKeyValueInline("msRemElapsed", timerObj.msRemElapsed, idxFile);

	// specific fish analysis data
	vector<Fish> allFish = fishAnalysisObj.allArenas[idxFile].allFish;
	for (int i = 0; i < allFish.size(); i++)
	{
		writeKeyValueInline("FishIdx", i, idxFile);
		writeKeyValueInline("Head", allFish[i].head, idxFile);
		writeKeyValueInline("Tail", allFish[i].tail, idxFile);
		writeKeyValueInline("Center", allFish[i].center, idxFile);
		writeKeyValueInline("HeadingAngle", allFish[i].headingAngle, idxFile);
		writeKeyValueInline("ShockOn", allFish[i].shockOn, idxFile);
		writeKeyValueInline("PatternIdx", allFish[i].idxCase, idxFile);
	}
	yamlVec[idxFile] << "]";
}

string strVec2str(vector<string> strVec)
{
	if (strVec.empty()) // vector is empty, return empty string
		return "";

	string str = strVec[0];
	for (int j = 1; j < strVec.size(); j++)
		str += "," + strVec[j];
	
	return str;
}

string intVec2str(vector<int> intVec)
{
	if (intVec.empty()) // vector is empty, return empty string
		return "";

	string str = "";
	for (int& i : intVec)
		str += to_string(i) + ",";
	
	str.erase(str.end()-1,str.end()); // remove the last charactor
	return str;
}



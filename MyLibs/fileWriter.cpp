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
#include <ctime> // to get the current date and time
#include <algorithm>  // include the algorithm reverse


using namespace std;
using namespace cv;

void FileWriter::initialize(UserInterface& UIobj)
{
	for (int i = 0; i < numFiles; i++)
	{
		string basename = UIobj.getBasename(i);
		string videoName = path2save + basename + ".avi";
		string yamlName = path2save + basename + ".yaml";

		FileStorage fObj(yamlName, FileStorage::WRITE);
		try {
			tryCatchFalse(fObj.isOpened(), "YAML file writer CANNOT be opened!");
		} catch (string errorMsg) {
			cout << errorMsg << endl;
			waitUserInput2exit();
		}
		yamlVec.push_back(fObj);


		VideoWriter vObj(videoName, CV_FOURCC('D', 'I', 'V', 'X'), frameRate, Size(width, height), false);
		try {
			tryCatchFalse(vObj.isOpened(), "Video file writer CANNOT be opened!");
		} catch (string errorMsg) {
			cout << errorMsg << endl;
			waitUserInput2exit();
		}
		videoVec.push_back(vObj);

	}

}

void FileWriter::writeOutExpSettings(UserInterface& UIobj, Cameras& camerasObj,
	FishAnalysis& fishAnalysisObj)
{
	for (int i = 0; i < numFiles; i++)
	{
		writeKeyValuePair("FishIDs", strVec2str(UIobj.fishIDs[i]), i);
		writeKeyValuePair("FishAge", UIobj.fishAge, i);
		writeKeyValuePair("FishStrain", UIobj.strainNames[i], i);
		writeKeyValuePair("Arena", i + 1, i); // record which arena is in use
		writeKeyValuePair("Task", UIobj.expTask, i);
		writeKeyValuePair("CSpattern", UIobj.CSstrs[i], i);
		writeKeyValuePair("ExpStartTime", timeStr, i);
		writeKeyValuePair("FrameRate", FRAMERATE, i);// TODO: is this cross-file used macro a good practice?
		writeKeyValuePair("FrameSize", Size(FRAMEWIDTH, FRAMEHEIGHT), i);
		writeKeyValuePair("ImgSeg", Size(fishAnalysisObj.x_cut,fishAnalysisObj.y_cut), i);
		writeKeyValuePair("yDivide", fishAnalysisObj.yDivs[i], i);
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
	writeKeyValueInline("FrameNum", timerObj.idxFrame / numFiles, cIdx);
	writeOut.writeKeyValueInline("ExpPhase", timerObj.expPhase, cIdx);
	writeOut.writeKeyValueInline("sElapsed", timerObj.sElapsed, cIdx);
	writeOut.writeKeyValueInline("msRemElapsed", timerObj.msRemElapsed, cIdx);

	// specific fish analysis data
	vector<FishData> allFish = fishAnalysisObj.allArenas[cIdx].allFish;
	for (int i = 0; i < allFish.size(); i++)
	{
		writeOut.writeKeyValueInline("FishIdx", i, cIdx);
		writeOut.writeKeyValueInline("Head", allFish[i].head, cIdx);
		writeOut.writeKeyValueInline("Tail", allFish[i].tail, cIdx);
		writeOut.writeKeyValueInline("Center", allFish[i].center, cIdx);
		writeOut.writeKeyValueInline("HeadingAngle", allFish[i].headingAngle, cIdx);
		writeOut.writeKeyValueInline("ShockOn", allFish[i].shockOn, cIdx);
		writeOut.writeKeyValueInline("PatternIdx", allFish[i].idxCase, cIdx);
	}
	writeOut.yamlVec[cIdx] << "]";
}

string strVec2str(vector<string> strVec)
{
	if (strVec.empty()) // vector is empty, return empty string
		return "";

	string str = strVec[0];
	for (int j = 1; j < strVec.size(); j++)
	{
		str += "," + strVec[j];
	}
	//str += "]";
	return str;
}

bool iequals(const string& a, const string& b)
{
	unsigned int sz = a.size();
	if (b.size() != sz)
		return false;
	for (unsigned int i = 0; i < sz; ++i)
		if (tolower(a[i]) != tolower(b[i]))
			return false;
	return true;
}

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

void FileWriter::initialize(vector<string> baseNames, Size frameSize, int frameRate = 10)
{
	numFiles = baseNames.size();
	for (string& baseName : baseNames)
	{
		initialize(baseName);
		initialize(baseName, frameSize, frameRate);
	}
}
void FileWriter::initialize(vector<string> baseNames, Size frameSize, int totalTrialTime, int totalTrials, int frameRate,int numFish)
{
	numFiles = baseNames.size();
	for (string& baseName : baseNames)
	{
		initialize(baseName, totalTrialTime, totalTrials, frameRate, numFish);
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

void FileWriter::initialize(string baseName, int totalTrialTime, int totalTrials, int frameRate,int numFish)
{
	saveData saveDataObj;
	saveDataObj.initialize(totalTrialTime,totalTrials,frameRate,numFish,baseName);

	saveDataVec.push_back(saveDataObj);


}
void FileWriter::initialize(string baseName, Size frameSize, int frameRate = 10)
{
	string videoName = path2save + baseName + ".avi";
	VideoWriter vObj(videoName, CV_FOURCC('D', 'I', 'V', 'X'), frameRate, frameSize, false);
	//vObj.open(videoName, CV_FOURCC('D', 'I', 'V', 'X'), frameRate, frameSize, false);
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
		writeKeyValuePair("ExpStartTime", UIobj.startTimeStr, i);
		writeKeyValuePair("FrameRate", camerasObj.frameRate, i);// TODO: is this cross-file used macro a good practice?
		writeKeyValuePair("FrameSize", Size(camerasObj.frameWidth, camerasObj.frameHeight), i);
		// TODO: decide where to save x_, y_cut? FishAnalysis or Arena?
		//writeKeyValuePair("ImgSeg", Size(fishAnalysisObj.x_cut,fishAnalysisObj.y_cut), i);
		//writeKeyValuePair("yDivide", fishAnalysisObj.yDivs[i], i);
	}

}

void FileWriter::writeOutExpSettings(UserInterface& UIobj, SingleCamera& cameraObj,
	Arena& ArenaObj)
{
	for (int i = 0; i < numFiles; i++)
	{


		writeKeyValuePair("FishIDs", intVec2str(UIobj.allFishIDs[i]), i);
		writeKeyValuePair("FishAge", UIobj.fishAge, i);
		writeKeyValuePair("FishStrain", UIobj.strainName, i);
		writeKeyValuePair("Arena", i + 1, i); // record which arena is in use
		writeKeyValuePair("Task", UIobj.expTask, i);
		writeKeyValuePair("CSpattern", UIobj.visPattern, i);
		writeKeyValuePair("ExpStartTime", UIobj.startTimeStr, i);
		writeKeyValuePair("FrameRate", cameraObj.frameRate, i);// TODO: is this cross-file used macro a good practice?
		writeKeyValuePair("FrameSize", Size(cameraObj.frameWidth, cameraObj.frameHeight), i);
		// TODO: decide where to save x_, y_cut? FishAnalysis or Arena?
		writeKeyValuePair("xCut", ArenaObj.xCut, i);
		writeKeyValuePair("yCut", ArenaObj.yCut, i);
		writeKeyValuePair("xDivide", ArenaObj.allFish[i].xDiv, i);

	}

}
void FileWriter::writeOutExpSettings(UserInterface& UIobj, SingleCamera& cameraObj,
	Arena& ArenaObj, std::vector<std::string> baseNames)
{
	for (int i = 0; i < numFiles; i++)
	{
		std::ofstream outfile;
		string txtName = path2save + baseNames[i] + ".txt";
		outfile.open(txtName);
		outfile << "FishIDs" << ":" << intVec2str(UIobj.allFishIDs[i]) << "\n";
		outfile << "FishAge" << ":" << UIobj.fishAge << "\n";
		outfile << "FishStrain" << ":" << UIobj.strainName << "\n";
		outfile << "Arena" << ":" << i + 1 << "\n";
		outfile << "Task" << ":" << UIobj.expTask << "\n";
		outfile << "CSpattern" << ":" << UIobj.visPattern << "\n";
		outfile << "ExpStartTime" << ":" << UIobj.startTimeStr << "\n";
		outfile << "FrameRate" << ":" << cameraObj.frameRate << "\n";
		outfile << "FrameSize" << ":" << Size(cameraObj.frameWidth, cameraObj.frameHeight) << "\n";
		outfile << "xCut" << ":" << ArenaObj.xCut << "\n";
		outfile << "yCut" << ":" << ArenaObj.yCut << "\n";
		outfile << "xDivide" << ":" << ArenaObj.allFish[i].xDiv << "\n";
		outfile << "FishIDs" << ":" << intVec2str(UIobj.allFishIDs[i]) << "\n";

		outfile.close();
	}

}
void FileWriter::writeOutFrame(Arena& ArenaObj,
	int idxFile) {
	videoVec[idxFile] << ArenaObj.opencvImg;
}

void FileWriter::writeOutFrame(ExpTimer& timerObj, Arena& ArenaObj,
	int idxFile,int trialCount)
{
	// TODO: align the abstraction level

	//cv::Mat gray_Img;
	//cv::cvtColor(ArenaObj.opencvImg, gray_Img, CV_BGR2GRAY);
	//cv::imshow("test2", ArenaObj.opencvImg);
	//cv::waitKey(100);

	videoVec[idxFile] << ArenaObj.opencvImg; // write image to disk

	// write the custom class to disk
	yamlVec[idxFile] << "Frames" << "[";
	// Python-like inline compact form
	// general experimental info
	writeKeyValueInline("FrameNum", timerObj.count / numFiles+ trialCount * 4000,idxFile);
	writeKeyValueInline("ExpPhase", timerObj.expPhase, idxFile);
	writeKeyValueInline("sElapsed", timerObj.sElapsed, idxFile);
	writeKeyValueInline("msRemElapsed", timerObj.msRemElapsed, idxFile);

	// specific fish analysis data
	vector<Fish> allFish = ArenaObj.allFish;
	for (int i = 0; i < allFish.size(); i++)
	{
		writeKeyValueInline("FishIdx", i, idxFile);
		writeKeyValueInline("Head", allFish[i].head, idxFile);
		writeKeyValueInline("Tail", allFish[i].tail, idxFile);
		writeKeyValueInline("Center", allFish[i].center, idxFile);
		writeKeyValueInline("HeadingAngle", allFish[i].headingAngle, idxFile);
		writeKeyValueInline("ShockOn", allFish[i].shockOn, idxFile);
		writeKeyValueInline("PatternIdx", allFish[i].idxCase, idxFile);
		writeKeyValueInline("trialCount", trialCount, idxFile);
	}
	yamlVec[idxFile] << "]";
}
void FileWriter::writeOutData(std::vector<std::string> baseNames) {
	for (int i = 0; i < numFiles; i++) {
		saveDataVec[i].saveMat(baseNames[i]);
	}
}
void FileWriter::saveFrame(ExpTimer& timerObj, Arena& ArenaObj,
	int idxFile, int trialCount) {
	int FrameNum = timerObj.count  + trialCount * saveDataVec[idxFile].trialSize;
	vector<Fish> allFish = ArenaObj.allFish;
	saveDataVec[idxFile].FrameNum[FrameNum] = FrameNum;
	saveDataVec[idxFile].sElapsed[FrameNum] = timerObj.sElapsed;
	saveDataVec[idxFile].msRemElapsed[FrameNum] = timerObj.msRemElapsed;
	saveDataVec[idxFile].trialCount[FrameNum] = trialCount;
	for (int i=0; i < saveDataVec[idxFile].numFish; i++) {
		saveDataVec[idxFile].allFishData[i].HeadX[FrameNum] = allFish[i].head.x;
		saveDataVec[idxFile].allFishData[i].HeadY[FrameNum] = allFish[i].head.y;
		saveDataVec[idxFile].allFishData[i].TailX[FrameNum] = allFish[i].tail.x;
		saveDataVec[idxFile].allFishData[i].TailY[FrameNum] = allFish[i].tail.y;
		saveDataVec[idxFile].allFishData[i].CenterX[FrameNum] = allFish[i].center.x;
		saveDataVec[idxFile].allFishData[i].CenterY[FrameNum] = allFish[i].center.y;
		saveDataVec[idxFile].allFishData[i].HeadingAngle[FrameNum] = allFish[i].headingAngle;
		saveDataVec[idxFile].allFishData[i].ShockOn[FrameNum] = allFish[i].shockOn;
		saveDataVec[idxFile].allFishData[i].PatternIdx[FrameNum] = allFish[i].idxCase;

	}

}

void saveData::initialize(int totalTrialTime, int totalTrials,int frameRate,int numfish, std::string baseName) {
	//std::string  matname = path2save + baseName + ".mat";

	numFish = numfish;
	arraySize = totalTrialTime* totalTrials* frameRate;
	trialSize = totalTrialTime * frameRate;

	FrameNum = new int[arraySize];
	sElapsed = new int[arraySize];
	msRemElapsed = new int[arraySize];
	trialCount = new int[arraySize];
	for (int i = 0; i < numfish; i++) {
		fishData fishdata(arraySize, i);
		allFishData.push_back(fishdata);
	}
	totalData = new int[arraySize * (4 + numFish*9)];
}

void saveData::saveMat(std::string baseName) {
	std::string  matname = path2save + baseName + ".mat";
	const char* matName = matname.c_str();

	MATFile* pmatFile = matOpen(matName, "w");

	mxArray* pMxArray = mxCreateNumericMatrix(arraySize, (4 + numFish * 9), mxINT32_CLASS, mxREAL);

	for (int i = 0; i < arraySize; i++) {
		totalData[i] = FrameNum[i];
	}
	for (int i = 0; i < arraySize; i++) {
		totalData[i+ arraySize] = sElapsed[i];
	}
	for (int i = 0; i < arraySize; i++) {
		totalData[i+ arraySize*2] = msRemElapsed[i];
	}
	for (int i = 0; i < arraySize; i++) {
		totalData[i + arraySize * 3] = trialCount[i];
	}
	for (int j = 0; j < numFish;j++) {
		for (int m = 0; m < allFishData[j].totalFishData.size();m++) {
			for (int i = 0; i < arraySize; i++) {
				totalData[i + arraySize * (4+m) + j * arraySize * 9] = allFishData[j].totalFishData[m][i];
			}
		}
	}

	mxSetData(pMxArray, totalData);
	matPutVariable(pmatFile, "output", pMxArray);
	matClose(pmatFile);
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

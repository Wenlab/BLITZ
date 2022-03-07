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


void FileWriter::initialize(string baseName, int totalTrialTime, int totalTrials, int frameRate)
{
	saveData saveDataObj;
	saveDataObj.initialize(totalTrialTime, totalTrials, frameRate, baseName);

	saveDataVec.push_back(saveDataObj);


}

void FileWriter::writeOutExpSettings(UserInterface& UIobj)
{

	std::ofstream outfile;
	string txtName = path2save + UIobj.baseNames + ".txt";
	outfile.open(txtName);
	outfile << "FishAge" << ":" << UIobj.fishAge << "\n";
	outfile << "FishStrain" << ":" << UIobj.strainName << "\n";
	outfile << "Task" << ":" << UIobj.expTask << "\n";
	outfile << "ExpStartTime" << ":" << UIobj.startTimeStr << "\n";


	outfile.close();


}

void FileWriter::writeOutData(std::string baseNames) {

	saveDataVec[0].saveMat(baseNames);

}

void FileWriter::saveFrame(ExpTimer& timerObj, Fish& fishObj, FCamera& FcameraObj,
	int idxFile, int trialCount, int experimentStage) {
	int FrameNum = timerObj.count + trialCount * saveDataVec[idxFile].trialSize;

	saveDataVec[idxFile].FrameNum[FrameNum] = FrameNum;
	saveDataVec[idxFile].sElapsed[FrameNum] = timerObj.sElapsed;
	saveDataVec[idxFile].msRemElapsed[FrameNum] = timerObj.msRemElapsed;
	saveDataVec[idxFile].trialCount[FrameNum] = trialCount;
	saveDataVec[idxFile].experimentStage[FrameNum] = experimentStage;

	saveDataVec[idxFile].allFishData[0].HeadX[FrameNum] = fishObj.head.x;
	saveDataVec[idxFile].allFishData[0].HeadY[FrameNum] = fishObj.head.y;
	saveDataVec[idxFile].allFishData[0].ShockOn[FrameNum] = fishObj.shockOn;
	saveDataVec[idxFile].allFishData[0].PatternIdx[FrameNum] = fishObj.idxCase;
	saveDataVec[idxFile].allFishData[0].ExcitationIdx[FrameNum] = fishObj.pattern;

	for (int i = 0; i < 59; i++) {
		saveDataVec[idxFile].Fcamera[FrameNum][i] = FcameraObj.read_[i];
	}
	saveDataVec[idxFile].Fcamera[FrameNum][59] = FcameraObj.samps;

}

void saveData::initialize(int totalTrialTime, int totalTrials, int frameRate, std::string baseName) {
	//std::string  matname = path2save + baseName + ".mat";

	arraySize = totalTrialTime * totalTrials * frameRate;
	trialSize = totalTrialTime * frameRate;

	FrameNum = new int[arraySize];
	sElapsed = new int[arraySize];
	msRemElapsed = new int[arraySize];
	trialCount = new int[arraySize];
	experimentStage = new int[arraySize];
	Fcamera = new int*[arraySize];
	for (int i = 0; i < arraySize; i++) {
		Fcamera[i] = new int[60];
	}

	fishData fishdata(arraySize, 0);
	allFishData.push_back(fishdata);

	totalData = new int[arraySize * (5 + 5+60)];
}

void saveData::saveMat(std::string baseName) {
	std::string  matname = path2save + baseName + ".mat";
	const char* matName = matname.c_str();

	MATFile* pmatFile = matOpen(matName, "w");

	mxArray* pMxArray = mxCreateNumericMatrix(arraySize, (5 + 5+60), mxINT32_CLASS, mxREAL);

	for (int i = 0; i < arraySize; i++) {
		totalData[i] = FrameNum[i];
	}
	for (int i = 0; i < arraySize; i++) {
		totalData[i + arraySize] = sElapsed[i];
	}
	for (int i = 0; i < arraySize; i++) {
		totalData[i + arraySize * 2] = msRemElapsed[i];
	}
	for (int i = 0; i < arraySize; i++) {
		totalData[i + arraySize * 3] = trialCount[i];
	}
	for (int i = 0; i < arraySize; i++) {
		totalData[i + arraySize * 4] = experimentStage[i];
	}

	for (int m = 0; m < allFishData[0].totalFishData.size(); m++) {
		for (int i = 0; i < arraySize; i++) {
			totalData[i + arraySize * (5 + m)] = allFishData[0].totalFishData[m][i];
		}
	}
	for (int m = 0; m < 60; m++) {
		for (int i = 0; i < arraySize; i++) {
			totalData[i + arraySize * (5+5 + m)] = Fcamera[i][m];
		}
	}

	mxSetData(pMxArray, totalData);
	matPutVariable(pmatFile, "output", pMxArray);
	matClose(pmatFile);
}



void saveData::test_ini(int arraysize, std::string baseName) {
	//std::string  matname = path2save + baseName + ".mat";

	arraySize = arraysize;

	FrameNum = new int[arraySize];
	sElapsed = new int[arraySize];
	msRemElapsed = new int[arraySize];
	Fcamera = new int*[arraySize];
	for (int i=0; i < arraySize; i++) {
		Fcamera[i] = new int[100];
	}


	totalData = new int[arraySize * (103)];
}

void saveData::test_frame(ExpTimer& timerObj, FCamera& FcameraObj,
	int Framenum) {


	FrameNum[Framenum] = Framenum;
	sElapsed[Framenum] = timerObj.sElapsed;
	msRemElapsed[Framenum] = timerObj.msRemElapsed;

	for (int i = 0; i < 99; i++) {
		Fcamera[Framenum][i] = FcameraObj.read_[i];
	}
	Fcamera[Framenum][99] = FcameraObj.samps;



}

void saveData::test_save(std::string baseName) {
	std::string  matname = path2save + baseName + ".mat";
	const char* matName = matname.c_str();

	MATFile* pmatFile = matOpen(matName, "w");

	mxArray* pMxArray = mxCreateNumericMatrix(arraySize, (103), mxINT32_CLASS, mxREAL);

	for (int i = 0; i < arraySize; i++) {
		totalData[i] = FrameNum[i];
	}
	for (int i = 0; i < arraySize; i++) {
		totalData[i + arraySize] = sElapsed[i];
	}
	for (int i = 0; i < arraySize; i++) {
		totalData[i + arraySize * 2] = msRemElapsed[i];
	}
	for (int m = 0; m < 100; m++) {
		for (int i = 0; i < arraySize; i++) {
			totalData[i + arraySize * (3 + m)] = Fcamera[i][m];
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

	str.erase(str.end() - 1, str.end()); // remove the last charactor
	return str;
}



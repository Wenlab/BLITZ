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
* Filename: experiment.cpp
* Abstract: this file contains function definitions
*		used in constructing final behavioral learning experiment in zebrafish
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: Apr. 28, 2018

* Replaced Version: 1.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
*/

// Include user-defined libraries
#include "experiment.h"

// Include standard libraries
#include <iostream>
#include <string>
#include <random>
#include <iterator>
#include <functional>


using namespace std;
using namespace cv;

bool ExperimentData::initialize()
{
	//writeOut.get_CS_strings(CSpatterns);
	numCameras = writeOut.enquireInfoFromUser();	
	CSpatterns = get_CS_patterns(writeOut.CSstrs);
	/* Create yaml and video files to write in */
	if (!writeOut.initialize(pathName, WIDTH, HEIGHT, FRAMERATE,
		X_CUT, Y_CUT, yDivs))
		return false;
	

	if (!cams.initialize(numCameras, WIDTH, HEIGHT, FRAMERATE))
		return false;

	if (!screen.initialize(CSpatterns[0]))
		return false;


	/* Initialize the serial port */
	if (!thePort.initialize(COM_NUM))
		return false;
	
	allArenas = initializeAllArenas(yDivs, writeOut.fishIDs, writeOut.fishAge);

	return true;
}

void ExperimentData::prepareBgImg(const int prepareTime)
{
	int tempIdx = 0;
	while (tempIdx < prepareTime * FRAMERATE * numCameras)//expTimer.getElapsedTimeInSec() < prepareTime)
	{
		tempIdx++;
		int timeInSec = tempIdx / (FRAMERATE * numCameras);
		cams.grabPylonImg();
		//int timeInSec = expTimer.getElapsedTimeInSec();
		// Display progress message in the command window
		cout << "Preparing... " << timeInSec << " in " << prepareTime << " s" << endl;

		int cIdx = cams.cIdx;

		/* Convert Pylon image to opencvImg */
		allArenas[cIdx].prepareBgImg(cams.ptrGrabResult->GetWidth(), cams.ptrGrabResult->GetHeight()
			, cIdx, (uint8_t*)cams.pylonImg.GetBuffer());

		screen.renderTexture();
	}
	
}

void ExperimentData::runUnpairedOLexp()
{
	const int prepareTime = 1 * 60; // seconnds, default 1 min
	const int baselineEndTime = 10 * 60; // seconds, default 10 mins
	const int trainingEndTime = 30 * 60; // seconds, default 20 mins
	const int blackoutEndTime = 31 * 60; // seconds, default 1 min
	const int testEndTime = 49 * 60; // seconds, default 18 mins (including memory extinction period)
	const int expEndTime = testEndTime;

	const int numShocks = 20;
	vector<int> vec;
	for (int i = baselineEndTime * FRAMERATE; i < trainingEndTime*FRAMERATE; i++)
		vec.push_back(i);

	// First create an instance of an engine.
	random_device rnd_device;
	// Specify the engine and distribution.
	mt19937 g(rnd_device());
	shuffle(vec.begin(), vec.end(), g);
	vector<int> rndVec(vec.begin(), vec.begin() + numShocks);
	

	prepareBgImg(prepareTime);
	expTimer.start(); // reset timer to 0

	for (idxFrame = 0; idxFrame < numCameras * expEndTime * FRAMERATE; idxFrame++)// giant grabbing loop
	{

		cams.grabPylonImg();

		int cIdx = cams.cIdx;
		allArenas[cIdx].prepareBgImg(
			cams.ptrGrabResult->GetWidth(),
			cams.ptrGrabResult->GetHeight(),
			cIdx,
			(uint8_t*)cams.pylonImg.GetBuffer());

		getTime();
		if (!allArenas[cIdx].findAllFish())
			cout << "Fish in arena " << cIdx + 1 << " not found." << endl;
		if (sElapsed < baselineEndTime)
		{
			expPhase = 0;             //baseline = 0, training = 1, blackout = 2, test = 3
			screen.updatePatternInBaseline(sElapsed);
		}
		else if (sElapsed < trainingEndTime)
		{
			expPhase = 1;
			for (int i = 0; i < allArenas[cIdx].numFish; i++)
			{	// find certain element in the vector
				int target = (idxFrame - cIdx) / numCameras;
				if (find(rndVec.begin(), rndVec.end(), target) != rndVec.end())
					giveFishShock(i);
				else
					allArenas[cIdx].allFish[i].shockOn = 0;
				int pIdx = screen.allAreas[cIdx].allPatches[i].pIdx;
				screen.allAreas[cIdx].allPatches[i].pIdx
					= allArenas[cIdx].allFish[i].updatePatternInTraining(sElapsed, pIdx, ITI);
				screen.allAreas[cIdx].allPatches[i].updatePattern();
			}
		}
		else if (sElapsed < blackoutEndTime)
		{
			expPhase = 2;
			if (idxFrame == trainingEndTime * FRAMERATE * numCameras + cIdx)
			{
				allArenas[cIdx].resetShocksOn();
				// make it an AreaData method
				screen.updatePatternInBlackout();
			}
		}
		else if (sElapsed < testEndTime)
		{
			expPhase = 3;
			screen.updatePatternInTest(sElapsed);
		}
		screen.renderTexture();
		writeOutFrame();
		annotateFishImgs();
		displayFishImgs("Display");
	}
	cout << "Experiment ended. " << endl;
}

void ExperimentData::runBlueTest()
{
	const int prepareTime = 1 * 60; // seconnds, default 1 min
	const int baselineEndTime = 10 * 60; // seconds, default 10 mins
	const int trainingEndTime = 30 * 60; // seconds, default 20 mins
	const int expEndTime = trainingEndTime;

	prepareBgImg(prepareTime);
	expTimer.start(); // reset timer to 0

	for (idxFrame = 0; idxFrame < numCameras * expEndTime * FRAMERATE; idxFrame++)// giant grabbing loop
	{

		cams.grabPylonImg();
		int cIdx = cams.cIdx;
		allArenas[cIdx].prepareBgImg(
			cams.ptrGrabResult->GetWidth(),
			cams.ptrGrabResult->GetHeight(),
			cIdx,
			(uint8_t*)cams.pylonImg.GetBuffer());

		if (!getTime()) {
			break;
		}
		if (!allArenas[cIdx].findAllFish())
			cout << "Fish in arena " << cIdx + 1 << " not found." << endl;
		if (sElapsed < baselineEndTime)
		{
			expPhase = 0;             //baseline = 0, training = 1, blackout = 2, test = 3
			screen.updatePatternInBaseline(sElapsed);
		}
		else if (sElapsed < expEndTime)
		{
			expPhase = 1;
			screen.updatePatternInTest(sElapsed);
		}
		screen.renderTexture();
		writeOutFrame();
		annotateFishImgs();
		displayFishImgs("Display");
	}
	cout << "Experiment ended. " << endl;

}

void ExperimentData::runOLexp()
{
	//const int prepareTime = 1 * 60  ; // seconnds, default 1 min
	//const int baselineEndTime = 10 * 60 ; // seconds, default 10 mins
	//const int trainingEndTime = 30 * 60 ; // seconds, default 20 mins
	//const int blackoutEndTime =  31 * 60 ; // seconds, default 1 min
	//const int testEndTime = 49 * 60 ; // seconds, default 18 mins (including memory extinction period)
	//const int expEndTime = testEndTime;
	const int prepareTime = 1 * 60; // seconnds, default 1 min
	const int baselineEndTime = 10 * 60; // seconds, default 10 mins
	const int trainingEndTime = 30 * 60; // seconds, default 20 mins
	const int blackoutEndTime = 31 * 60; // seconds, default 1 min
	const int testEndTime = 49 * 60; // seconds, default 18 mins (including memory extinction period)
	const int expEndTime = testEndTime;

	prepareBgImg(prepareTime);
	expTimer.start(); // reset timer to 0

	for (idxFrame = 0; idxFrame < numCameras * expEndTime * FRAMERATE; idxFrame++)// giant grabbing loop
	{
	
		cams.grabPylonImg();
		int cIdx = cams.cIdx;
		allArenas[cIdx].prepareBgImg(
			cams.ptrGrabResult->GetWidth(), 
			cams.ptrGrabResult->GetHeight(),
			cIdx, 
			(uint8_t*)cams.pylonImg.GetBuffer());

		getTime();

		if (!allArenas[0].findAllFish())
			cout << "Fish in arena " << 1 << " not found."<< endl;
		if (sElapsed < baselineEndTime)
		{
			expPhase = 0;             //baseline = 0, training = 1, blackout = 2, test = 3
			screen.updatePatternInBaseline(sElapsed);
		}
		else if (sElapsed < trainingEndTime)
		{
			expPhase = 1;
			trainFish(0);
		}
		else if (sElapsed < blackoutEndTime)
		{
			expPhase = 2;
			allArenas[0].resetShocksOn();
			screen.updatePatternInBlackout();
		}
		else if (sElapsed < testEndTime)
		{
			expPhase = 3;
			screen.updatePatternInTest(sElapsed);
		}
		screen.renderTexture();
		writeOutFrame();
		annotateFishImgs();
		displayFishImgs("Display");
	}
	cout << "Experiment ended. " << endl;
}



void ExperimentData::giveFishShock(int fishIdx)
{
	int cIdx = cams.cIdx;
	allArenas[cIdx].allFish[fishIdx].shockOn = 1;
	// give a electric pulse
	thePort.givePulse(4*cIdx + fishIdx);
	allArenas[cIdx].allFish[fishIdx].lastShockTime = sElapsed;
}

void ExperimentData::trainFish(int cIdx) {
	for (int i = 0; i < allArenas[cIdx].numFish; i++)
	{
		int pIdx = screen.allAreas[cIdx].allPatches[i].pIdx;
		if (allArenas[cIdx].allFish[i].ifGiveShock(pIdx, sElapsed)) {
			giveFishShock(i);
		}
		screen.allAreas[cIdx].allPatches[i].pIdx 
			= allArenas[cIdx].allFish[i].updatePatternInTraining(sElapsed,pIdx,ITI);
		screen.allAreas[cIdx].allPatches[i].updatePattern();
	}
}

void ExperimentData::writeOutFrame()
{
	int cIdx = cams.cIdx;
	writeOut.videoVec[cIdx] << allArenas[cIdx].opencvImg; // write image to disk

	// write the custom class to disk
	writeOut.yamlVec[cIdx] << "Frames" << "[";
	// Python-like inline compact form
	// general experimental info
	writeOut.writeKeyValueInline("FrameNum", idxFrame / numCameras, cIdx);
	writeOut.writeKeyValueInline("ExpPhase", expPhase, cIdx);
	writeOut.writeKeyValueInline("sElapsed", sElapsed, cIdx);
	writeOut.writeKeyValueInline("msRemElapsed", msRemElapsed, cIdx);

	// specific fish analysis data
	vector<FishData> allFish = allArenas[cIdx].allFish;
	vector<PatchData> allPatches = screen.allAreas[cIdx].allPatches;
	string fishName;
	for (int i = 0; i < allFish.size(); i++)
	{
		writeOut.writeKeyValueInline("FishIdx", i, cIdx);
		writeOut.writeKeyValueInline("Head", allFish[i].head, cIdx);
		writeOut.writeKeyValueInline("Tail", allFish[i].tail, cIdx);
		writeOut.writeKeyValueInline("Center", allFish[i].center, cIdx);
		writeOut.writeKeyValueInline("HeadingAngle", allFish[i].headingAngle, cIdx);
		writeOut.writeKeyValueInline("ShockOn", allFish[i].shockOn, cIdx);
		writeOut.writeKeyValueInline("PatternIdx", allPatches[i].pIdx, cIdx);
	}
	writeOut.yamlVec[cIdx] << "]";

}

/* Adapted from code in stackfow */
void ExperimentData::displayFishImgs(string title) {
	int size;
	int i;
	int m, n;
	int x, y;

	// w - Maximum number of images in a row
	// h - Maximum number of images in a column
	int w, h;

	// scale - How much we have to resize the image
	float scale;
	int max;

	// If the number of arguments is lesser than 0 or greater than 12
	// return without displaying
	if (numCameras <= 0) {
		printf("Number of arguments too small....\n");
		return;
	}
	else if (numCameras > 3) {
		printf("Number of arguments too large, can only handle maximally 12 images at a time ...\n");
		return;
	}
	else if (numCameras == 1) {
		w = h = 1;
		size = 300;
	}
	else if (numCameras == 2) {
		w = 2; h = 1;
		size = 300;
	}
	else if (numCameras == 3) {
		w = 2; h = 2;
		size = 300;
	}
	// Create a new 1 channel image
	Mat DispImage = Mat::zeros(Size(100 + size * w, 60 + size * h), CV_8UC1);
	// Loop for cams.numCameras number of arguments
	for (i = 0, m = 20, n = 20; i < numCameras; i++, m += (20 + size)) {
		// Get the Pointer to the IplImage
		Mat img = allArenas[i].opencvImg;
		if (img.empty()) {
			std::cout << "Invalid arguments" << std::endl;
			return;
		}
		x = img.cols;
		y = img.rows;
		// Find whether height or width is greater in order to resize the image
		max = (x > y) ? x : y;
		// Find the scaling factor to resize the image
		scale = (float)((float)max / size);
		if (i % w == 0 && m != 20) {
			m = 20;
			n += 20 + size;
		}
		Rect ROI(m, n, (int)(x / scale), (int)(y / scale));
		Mat temp; resize(img, temp, Size(ROI.width, ROI.height));
		temp.copyTo(DispImage(ROI));
	}
	// Create a new window, and show the Single Big Image
	namedWindow(title, 1);
	imshow(title, DispImage);
}

void ExperimentData::annotateFishImgs()
{
	for (int i = 0; i < numCameras; i++)
	{
		vector<int> pIndices;
		AreaData area = screen.allAreas[i];
		for (int j = 0; j < area.numPatches; j++)
			pIndices.push_back(area.allPatches[j].pIdx);
		allArenas[i].annotateFish(pIndices);
	}
}

bool ExperimentData::getTime() {
	sElapsed = idxFrame / (FRAMERATE * numCameras);
	msRemElapsed = (int)expTimer.getElapsedTimeInMilliSec() % 1000;
	cout << "Time: " << sElapsed << " (s) " << endl;
	return true;
}

vector<string> ExperimentData::get_CS_patterns(vector<string> CS_strs) {
	vector<string> CSpatterns_string; 
	string imgFolderPath = "Images/";
	for (int i = 0; i < CS_strs.size(); i++)
	{
		CSpatterns_string.push_back(CS_strs[i]);
		cout << "CS pattern in " << i + 1 << " is: " << CSpatterns_string[i] << endl;
		CSpatterns_string[i] = imgFolderPath + CSpatterns_string[i] + ".jpg";
	}
	return CSpatterns_string;
}
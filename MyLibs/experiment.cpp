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
#include <chrono>
#include <string>


using namespace std;
using namespace cv;


bool ExperimentData::initialize(const char* imgName)
{
	if (!thePort.initialize(COM_NUM))
		return false;
	if (!cams.initialize())
		return false;
	const int historyLen = 2000; // used in MOG subtractor
	int numFishList[] = {2, 2, 2}; // the number of fish in each arena
	int binThreList[] = {30, 30, 30}; // the background threshold for each arena
	for (int i = 0; i < cams.numCameras; i++)
	{
		ArenaData arena(binThreList[i],numFishList[i]);
		arena.pMOG = cv::createBackgroundSubtractorMOG2(historyLen, arena.binThre, false);
		allArenas.push_back(arena);
	}
	if (!writeOut.initialize(cams.numCameras,cams.frameRate,cams.width,cams.height))
		return false;
	if (!screen.initialize(imgName, 3))
		return false;

	expTimer.start();
	return true;
}

void ExperimentData::prepareBgImg()
{
	const int prepareTime = 1 * 30; // seconds
	while (expTimer.getElapsedTimeInSec() < prepareTime)
	{
		cams.grabPylonImg();
		int timeInSec = expTimer.getElapsedTimeInSec();
		// Display progress message in the command window
		cout << "Preparing... " << timeInSec << " in " << to_string(prepareTime) << " s" << endl;

		int cIdx = cams.cIdx;

		/* Convert Pylon image to opencvImg */
		Mat rawImg = Mat(cams.ptrGrabResult->GetWidth(), cams.ptrGrabResult->GetHeight(),
			CV_8UC1, (uint8_t*)cams.pylonImg.GetBuffer());
		rawImg.copyTo(allArenas[cIdx].opencvImg);
		if (cIdx != 0)
			rot_90_CW(allArenas[cIdx].opencvImg, allArenas[cIdx].opencvImg);
		allArenas[cIdx].pMOG->apply(allArenas[cIdx].opencvImg, allArenas[cIdx].subImg);
		screen.renderTexture();
	}
		
	expTimer.start(); // reset timer to 0
	
	
}

void ExperimentData::runOLexp()
{
	// Get system time
	chrono::system_clock::time_point p = chrono::system_clock::now();
	time_t t = chrono::system_clock::to_time_t(p);
	char timeStr[26];
	ctime_s(timeStr,sizeof timeStr, &t);

	// Write out the general info for the experiment, only once
	for (int i = 0; i < cams.numCameras; i++)
	{
		writeOut.yamlVec[i] << "ExpStartTime" << timeStr;
		writeOut.yamlVec[i] << "FrameRate" << cams.frameRate;
		writeOut.yamlVec[i] << "FrameSize" << Size(cams.width, cams.height);
		writeOut.yamlVec[i] << "xCut" << allArenas[i].xCut;
		writeOut.yamlVec[i] << "yCut" << allArenas[i].xCut;
		vector<int> yDivideVec;
		for (int j = 0; j < screen.allAreas[i].numPatches; j++)
			yDivideVec.push_back(screen.allAreas[i].allPatches[j].yDivide);

		writeOut.yamlVec[i] << "DelimY" << yDivideVec;

		string fishName;
		for (int j = 0; j < allArenas[i].numFish; j++)
		{
			fishName = "Fish" + to_string(i);
			writeOut.yamlVec[i] << fishName << "[";
			writeOut.yamlVec[i] << screen.allAreas[i].allPatches[j].yDivide;
			writeOut.yamlVec[i] << "]";
		}
	}

	const int baselineEndTime = 1 * 60; // seconds
	const int trainingEndTime = 5 * 60; // seconds
	const int blackoutEndTime = 8 * 60; // seconds
	const int testEndTime = 10 * 60; // seconds
	const int expEndTime = testEndTime + 1;

	while (sElapsed <= expEndTime)// giant grabbing loop
	{
		cams.grabPylonImg();
		idxFrame++;
		int cIdx = cams.cIdx;
		// TODO: make the following block into function
		/* Convert Pylon image to opencvImg */
		Mat rawImg = Mat(cams.ptrGrabResult->GetWidth(), cams.ptrGrabResult->GetHeight(),
			CV_8UC1, (uint8_t*)cams.pylonImg.GetBuffer());
		rawImg.copyTo(allArenas[cIdx].opencvImg);
		if (cIdx != 0)
			rot_90_CW(allArenas[cIdx].opencvImg, allArenas[cIdx].opencvImg);
		// MOG motion tracking
		allArenas[cIdx].pMOG->apply(allArenas[cIdx].opencvImg, allArenas[cIdx].subImg);

		sElapsed = expTimer.getElapsedTimeInSec();
		msRemElapsed = (int)expTimer.getElapsedTimeInMilliSec() % 1000;
		cout << "Time: " << sElapsed << " (s) " << endl;
		if (!allArenas[cIdx].findAllFish())
			cout << "in arena: " << cIdx << endl;
		if (sElapsed < baselineEndTime)
		{
			expPhase = 0;
			updatePatternInBaseline();
		}
		else if (sElapsed < trainingEndTime)
		{
			expPhase = 1;
			for (int i = 0; i < allArenas[cIdx].numFish; i++)
			{
				if (ifGiveShock(i))
					giveFishShock(i);
				updatePatternInTraining(i);
			}
		}
		else if (sElapsed < blackoutEndTime)
		{
			expPhase = 2;
			/* TODO: The following code should be done only once */
			for (int i = 0; i < allArenas[cIdx].numFish; i++)
			{
				allArenas[cIdx].allFish[i].shockOn = 0;
				allArenas[cIdx].allFish[i].patternIndex = 2;
			}			
		}
		else if (sElapsed <= testEndTime)
		{
			expPhase = 3;
			updatePatternInTest();
		}
		else
		{ // experiment ends
			cout << "Experiment ended. " << endl;
			exit(0);
		}
		// TODO: level up the patternIdx update function to screen level
		for (int i = 0; i < screen.allAreas[cIdx].numPatches; i++)
		{
			screen.allAreas[cIdx].allPatches[i].updatePattern();
		}
		screen.renderTexture();
		writeOutFrame();
		annotateFishImgs();
		//imshow("Display", allArenas[0].opencvImg);
		displayFishImgs("Display");
	}
}

void ExperimentData::updatePatternInBaseline()
{
	if (sElapsed > lastScreenPatternUpdate + baselineInterval)
	{
		cout << "Update pattern during baseline session " << endl;
		lastScreenPatternUpdate = sElapsed;
		// uniformly choose a time from 15s to 45s
		baselineInterval = rand() % 30 + 15;

		int cIdx = cams.cIdx;
		for (int i = 0; i < screen.allAreas[cIdx].numPatches; i++)
		{
			screen.allAreas[cIdx].allPatches[i].pIdx = !screen.allAreas[cIdx].allPatches[i].pIdx;
		}

	}
	
}

void ExperimentData::updatePatternInTraining(int fishIdx)
{
	int cIdx = cams.cIdx;
	FishData fish = allArenas[cIdx].allFish[fishIdx];
	PatchData patch = screen.allAreas[cIdx].allPatches[fishIdx];
	int pIdx = patch.pIdx; // patternIdx
	int delimY = patch.yDivide;
	int lastTimeInCS = fish.lastTimeInCS;
	/* Update visual pattern whenver fish stays longer than NCStimeThre in non-CS area */
	int NCStimeThre = 48; // seconds
	int lastBlackoutStart = fish.lastBlackoutStart;
	int trialStart = fish.lastFishPatternUpdate;
	Point head = fish.head;

	if (pIdx == 2)
	{
		if (sElapsed > lastBlackoutStart + ITI)
		{
			/* Randomly choose a pattern for the patch */
			//allArenas[cIdx].allFish[fishIdx].patternIndex = rand() % 2;
			screen.allAreas[cIdx].allPatches[fishIdx].pIdx = rand() % 2;
			allArenas[cIdx].allFish[fishIdx].lastFishPatternUpdate = sElapsed;
			// restart a trial
			allArenas[cIdx].allFish[fishIdx].lastTimeInCS = sElapsed;
			allArenas[cIdx].allFish[fishIdx].lastTimeInNCS = sElapsed;

		}
	}
	else {
		// update lastTimeInCS and lastTimeInNCS of fish
		if (pIdx) // patternIdx == 1, since patternIdx == 2 is excluded
		{
			if (head.y < delimY) // In non-CS area
				allArenas[cIdx].allFish[fishIdx].lastTimeInNCS = sElapsed;
			else
				allArenas[cIdx].allFish[fishIdx].lastTimeInCS = sElapsed;

		}
		else {
			if (head.y > delimY) // In non-CS area
				allArenas[cIdx].allFish[fishIdx].lastTimeInNCS = sElapsed;
			else
				allArenas[cIdx].allFish[fishIdx].lastTimeInCS = sElapsed;
		}

		if (sElapsed - lastTimeInCS > NCStimeThre) // if stays too long in non-CS area
		{
			screen.allAreas[cIdx].allPatches[fishIdx].pIdx = 2;
			allArenas[cIdx].allFish[fishIdx].lastBlackoutStart = sElapsed;
		}
	}



}

void ExperimentData::updatePatternInTest()
{
	int testInterval = 30; // seconds, the interval in test is fixed
	if (sElapsed > lastScreenPatternUpdate + testInterval)
	{
		cout << "Update pattern during test" << endl;
		lastScreenPatternUpdate = sElapsed;
		int cIdx = cams.cIdx;
		for (int i = 0; i < screen.allAreas[cIdx].numPatches; i++)
		{
			screen.allAreas[cIdx].allPatches[i].pIdx = !screen.allAreas[cIdx].allPatches[i].pIdx;
		}
	}
}

bool ExperimentData::ifGiveShock(int fishIdx)
{
	int cIdx = cams.cIdx;
	FishData fish = allArenas[cIdx].allFish[fishIdx];
	PatchData patch = screen.allAreas[cIdx].allPatches[fishIdx];
	int pIdx = patch.pIdx; // patternIdx
	int delimY = patch.yDivide;
	Point head = fish.head;
	int headingAngle = fish.headingAngle;
	int lastShockTime = fish.lastShockTime;

	/* Control parameters */
	int thinkingTime = 7; // seconds, give fish some thinking time
	int shockCD = 3; // seconds
	/* Give fish a shock whenever it stays in CS area too long */
	int CStimeThre = 10;

	if (pIdx == 2) // blackout 
		return false;
	if (sElapsed < fish.lastFishPatternUpdate + thinkingTime)
		return false;
	if (sElapsed < fish.lastShockTime + shockCD)
		return false;
	if (head.x == -1) // invalid frame
		return false;
	if (pIdx) // patternIdx == 1, since 2 is already excluded
	{
		if (head.y < delimY) // in non-CS area
			return false;
		else {
			if (sElapsed - lastShockTime > CStimeThre)
				return true;
			else {
				if (headingAngle < 0) // fish is trying to escape CS area
					return false;
				else
					return true;
			}
		}
	}
	else
	{
		if (head.y > delimY)
			return false;
		else {
			if (sElapsed - lastShockTime > CStimeThre)
				return true;
			else {
				if (headingAngle > 0) // fish is trying to escape CS area
					return false;
				else
					return true;
			}
		}
	}
	

}

void ExperimentData::giveFishShock(int fishIdx)
{
	int cIdx = cams.cIdx;
	// give a electric pulse
	thePort.givePulse(MAX_FISH_PER_ARENA*cIdx + fishIdx);
	allArenas[cIdx].allFish[fishIdx].shockOn = 1;
	allArenas[cIdx].allFish[fishIdx].lastShockTime = sElapsed;
}


void ExperimentData::writeOutFrame()
{
	int cIdx = cams.cIdx;
	writeOut.videoVec[cIdx] << allArenas[cIdx].opencvImg; // write image to disk

	// write the custom class to disk
	writeOut.yamlVec[cIdx] << "Frames" << "[";
	// Python-like inline compact form 
	// general experimental info
	writeOut.yamlVec[cIdx] << "{:" << "FrameNum" << idxFrame << "}";
	writeOut.yamlVec[cIdx] << "{:" << "ExpPhase" << expPhase << "}";
	writeOut.yamlVec[cIdx] << "{:" << "sElapsed" << sElapsed << "}";
	writeOut.yamlVec[cIdx] << "{:" << "msRemElapsed" << msRemElapsed << "}";
	// specific fish analysis data
	vector<FishData> allFish = allArenas[cIdx].allFish;
	vector<PatchData> allPatches = screen.allAreas[cIdx].allPatches;
	string fishName;
	for (int i = 0; i < allFish.size(); i++)
	{
		writeOut.yamlVec[cIdx] << "{:" << "FishIdx" << i << "}";
		//fishName = "Fish" + to_string(i);
		//writeOut.yamlVec[cIdx] << fishName << "[";
		writeOut.yamlVec[cIdx] << "{:" << "Head" << allFish[i].head << "}";
		writeOut.yamlVec[cIdx] << "{:" << "Tail" << allFish[i].tail << "}";
		writeOut.yamlVec[cIdx] << "{:" << "Center" << allFish[i].center << "}";
		writeOut.yamlVec[cIdx] << "{:" << "HeadingAngle" << allFish[i].headingAngle << "}";
		writeOut.yamlVec[cIdx] << "{:" << "ShockOn" << allFish[i].shockOn << "}";
		writeOut.yamlVec[cIdx] << "{:" << "PatternIdx" << allPatches[i].pIdx << "}";
		//writeOut.yamlVec[cIdx] << "]";
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
	if (cams.numCameras <= 0) {
		printf("Number of arguments too small....\n");
		return;
	}
	else if (cams.numCameras > 14) {
		printf("Number of arguments too large, can only handle maximally 12 images at a time ...\n");
		return;
	}
	// Determine the size of the image,
	// and the number of rows/cols
	// from number of arguments
	else if (cams.numCameras == 1) {
		w = h = 1;
		size = 300;
	}
	else if (cams.numCameras == 2) {
		w = 2; h = 1;
		size = 300;
	}
	else if (cams.numCameras == 3 || cams.numCameras == 4) {
		w = 2; h = 2;
		size = 300;
	}
	else if (cams.numCameras == 5 || cams.numCameras == 6) {
		w = 3; h = 2;
		size = 200;
	}
	else if (cams.numCameras == 7 || cams.numCameras == 8) {
		w = 4; h = 2;
		size = 200;
	}
	else {
		w = 4; h = 3;
		size = 150;
	}

	// Create a new 1 channel image
	Mat DispImage = Mat::zeros(Size(100 + size * w, 60 + size * h), CV_8UC1);

	// Loop for cams.numCameras number of arguments
	for (i = 0, m = 20, n = 20; i < cams.numCameras; i++, m += (20 + size)) {
		// Get the Pointer to the IplImage
		//Mat img = va_arg(args, Mat);
		Mat img = allArenas[i].opencvImg;
		// Check whether it is NULL or not
		// If it is NULL, release the image, and return
		if (img.empty()) {
			printf("Invalid arguments");
			return;
		}

		// Find the width and height of the image
		x = img.cols;
		y = img.rows;

		// Find whether height or width is greater in order to resize the image
		max = (x > y) ? x : y;

		// Find the scaling factor to resize the image
		scale = (float)((float)max / size);

		// Used to Align the images
		if (i % w == 0 && m != 20) {
			m = 20;
			n += 20 + size;
		}

		// Set the image ROI to display the current image
		// Resize the input image and copy the it to the Single Big Image
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
	for (int i = 0; i < cams.numCameras; i++)
	{
		ArenaData arena = allArenas[i];
		for (int j = 0; j < allArenas[i].numFish; j++)
		{
			int pIdx = screen.allAreas[i].allPatches[j].pIdx;
			// Put text on each image respectively
			if (pIdx == 0)
				putText(allArenas[i].opencvImg, "CS TOP", Point(10, 45), FONT_HERSHEY_TRIPLEX, 1, Scalar::all(255), 2);
			else if (pIdx == 1)
				putText(allArenas[i].opencvImg, "CS BOTTOM", Point(10, 45), FONT_HERSHEY_TRIPLEX, 1, Scalar::all(255), 2);

			Point head = allArenas[i].allFish[j].head;
			if (head.x == -1) // invalid fish analysis data
				continue;
			circle(allArenas[i].opencvImg, head, 5, Scalar(255), 2);
			circle(allArenas[i].opencvImg, allArenas[i].allFish[j].tail, 3, Scalar(255), 2);

			

		}
	}
}

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
	
	const vector<vector<float>> allAreaPos =
	{
		{ 0.082f, 0.300f, 0.258f, 0.668f },
		{ 0.840f, -0.810f, 0.258f, 0.73f },
		{ -0.665f, -0.810f, 0.258f, 0.73f }
	};
	// y division pos for all fish
	vector<vector<int>> yDivs =
	{
		{ 195, 195, 574, 574 },
		{ 223, 223, 588, 588 },
		{ 214, 214, 588, 588 }
	};

	vector<vector<int>> yPatternDivs =
	{
		{ 818, 818, 942, 942 },
		{ 247, 247, 365, 365 },
		{ 238, 238, 358, 358 }
	};

	int binThreList[] = { 30, 30, 30 }; // the background threshold for each arena
	string imgFolderPath = "Images/";

	showWelcomeMsg();
	string imgStr = imgFolderPath + CSpattern + ".jpg";
	const char* imgName = imgStr.c_str();
	string CSstr = get_CS_string(CSpattern);
	string timeStr = get_current_date_time();
	numCameras = enquireNumCams();
	if (!cams.initialize(numCameras, WIDTH, HEIGHT, FRAMERATE))
		return false;
	cout << endl; // separated with an empty line

	cout << "Initializing the projector screen .. " << endl;
	if (!screen.initialize(imgName, numCameras))
		return false;
	cout << endl; // separated with an empty line

	/* Initialize the serial port */
	if (!thePort.initialize(COM_NUM))
		return false;
	cout << endl; // separated with an empty line

	int fishAge = enquireFishAge();
	string expTask = enquireExpTask();
	
	for (int i = 0; i < numCameras; i++)
	{
		// create ArenaData and push it into exp.allArenas
		vector<string> fishIDs = enquireFishIDs(i);

		ArenaData arena(binThreList[i], fishIDs.size());

		arena.initialize(fishIDs, fishAge, yDivs[i]);
		allArenas.push_back(arena);

		// create AreaData and push it into screen.allAreas
		// the screen coordinates are (-1,1)
		AreaData area(allAreaPos[i], arena.numFish); 
		area.initialize(yPatternDivs[i]);
		screen.allAreas.push_back(area);

		// Append strain info to contentName
		string strainName = get_strainName(fishIDs[0][0]);
		string contentName = timeStr + "_" + "Arena" + to_string(i+1) 
			+ "_" + strainName + "_" + to_string(fishAge)
			+ "dpf_" + expTask + "_" + CSstr;

		/* Create yaml and video files to write in */
		if (!writeOut.initialize(contentName, WIDTH, HEIGHT, FRAMERATE))
			return false;

		/* Write out general experiment context info */
			
		writeOut.writeKeyValuePair("FishIDs", strVec2str(fishIDs), i);
		writeOut.writeKeyValuePair("FishAge", fishAge, i);
		writeOut.writeKeyValuePair("FishStrain", strainName, i);
		writeOut.writeKeyValuePair("Arena", i+1, i); // record which arena is in use
		writeOut.writeKeyValuePair("Task", expTask, i);
		writeOut.writeKeyValuePair("CSpattern", CSstr, i);
		writeOut.writeKeyValuePair("ExpStartTime", timeStr, i);
		writeOut.writeKeyValuePair("FrameRate", FRAMERATE, i);
		writeOut.writeKeyValuePair("FrameSize", Size(WIDTH, HEIGHT), i);
		writeOut.writeKeyValuePair("xCut", X_CUT, i);
		writeOut.writeKeyValuePair("yCut", Y_CUT, i);
		writeOut.writeKeyValuePair("yDivide", yDivs[i], i);


	}
	expTimer.start();
	return true;
}

void ExperimentData::prepareBgImg(const int prepareTime)
{
	while (expTimer.getElapsedTimeInSec() < prepareTime)
	{
		cams.grabPylonImg();
		int timeInSec = expTimer.getElapsedTimeInSec();       
		// Display progress message in the command window                                                          
		cout << "Preparing... " << timeInSec << " in " << to_string(prepareTime) << " s" << endl;

		int cIdx = cams.cIdx;

		/* Convert Pylon image to opencvImg */
		allArenas[cIdx].prepareBgImg(cams.ptrGrabResult->GetWidth(), cams.ptrGrabResult->GetHeight()
			, cIdx, (uint8_t*)cams.pylonImg.GetBuffer());
		
		screen.renderTexture();
	}
	expTimer.start(); // reset timer to 0
}
void ExperimentData::runUnpairedOLexp()
{
	const int prepareTime = 1 * 60; // seconnds, default 1 min
	const int baselineEndTime = 1 * 60; // seconds, default 10 mins
	const int trainingEndTime = 8 * 60; // seconds, default 20 mins
	const int blackoutEndTime = 8 * 60; // seconds, default 1 min
	const int testEndTime = 10 * 60; // seconds, default 18 mins (including memory extinction period)
	const int expEndTime = testEndTime;

	const int numShocks = 20; // comparable to shocks fish received in the normal OLexp group
	
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

	while (idxFrame < numCameras * expEndTime * FRAMERATE)// giant grabbing loop
	{
		cams.grabPylonImg();
		idxFrame++;
		int cIdx = cams.cIdx;
		// TODO: make the following block into function
		/* Convert Pylon image to opencvImg */
		allArenas[cIdx].prepareBgImg(cams.ptrGrabResult->GetWidth(), cams.ptrGrabResult->GetHeight()
			, cIdx, (uint8_t*)cams.pylonImg.GetBuffer());
		sElapsed = expTimer.getElapsedTimeInSec();
		msRemElapsed = (int)expTimer.getElapsedTimeInMilliSec() % 1000;
		cout << "Time: " << sElapsed << " (s) " << endl;
		if (!allArenas[cIdx].findAllFish())
			cout << "in arena: " << cIdx << endl;
		if (sElapsed < baselineEndTime)
		{
			expPhase = 0;
			screen.updatePatternInBaseline(sElapsed);
		}
		else if (sElapsed < trainingEndTime)
		{
			expPhase = 1;
			for (int i = 0; i < allArenas[cIdx].numFish; i++)
			{	// find certain element in the vector
				int target = (idxFrame - cIdx) / numCameras;
				if (find(rndVec.begin(), rndVec.end(), target) != rndVec.end())
					giveFishShock(i,1);
				else
					allArenas[cIdx].allFish[i].shockOn = 0;
				updatePatternInTraining(i);
			}
		}
		else if (sElapsed < blackoutEndTime)
		{
			expPhase = 2;
			/* TODO: The following code should be done only once */
			allArenas[cIdx].BlackoutExp();
			screen.BlackoutExp();
			/* The problem is I change pIdx of allArenas but pIdx of area is the useful one*/
		}
		else if (sElapsed <= testEndTime)
		{
			expPhase = 3;
			screen.updatePatternInTest(sElapsed);
		}
		else
		{ // experiment ends
		  //cout << "Experiment ended. " << endl;
		  //exit(0);
		}
		screen.updatePattern();
		screen.renderTexture();
		writeOutFrame();
		annotateFishImgs();

		displayFishImgs("Display");
	}
	cout << "Experiment ended. " << endl;
}


void ExperimentData::runOLexp()
{
	const int prepareTime = 1 * 60 ; // seconnds, default 1 min   
	const int baselineEndTime = 1 * 60 ; // seconds, default 10 mins
	const int trainingEndTime = 9 * 60 ; // seconds, default 20 mins
	const int blackoutEndTime = 10 * 60 ; // seconds, default 1 min
	const int testEndTime = 11 * 60 ; // seconds, default 18 mins (including memory extinction period)
	const int expEndTime = testEndTime;

	prepareBgImg(prepareTime);
	
	while (idxFrame < numCameras * expEndTime * FRAMERATE )// giant grabbing loop
	{
		cams.grabPylonImg();
		
		idxFrame++;
		
		int cIdx = cams.cIdx;      

		allArenas[cIdx].prepareBgImg(cams.ptrGrabResult->GetWidth(), cams.ptrGrabResult->GetHeight()
			, cIdx, (uint8_t*)cams.pylonImg.GetBuffer());

		sElapsed = expTimer.getElapsedTimeInSec();     
		msRemElapsed = (int)expTimer.getElapsedTimeInMilliSec() % 1000; 
		cout << "Time: " << sElapsed << " (s) " << endl;

		if (!allArenas[cIdx].findAllFish())
			cout << "in arena: " << cIdx << endl;
		if (sElapsed < baselineEndTime)
		{
			expPhase = 0;             //baseline = 0, training = 1, blackout = 2, test = 3
			screen.updatePatternInBaseline(sElapsed);                
		}
		else if (sElapsed < trainingEndTime)
		{
			expPhase = 1;
			TrainingExp(cIdx);
		}
		else if (sElapsed < blackoutEndTime)
		{
			expPhase = 2;
			screen.BlackoutExp();
			allArenas[cIdx].BlackoutExp();
		}
		else if (sElapsed <= testEndTime)
		{
			expPhase = 3;
			screen.updatePatternInTest(sElapsed);
		}
		else
		{   // experiment ends
			//cout << "Experiment ended. " << endl;
			//exit(0);
		}
		screen.updatePattern();         
		screen.renderTexture();
		writeOutFrame();
		annotateFishImgs();
		displayFishImgs("Display");
	}
	cout << "Experiment ended. " << endl;
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


void ExperimentData::giveFishShock(int fishIdx, int flag)
{
	int cIdx = cams.cIdx;
	allArenas[cIdx].allFish[fishIdx].shockOn = flag;
	if (!flag)
		return;
	// give a electric pulse
	thePort.givePulse(4*cIdx + fishIdx);
	allArenas[cIdx].allFish[fishIdx].lastShockTime = sElapsed;
}

void ExperimentData::TrainingExp(int cIdx) {
	for (int i = 0; i < allArenas[cIdx].numFish; i++)
	{
		int pIdx = screen.allAreas[cIdx].allPatches[i].pIdx;
		int flag = allArenas[cIdx].allFish[i].ifGiveShock(pIdx, sElapsed);
		giveFishShock(i, flag);
		updatePatternInTraining(i);
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
	writeOut.writeKeyValueInline("FrameNum", idxFrame, cIdx);
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
	else if (numCameras > 14) {
		printf("Number of arguments too large, can only handle maximally 12 images at a time ...\n");
		return;
	}
	// Determine the size of the image,
	// and the number of rows/cols
	// from number of arguments
	else if (numCameras == 1) {
		w = h = 1;
		size = 300;
	}
	else if (numCameras == 2) {
		w = 2; h = 1;
		size = 300;
	}
	else if (numCameras == 3 || numCameras == 4) {
		w = 2; h = 2;
		size = 300;
	}
	else if (numCameras == 5 || numCameras == 6) {
		w = 3; h = 2;
		size = 200;
	}
	else if (numCameras == 7 || numCameras == 8) {
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
	for (i = 0, m = 20, n = 20; i < numCameras; i++, m += (20 + size)) {
		// Get the Pointer to the IplImage
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
	for (int i = 0; i < numCameras; i++)
	{
		ArenaData arena = allArenas[i];
		for (int j = 0; j < allArenas[i].numFish; j++)
		{
			int pIdx = screen.allAreas[i].allPatches[j].pIdx;
			// Put text on each image respectively
			/*if (pIdx == 0)
				putText(allArenas[i].opencvImg, "CS TOP", Point(10, 45), FONT_HERSHEY_TRIPLEX, 1, Scalar::all(255), 2);
			else if (pIdx == 1)
				putText(allArenas[i].opencvImg, "CS BOTTOM", Point(10, 45), FONT_HERSHEY_TRIPLEX, 1, Scalar::all(255), 2);*/

			Point head = allArenas[i].allFish[j].head;
			if (head.x == -1) // invalid fish analysis data
				continue;
			circle(allArenas[i].opencvImg, head, 5, Scalar(255), 2);
			circle(allArenas[i].opencvImg, allArenas[i].allFish[j].tail, 3, Scalar(255), 2);

		}
	}
}


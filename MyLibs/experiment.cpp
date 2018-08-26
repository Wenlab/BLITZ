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
	//y division pos for all patch
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
		if (!writeOut.initialize(pathName,contentName, WIDTH, HEIGHT, FRAMERATE))
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
	//expTimer.start(); 
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
	expTimer.start(); // reset timer to 0
	while (idxFrame < numCameras * expEndTime * FRAMERATE)// giant grabbing loop
	{
		cams.grabPylonImg();
		idxFrame++;
		int cIdx = cams.cIdx;
		allArenas[cIdx].prepareBgImg(cams.ptrGrabResult->GetWidth(), cams.ptrGrabResult->GetHeight()
			, cIdx, (uint8_t*)cams.pylonImg.GetBuffer());
		getTime();
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
		screen.renderTexture();
		writeOutFrame();
		annotateFishImgs();

		displayFishImgs("Display");
	}
	cout << "Experiment ended. " << endl;
}

void ExperimentData::runOLexp()
{
	const int prepareTime = 1 * 60 / 10 ; // seconnds, default 1 min
	const int baselineEndTime = 1 ; // seconds, default 10 mins
	const int trainingEndTime = 2 ;//* 60 / 10 ; // seconds, default 20 mins
	const int blackoutEndTime = 4;// *60 / 10; // seconds, default 1 min
	const int testEndTime = 5 * 60 / 10 ; // seconds, default 18 mins (including memory extinction period)
	const int expEndTime = testEndTime;

	prepareBgImg(prepareTime);
	expTimer.start(); // reset timer to 0



	while (idxFrame < numCameras * expEndTime * FRAMERATE )// giant grabbing loop
	{
		idxFrame++;
		
		cams.grabPylonImg();

		int cIdx = cams.cIdx;
		allArenas[cIdx].prepareBgImg(
			cams.ptrGrabResult->GetWidth(), 
			cams.ptrGrabResult->GetHeight(),
			cIdx, 
			(uint8_t*)cams.pylonImg.GetBuffer());

		
		getTime();
		
		if (!allArenas[cIdx].findAllFish())
			//cout << "Fish in arena " << cIdx << "not found."<< endl;
		if (sElapsed < baselineEndTime)
		{
			expPhase = 0;             //baseline = 0, training = 1, blackout = 2, test = 3
			screen.updatePatternInBaseline(sElapsed);
		}
		else if (sElapsed < trainingEndTime)
		{
			expPhase = 1;
			trainFish(cIdx);
		}
		else if (sElapsed < blackoutEndTime)
		{
			expPhase = 2;
			//cout << idxFrame << endl;
			//cout << trainingEndTime * FRAMERATE * numCameras + cIdx << endl;
			if (idxFrame == trainingEndTime * FRAMERATE * numCameras + cIdx)
			{
				allArenas[cIdx].resetShocksOn();
				// make it an AreaData method
				screen.updatePatternInBlackout();
			}
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
		screen.renderTexture();
		writeOutFrame();
		annotateFishImgs();
		displayFishImgs("Display");
	}
	cout << "Experiment ended. " << endl;
	system("pause");
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
		allArenas[i].annotateFish();
	}
}

void ExperimentData::getTime() {
	sElapsed = idxFrame / (FRAMERATE * numCameras);//expTimer.getElapsedTimeInSec();
	msRemElapsed = (int)expTimer.getElapsedTimeInMilliSec() % 1000;
	cout << "Time: " << sElapsed << " (s) " << endl;
}
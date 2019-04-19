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

// TODO: make each module can be enabled separately
bool Experiment::initialize()
{
	//writeOut.get_CS_strings(CSpatterns);
	numCameras = writeOut.enquireInfoFromUser(); // TODO: put this function into UserInterface class
	CSpatterns = get_CS_patterns(writeOut.CSstrs); // TODO: this line should be incorperated into the above function.
	/* Create yaml and video files to write in */
	if (!writeOut.initialize(pathName, WIDTH, HEIGHT, FRAMERATE,
		X_CUT, Y_CUT, yDivs))
		return false;
	// TODO: consider to improve this error handling with exceptions

	if (!cams.initialize(numCameras, WIDTH, HEIGHT, FRAMERATE)) // TODO: consider to improve this error handling with exceptions
		return false;

	allArenas = initializeAllArenas(yDivs, writeOut.fishIDs, writeOut.fishAge);
	// TODO: -> fishAnalysisObj.initialize(UIobj.fishIDs, UIobj.fishAge);

	fileWriterObj.writeOutExpSettings();
	return true;
}

/* example method for running a specific experiment */

void Experiment::runXXexp()
{
	// parameters
	const int prepareTime = 1 * 60; // seconnds, default 1 min
	const int baselineEndTime = 2 * 60; // seconds, default 10 mins
	const int trainingEndTime = 8 * 60; // seconds, default 20 mins
	const int blackoutEndTime = 9 * 60; // seconds, default 1 min
	const int testEndTime = 15 * 60; // seconds, default 18 mins (including memory extinction period)
	const int expEndTime = testEndTime;

	int ITI = 10; // seconds, Inter-trial Interval
	int lastScreenPatternUpdate = -1; // seconds
	const int NCStimeThre = 48; // seconds
	const baselineInterval = 30; // seconds
	const testInterval = 30; // seconds


	// experimental procedures

	for (timerObj.idxFrame = 0; timerObj.idxFrame < getIdxFrame(FRAMERATE, prepareTime, 0); timerObj.idxFrame++) {
		camerasObj.grabPylonImg(); // grabbing
		fishAnalysisObj.prepareBgImg((uint8_t*)cams.getPtr2buffer());
	}

	timerObj.start();

	// TODO: consider to add some abstraction?
	for (timerObj.idxFrame = 0; timerObj.idxFrame < getIdxFrame(expEndTime, 0); timerObj.idxFrame++) // TODO: how to modularize inside this giant grabbing loop.
	{
		camerasObj.grabPylonImg(); // grabbing
		int cIdx = camerasObj.cIdx;

		fishAnalysisObj.alignImgs(cIdx, (uint8_t*)cams.pylonImg.GetBuffer());// TODO: -> cams.getPtr2buffer()
		timerObj.getTime();

		fishAnalysisObj.findAllFish();

		// run baseline session
		if (idxFrame < getIdxFrame(baselineEndTime, 0))
		{
			timerObj.expPhase = 0;
			screen.reverse(sElapsed, baselineInterval);
		}
		else if (idxFrame < getIdxFrame(trainingEndTime, 0))
		{
			timerObj.expPhase = 1;
			vector<int> fish2shock = fishAnalysisObj.checkIfGiveShock();
			relayObj.givePulse(fish2shock);
			// update pattern indices based on shock status, fish positions, and current pattern
			vector<int> fish2reversePattern = fishAnalysisObj.checkIfReversePattern(); // TODO: is there a better place to put this method?
			screen.reverse(fish2reversePattern);
		}
		else if (idxFrame < getIdxFrame(blackoutEndTime, 0))
		{
			timerObj.expPhase = 2;
			fishAnalysisObj.resetShocksOn();
			screen.renderBlackPattern();
		}
		else if (idxFrame < getIdxFrame(testEndTime, 0))
		{
			timerObj.expPhase = 3;
			screen.reverse(sElapsed-trainingEndTime, testInterval);
		}
		fileWriterObj.writeOutFrame();
		fishAnalysisObj.annotateImgs();
		fishAnalysisObj.displayImgs();
	}
	cout << "Experiment ended. " << endl;
}


void ExperimentData::runOLexp()
{
	const int prepareTime = 1 * 60; // seconnds, default 1 min
	const int baselineEndTime = 2 * 60; // seconds, default 10 mins
	const int trainingEndTime = 8 * 60; // seconds, default 20 mins
	const int blackoutEndTime = 9 * 60; // seconds, default 1 min
	const int testEndTime = 15 * 60; // seconds, default 18 mins (including memory extinction period)
	const int expEndTime = testEndTime;

	prepareBgImg(prepareTime); // TODO: -> FishAnalysisObj.prepareBgImg();
	expTimer.start(); // reset timer to 0

	// TODO: consider to encapsulate FRAMERATE?
	for (idxFrame = 0; idxFrame < numCameras * expEndTime * FRAMERATE; idxFrame++)// giant grabbing loop
	{

		cams.grabPylonImg();

		int cIdx = cams.cIdx;
		// TODO: encapsulate the following lines to a FishAnalysis method.
		// e.g., fishAnalysisObj.prepareBgImg(WIDTH, HEIGHT, cIdx, (uint8_t*)cams.pylonImg.GetBuffer());
		// TODO: consider to drop out the first two arguments with global variables
		allArenas[cIdx].prepareBgImg(
			cams.ptrGrabResult->GetWidth(),
			cams.ptrGrabResult->GetHeight(),
			cIdx,
			(uint8_t*)cams.pylonImg.GetBuffer());

		if (!getTime()) { // TODO: consider to improve the error handling.
			break;
		}
		if (!allArenas[cIdx].findAllFish()) // TODO: make this line a FishAnalysis method as well as improve error handling
			cout << "Fish in arena " << cIdx + 1 << " not found."<< endl;
		// TODO: -> fishAnalysisObj.findAllFish();
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
			allArenas[cIdx].resetShocksOn();
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

	// TODO: wrap the above RNG code

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
					allArenas[cIdx].allFish[i].shockOn = 0; // TODO: condiser to align abstraction level
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
		screen.renderTexture(); // TODO: Specify rendering style, e.g., avoidance, OMR, etc.
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

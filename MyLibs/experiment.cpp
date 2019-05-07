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
	UIobj.enquireInfoFromUser();
	// TODO: consider to encapsulate this property with a method
	if (UIobj.devices2use[0]) // initialize relay
		relayObj.initialize();
	else if (UIobj.devices2use[1]) // initialize pattern rendering
	  screenObj.initialize();
	else if (UIobj.devices2use[2]) // initialize cameras
	{
		camerasObj.initialize(UIobj.cameras2open); // TODO: use method instead
		fishAnalysisObj.initialize();
	}

	fileWriterObj.initialize(UIobj);
	fileWriterObj.writeOutExpSettings(); //TODO: consider to move this into runXXexp?
	timerObj.initialize();
}

void Experiment::runOLexp()
{
	const int prepareTime = 1 * 60; // seconnds, default 1 min
	const int baselineEndTime = 2 * 60; // seconds, default 10 mins
	const int trainingEndTime = 8 * 60; // seconds, default 20 mins
	const int blackoutEndTime = 9 * 60; // seconds, default 1 min
	const int testEndTime = 15 * 60; // seconds, default 18 mins (including memory extinction period)
	const int expEndTime = testEndTime;

	int ITI = 10; // seconds, inter-trial interval, TODO: implement the concept of trials
	const int baselineInterval = 30; // seconds
	const int testInterval = 30; // seconds

	// Preparation before experiment starts
	// TODO: write the implementations
	for (timerObj.resetCount(); timerObj.getCount() < camerasObj.getIdxEndFrame(prepareTime); timerObj.addCount()) // TODO: write a macro to encapsulate this `for`
	{
		camerasObj.grabPylonImg(); // TODO: update the return type
		int cIdx = camerasObj.cIdx;
		fishAnalysisObj.aIdx = cIdx;


		fishAnalysisObj.getImgFromCamera((uint8_t*)camerasObj.getPtr2buffer(), cIdx);
		// TODO: include the following two lines in the method "preprocessImg"
		fishAnalysisObj.alignImg();
		fishAnalysisObj.buildBgImg(cIdx);

	}

	timerObj.reset(); //TODO, reset seconds and count

	for (timerObj.resetCount(); timerObj.getCount() < camerasObj.getIdxEndFrame(prepareTime); timerObj.addCount())
	{
		timerObj.getTime();
		camerasObj.grabPylonImg(); // grabbing
		int cIdx = camerasObj.getIdxCamera();
		fishAnalysisObj.getImgFromCamera((uint8_t*)camerasObj.getPtr2buffer(), cIdx);
		fishAnalysisObj.alignImg(cIdx);
		fishAnalysisObj.buildBgImg(cIdx);

		fishAnalysisObj.findAllFish();

		// run baseline session
		if (timerObj.getCount() < camerasObj.getIdxEndFrame(baselineEndTime))
		{
			timerObj.expPhase = 0;
			screen.reverse(sElapsed, baselineInterval);
		}
		else if (timerObj.getCount() < camerasObj.getIdxEndFrame(trainingEndTime))
		{
			timerObj.expPhase = 1;
			vector<vector<bool>> fish2shock = fishAnalysisObj.checkIfGiveShock();
			// TODO: map 2D fish2shock to 1D
			relayObj.givePulse(fish2shock);
			// update pattern indices based on shock status, fish positions, and current pattern
			vector<vector<bool>> fish2reversePattern = fishAnalysisObj.checkIfReversePattern(); // TODO: is there a better place to put this method?
			screen.reverse(fish2reversePattern);
		}
		else if (timerObj.getCount() < camerasObj.getIdxEndFrame(trainingEndTime))
		{
			timerObj.expPhase = 3;
			screen.reverse(sElapsed-trainingEndTime, testInterval);
		}
		fileWriterObj.writeOutFrame();
		fishAnalysisObj.annotateImgs();
		fishAnalysisObj.displayImg("Live");

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

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
#include <numeric>
#include <algorithm>


using namespace std;
using namespace cv;

void Experiment::initialize()
{
	// TODO: better place for this variable?
	const vector<vector<float>> allAreaPos =
	{
		{ 0.068f, 0.300f, 0.258f, 0.668f },
		{ 0.840f, -0.810f, 0.258f, 0.73f },
		{ -0.668f, -0.810f, 0.258f, 0.73f }
	};

	vector<int> patchesInAreas = {4,4,4};
	// TODO: make this variable as an argument
	string renderType = "half";

	UIobj.enquireInfoFromUser();
	// TODO: consider to encapsulate this property with a method
	if (UIobj.devices2use[0]) // initialize relay
		relayObj.initialize(COM_NUM);
	else if (UIobj.devices2use[1]) // initialize pattern rendering
		screenObj.initialize(UIobj.visPattern, renderType, allAreaPos, patchesInAreas);
	else if (UIobj.devices2use[2]) // initialize cameras
	{
		camerasObj.initialize(UIobj.cameras2open); // TODO: use method instead
		vector<int> numFishInArenas;
		for (auto i : UIobj.allFishIDs)
			numFishInArenas.push_back(i.size());
		
		fishAnalysisObj.initialize(numFishInArenas);
	}

	fileWriterObj.initialize(UIobj.baseNames, 
		Point(camerasObj.frameWidth,camerasObj.frameHeight),
		camerasObj.frameRate);

	fileWriterObj.writeOutExpSettings(UIobj,camerasObj,fishAnalysisObj);

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
	for (timerObj.resetCount(); timerObj.getCount() < camerasObj.time2IdxFrame(prepareTime); timerObj.addCount()) // TODO: write a macro to encapsulate this `for`
	{
		camerasObj.grabPylonImg(); // TODO: update the return type
		int cIdx = camerasObj.cIdx;
		fishAnalysisObj.aIdx = cIdx;

		fishAnalysisObj.getImgFromCamera(camerasObj.frameWidth, camerasObj.frameHeight, (uint8_t*)camerasObj.getPtr2buffer());
		// TODO: include the following two lines in the method "preprocessImg"
		fishAnalysisObj.alignImg();
		fishAnalysisObj.buildBgImg();

	}

	timerObj.start(); //TODO, reset seconds and count

	for (timerObj.resetCount(); timerObj.getCount() < camerasObj.time2IdxFrame(prepareTime); timerObj.addCount())
	{
		timerObj.getTime();
		camerasObj.grabPylonImg(); // grabbing
		int cIdx = camerasObj.cIdx;
		fishAnalysisObj.aIdx = cIdx;

		fishAnalysisObj.getImgFromCamera(camerasObj.frameWidth, camerasObj.frameHeight, (uint8_t*)camerasObj.getPtr2buffer());
		// TODO: include the following two lines in the method "preprocessImg"
		fishAnalysisObj.alignImg();
		fishAnalysisObj.buildBgImg();

		fishAnalysisObj.findAllFish();

		// run baseline session
		if (timerObj.getCount() < camerasObj.time2IdxFrame(baselineEndTime))
		{
			timerObj.expPhase = 0;
			screenObj.reverse(timerObj.sElapsed, baselineInterval);
		}
		else if (timerObj.getCount() < camerasObj.time2IdxFrame(trainingEndTime))
		{
			timerObj.expPhase = 1;
			vector<vector<bool>> fish2shock = fishAnalysisObj.checkWhich2GiveShock(timerObj.sElapsed);
			// TODO: map 2D fish2shock to 1D
			vector<bool> channels2open;
			for (vector<bool> v : fish2shock)
				for (auto i : v)
					channels2open.push_back(i);

			relayObj.givePulse(channels2open);
			// update pattern indices based on shock status, fish positions, and current pattern
			vector<vector<bool>> fish2reversePattern = fishAnalysisObj.checkWhich2ReversePattern(timerObj.sElapsed); // TODO: is there a better place to put this method?
			screenObj.reverse(fish2reversePattern);
		}
		else if (timerObj.getCount() < camerasObj.time2IdxFrame(trainingEndTime))
		{
			timerObj.expPhase = 3;
			screenObj.reverse(timerObj.sElapsed-trainingEndTime, testInterval);
		}
		fileWriterObj.writeOutFrame(timerObj, fishAnalysisObj, cIdx);
		fishAnalysisObj.annotateImg();
		fishAnalysisObj.displayImg("Live");

	}
	cout << "Experiment ended. " << endl;

}

void Experiment::runUnpairedOLexp()
{
	const int prepareTime = 1 * 60; // seconnds, default 1 min
	const int baselineEndTime = 1 * 60; // seconds, default 10 mins
	const int trainingEndTime = 8 * 60; // seconds, default 20 mins
	const int blackoutEndTime = 8 * 60; // seconds, default 1 min
	const int testEndTime = 10 * 60; // seconds, default 18 mins (including memory extinction period)
	const int expEndTime = testEndTime;
	const int baselineInterval = 30; // seconds
	const int testInterval = 30; // seconds

	const int numShocks = 20; // comparable to shocks fish received in the normal OLexp group


	// write a function to sample frames2giveShock
	vector<bool> frames2giveShock = RNGsampleFrames2giveShock(numShocks, trainingEndTime - baselineEndTime);
	for (timerObj.resetCount(); timerObj.getCount() < camerasObj.time2IdxFrame(prepareTime); timerObj.addCount()) // TODO: write a macro to encapsulate this `for`
	{
		camerasObj.grabPylonImg(); // TODO: update the return type
		int cIdx = camerasObj.cIdx;
		fishAnalysisObj.aIdx = cIdx;

		fishAnalysisObj.getImgFromCamera(camerasObj.frameWidth, camerasObj.frameHeight, (uint8_t*)camerasObj.getPtr2buffer());
		// TODO: include the following two lines in the method "preprocessImg"
		fishAnalysisObj.alignImg();
		fishAnalysisObj.buildBgImg();

	}
	timerObj.start();

	for (timerObj.resetCount(); timerObj.getCount() < camerasObj.time2IdxFrame(prepareTime); timerObj.addCount())
	{
		timerObj.getTime();
		camerasObj.grabPylonImg(); // grabbing
		int cIdx = camerasObj.cIdx;
		fishAnalysisObj.aIdx = cIdx;

		fishAnalysisObj.getImgFromCamera(camerasObj.frameWidth, camerasObj.frameHeight, (uint8_t*)camerasObj.getPtr2buffer());
		// TODO: include the following two lines in the method "preprocessImg"
		fishAnalysisObj.alignImg();
		fishAnalysisObj.buildBgImg();

		fishAnalysisObj.findAllFish();

		// run baseline session
		if (timerObj.getCount() < camerasObj.time2IdxFrame(baselineEndTime))
		{
			timerObj.expPhase = 0;
			screenObj.reverse(timerObj.sElapsed, baselineInterval);
		}
		else if (timerObj.getCount() < camerasObj.time2IdxFrame(trainingEndTime))
		{
			timerObj.expPhase = 1;
			vector<bool> channels2open;
			if (frames2giveShock[timerObj.getCount() - camerasObj.time2IdxFrame(baselineEndTime)])
				channels2open.resize(12, true);//TODO: the magic number means max number of channels, consider to have this variable in the Relay class
			else
				channels2open.resize(12, false);

			relayObj.givePulse(channels2open);
			// update pattern indices based on shock status, fish positions, and current pattern
			vector<vector<bool>> fish2reversePattern = fishAnalysisObj.checkWhich2ReversePattern(timerObj.sElapsed); // TODO: is there a better place to put this method?
			screenObj.reverse(fish2reversePattern);
		}
		else if (timerObj.getCount() < camerasObj.time2IdxFrame(trainingEndTime))
		{
			timerObj.expPhase = 3;
			screenObj.reverse(timerObj.sElapsed - trainingEndTime, testInterval);
		}
		fileWriterObj.writeOutFrame(timerObj, fishAnalysisObj, cIdx);
		fishAnalysisObj.annotateImg();
		fishAnalysisObj.displayImg("Live");

	}
	cout << "Experiment ended. " << endl;
}

vector<bool> Experiment::RNGsampleFrames2giveShock(int numShocks, int sampleDuration, int startTime)
{
	// First create an instance of an engine.
	random_device rnd_device;
	// Specify the engine and distribution.
	mt19937 g(rnd_device());
	vector<int> vec(sampleDuration);
	iota(vec.begin(), vec.end(), 1);

	shuffle(vec.begin(), vec.end(), g);
	vector<int> rndVec(vec.begin(), vec.begin() + numShocks);

	for (auto& i : rndVec)
		i += startTime;
	
	int frameRate = camerasObj.frameRate;
	int numCameras = UIobj.numOpenCameras;
	vector<bool> frames2giveShock(sampleDuration * camerasObj.frameRate);
	for (auto& i : rndVec)
		fill(frames2giveShock.begin() + frameRate * i, frames2giveShock.begin() + frameRate * i + numCameras, true);
	
	return frames2giveShock;


}
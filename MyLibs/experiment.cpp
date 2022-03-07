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

//
//void Experiment::initializeTest_2()
//{
//	//vector<int> xDivs = { 207,207,207,207,207,207 ,207,207,207 };
//	//vector<string> fishIDs = { "0","1","2","3","4","5","6","7","8" };
//	UIobj.enquireInfoFromUser();
//
//	relayObj.initialize(COM_NUM);
//	TCPobj.initialize();
//	screenObj.initialize(2);
//
//}
//
//void Experiment::runOLexptest_2() {
//	const int prepareTime = 1 * 60; // seconnds, default 1 min
//
//	int trialCount = 0;
//	//bool randomPattern;
//	const int trialTime = 120;// seconds
//	const int subTrialTime = 30;// seconds
//	const int ITI = 10; // seconds, inter-trial interval, TODO: implement the concept of trials
//	const int baselineTrial = 5;
//	const int trainingTrial = 10;
//	const int testTrial = 10;
//	const int frameRate = 50;
//
//	// Preparation before experiment starts
//
//	fileWriterObj.initialize(UIobj.baseNames, (trialTime + ITI), (baselineTrial + trainingTrial + testTrial), frameRate);
//	fileWriterObj.writeOutExpSettings(UIobj);
//	/*
//	Control the pattern displayed
//	0=leftPatch; 1=rightPatch; 2=blackPatch
//	*/
//	bool randomPattern = 1;
//
//
//	timerObj.start();
//	TCPobj.getLocation();
//	// baseline
//	experimentStage = 0;
//	for (; trialCount < baselineTrial; trialCount++) {
//
//		for (timerObj.resetCount(), randomPattern = (std::rand() % 2); timerObj.getCount() < (ITI + trialTime) * frameRate; timerObj.addCount()) {
//
//			timerObj.getTime();
//			TCPobj.getLocation();
//
//			//refresh screen
//			if (timerObj.getCount() == 0) {
//				fishObj.idxCase = randomPattern;
//				screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX,TCPobj.platformY);
//			}
//
//			if ((timerObj.getCount() + 1) == (subTrialTime * frameRate) || (timerObj.getCount() + 1) == (subTrialTime * frameRate) * 2 || (timerObj.getCount() + 1) == (subTrialTime * frameRate) * 3) {
//				randomPattern = !(randomPattern);
//				fishObj.idxCase = randomPattern;
//				screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
//			}
//
//			if ((timerObj.getCount() + 1) == trialTime * frameRate) {
//				fishObj.idxCase = blackPatch;
//				screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
//			}
//
//			fishObj.getPosition(TCPobj.platformX, TCPobj.platformY);
//	
//			TCPobj.setExcitation_test(fishObj.generatePattern(timerObj.getCount()));
//
//			fileWriterObj.saveFrame(timerObj, fishObj, 0, trialCount, experimentStage);
//
//			while (1) {
//				timerObj.getTime();
//				if ((timerObj.sElapsed * 1000 + timerObj.msRemElapsed) > timerObj.getCount() * 20)
//					break;
//			}
//		}
//		std::cout << trialCount << std::endl;
//	}
//
//	//train
//	experimentStage = 1;
//	for (; trialCount < baselineTrial + trainingTrial; trialCount++) {
//		for (timerObj.resetCount(); timerObj.getCount() < (ITI + trialTime) * frameRate; timerObj.addCount()) {
//			timerObj.getTime();
//			TCPobj.getLocation();
//			//refresh screen
//			if (timerObj.getCount() == 0) {
//				fishObj.setNewPattern();
//				screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
//				fishObj.lastTimeUpdatePattern = timerObj.sElapsed;
//			}
//
//			if ((timerObj.getCount() + 1) == (subTrialTime * frameRate) || (timerObj.getCount() + 1) == (subTrialTime * frameRate) * 2 || (timerObj.getCount() + 1) == (subTrialTime * frameRate) * 3) {
//				fishObj.setNewPattern();
//				screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
//				fishObj.lastTimeUpdatePattern = timerObj.sElapsed;
//			}
//
//			if ((timerObj.getCount() + 1) == trialTime * frameRate) {
//				fishObj.idxCase = blackPatch;
//				screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
//				fishObj.lastTimeUpdatePattern = timerObj.sElapsed;
//			}
//
//			fishObj.getPosition(TCPobj.platformX, TCPobj.platformY);
//			TCPobj.setExcitation_test(fishObj.generatePattern(timerObj.getCount()));
//
//			//fileWriterObj.saveFrame(timerObj, fishObj, 0, trialCount, experimentStage);
//
//			/*
//			Control the electric shock
//			0=no shock; 1=0.1s electric shock
//			channel 2 and 5 is not being used
//			*/
//			vector<bool> channels2open = {0,0,0,0,0,0,0,0};
//
//			channels2open[0]=fishObj.checkIfGiveShock2Test(timerObj.sElapsed);
//			relayObj.givePulse(channels2open,0.2);
//			channels2open.clear();
//
//			fileWriterObj.saveFrame(timerObj, fishObj, 0, trialCount, experimentStage);
//
//			while (1) {
//				timerObj.getTime();
//				if ((timerObj.sElapsed * 1000 + timerObj.msRemElapsed) > timerObj.getCount() * 20)
//					break;
//			}
//		}
//		std::cout << trialCount << std::endl;
//	}
//
//	//test
//	experimentStage = 2;
//	for (; trialCount < baselineTrial + trainingTrial + testTrial; trialCount++) {
//
//		for (timerObj.resetCount(), randomPattern = (std::rand() % 2); timerObj.getCount() < (ITI + trialTime) * frameRate; timerObj.addCount()) {
//
//			timerObj.getTime();
//			TCPobj.getLocation();
//
//			//refresh screen
//			if (timerObj.getCount() == 0) {
//				fishObj.idxCase = randomPattern;
//				screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
//			}
//
//			if ((timerObj.getCount() + 1) == (subTrialTime * frameRate) || (timerObj.getCount() + 1) == (subTrialTime * frameRate) * 2 || (timerObj.getCount() + 1) == (subTrialTime * frameRate) * 3) {
//				randomPattern = !(randomPattern);
//				fishObj.idxCase = randomPattern;
//				screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
//			}
//
//			if ((timerObj.getCount() + 1) == trialTime * frameRate) {
//				fishObj.idxCase = blackPatch;
//				screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
//			}
//
//			fishObj.getPosition(TCPobj.platformX, TCPobj.platformY);
//
//			TCPobj.setExcitation_test(fishObj.generatePattern(timerObj.getCount()));
//
//			fileWriterObj.saveFrame(timerObj, fishObj, 0, trialCount, experimentStage);
//
//			while (1) {
//				timerObj.getTime();
//				if ((timerObj.sElapsed * 1000 + timerObj.msRemElapsed) > timerObj.getCount() * 20)
//					break;
//			}
//		}
//		std::cout << trialCount << std::endl;
//	}
//	fileWriterObj.writeOutData(UIobj.baseNames);
//
//	std::cout << "Experiment ended. " << std::endl;
//	//cv::waitKey();
//}

void Experiment::initializeTest_3()
{

	UIobj.enquireInfoFromUser();
	relayObj.initialize(COM_NUM);
	TCPobj.initialize();
	screenObj2.initialize(2);
	fCameraObj.initialize();
	fCameraObj.startTask();

}

void Experiment::runOLexptest_3() {
	//const int prepareTime = 1 * 60; // seconnds, default 1 min

	int trialCount = 0;
	//bool randomPattern;
	const int trialTime = 120;// seconds
	const int subTrialTime = 30;// seconds
	const int ITI = 10; // seconds, inter-trial interval, TODO: implement the concept of trials
	const int baselineTrial = 5;
	const int trainingTrial = 10;
	const int testTrial = 10;
	const int frameRate = 20;

	// Preparation before experiment starts

	fileWriterObj.initialize(UIobj.baseNames, (trialTime + ITI), (baselineTrial + trainingTrial + testTrial), frameRate);
	fileWriterObj.writeOutExpSettings(UIobj);
	/*
	Control the pattern displayed
	0=leftPatch; 1=rightPatch; 2=blackPatch
	*/
	bool randomPattern = 1;


	timerObj.start();
	TCPobj.getLocation();
	// baseline
	experimentStage = 0;
	for (; trialCount < baselineTrial; trialCount++) {

		for (timerObj.resetCount(), randomPattern = (std::rand() % 2); timerObj.getCount() < (ITI + trialTime) * frameRate; timerObj.addCount()) {

			timerObj.getTime();
			TCPobj.getLocation();

			//refresh screen
			if (timerObj.getCount() == 0) {
				fishObj.idxCase = randomPattern;
				//screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
			}

			if ((timerObj.getCount() + 1) == (subTrialTime * frameRate) || (timerObj.getCount() + 1) == (subTrialTime * frameRate) * 2 || (timerObj.getCount() + 1) == (subTrialTime * frameRate) * 3) {
				randomPattern = !(randomPattern);
				fishObj.idxCase = randomPattern;
				//screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
			}

			if ((timerObj.getCount() + 1) == trialTime * frameRate) {
				fishObj.idxCase = fullPatch;
				//screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
			}
			screenObj2.refresh(fishObj.idxCase, window2, TCPobj.screenX, TCPobj.screenY);
			fishObj.getPosition(TCPobj.screenX, TCPobj.screenY);

			
			TCPobj.setExcitation(fishObj.generatePattern(timerObj.getCount()));

			fCameraObj.getFromFcamera();

			fileWriterObj.saveFrame(timerObj, fishObj, fCameraObj, 0, trialCount, experimentStage);
			
			if (timerObj.getCount() % 50 == 0) {
				std::cout << timerObj.getCount()<< std::endl;
			}

			while (1) {
				timerObj.getTime();
				if ((timerObj.sElapsed * 1000 + timerObj.msRemElapsed) > timerObj.getCount() * 50)
					break;
			}
		}
		std::cout << trialCount << std::endl;
	}

	//train
	experimentStage = 1;
	for (; trialCount < baselineTrial + trainingTrial; trialCount++) {
		for (timerObj.resetCount(); timerObj.getCount() < (ITI + trialTime) * frameRate; timerObj.addCount()) {
			timerObj.getTime();
			TCPobj.getLocation();
			//refresh screen
			if (timerObj.getCount() == 0) {
				fishObj.setNewPattern();
				
				fishObj.lastTimeUpdatePattern = timerObj.sElapsed;
			}

			if ((timerObj.getCount() + 1) == (subTrialTime * frameRate) || (timerObj.getCount() + 1) == (subTrialTime * frameRate) * 2 || (timerObj.getCount() + 1) == (subTrialTime * frameRate) * 3) {
				fishObj.setNewPattern();
				//screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
				fishObj.lastTimeUpdatePattern = timerObj.sElapsed;
			}

			if ((timerObj.getCount() + 1) == trialTime * frameRate) {
				fishObj.idxCase = fullPatch;
				//screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
				fishObj.lastTimeUpdatePattern = timerObj.sElapsed;
			}
			screenObj2.refresh(fishObj.idxCase, window2, TCPobj.screenX, TCPobj.screenY);

			fishObj.getPosition(TCPobj.screenX, TCPobj.screenY);


			TCPobj.setExcitation(fishObj.generatePattern(timerObj.getCount()));

			//fileWriterObj.saveFrame(timerObj, fishObj, 0, trialCount, experimentStage);

			/*
			Control the electric shock
			0=no shock; 1=0.1s electric shock
			channel 2 and 5 is not being used
			*/
			vector<bool> channels2open = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

			channels2open[0] = fishObj.checkIfGiveShock2Test(timerObj.sElapsed);
			relayObj.givePulse(channels2open,0.2);
			channels2open.clear();

			fCameraObj.getFromFcamera();

			fileWriterObj.saveFrame(timerObj, fishObj, fCameraObj, 0, trialCount, experimentStage);

			if (timerObj.getCount() % 50 == 0) {
				std::cout << timerObj.getCount() << std::endl;
			}

			while (1) {
				timerObj.getTime();
				if ((timerObj.sElapsed * 1000 + timerObj.msRemElapsed- baselineTrial* (ITI + trialTime) *1000) > timerObj.getCount() * 50)
					break;
			}
		}
		std::cout << trialCount << std::endl;
	}

	//test
	experimentStage = 2;
	for (; trialCount < baselineTrial + trainingTrial + testTrial; trialCount++) {

		for (timerObj.resetCount(), randomPattern = (std::rand() % 2); timerObj.getCount() < (ITI + trialTime) * frameRate; timerObj.addCount()) {

			timerObj.getTime();
			TCPobj.getLocation();

			//refresh screen
			if (timerObj.getCount() == 0) {
				fishObj.idxCase = randomPattern;
				//screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
			}

			if ((timerObj.getCount() + 1) == (subTrialTime * frameRate) || (timerObj.getCount() + 1) == (subTrialTime * frameRate) * 2 || (timerObj.getCount() + 1) == (subTrialTime * frameRate) * 3) {
				randomPattern = !(randomPattern);
				fishObj.idxCase = randomPattern;
				//screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
			}

			if ((timerObj.getCount() + 1) == trialTime * frameRate) {
				fishObj.idxCase = fullPatch;
				//screenObj2.refresh(fishObj.idxCase, window2, TCPobj.platformX, TCPobj.platformY);
			}
			screenObj2.refresh(fishObj.idxCase, window2, TCPobj.screenX, TCPobj.screenY);
			fishObj.getPosition(TCPobj.screenX, TCPobj.screenY);

			TCPobj.setExcitation(fishObj.generatePattern(timerObj.getCount()));
			
			fCameraObj.getFromFcamera();

			fileWriterObj.saveFrame(timerObj, fishObj, fCameraObj, 0, trialCount, experimentStage);

			if (timerObj.getCount() % 50 == 0) {
				std::cout << timerObj.getCount() << std::endl;
			}

			while (1) {
				timerObj.getTime();
				if ((timerObj.sElapsed * 1000 + timerObj.msRemElapsed- (baselineTrial+ trainingTrial) * (ITI + trialTime) * 1000) > timerObj.getCount() * 50)
					break;
			}
		}
		std::cout << trialCount << std::endl;
	}
	fileWriterObj.writeOutData(UIobj.baseNames);
	TCPobj.closeExcitation();
	std::cout << "Experiment ended. " << std::endl;
	//cv::waitKey();
}

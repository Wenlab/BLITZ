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
#include "experiment_cv.h"

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
	vector<int> xDivs = { 215,215,215,215,215,215 };
	vector<string> fishIDs = { "0","1","3","4","6","7" };
	UIobj.enquireInfoFromUser();

	relayObj.initialize(COM_NUM);
	cameraObj.initialize();
	ArenaObj.initialize(xDivs, fishIDs);
	screenObj.initialize();



}


void Experiment::runOLexp() {
	const int prepareTime = 1 * 60; // seconnds, default 1 min

	int trialCount = 0;
	bool randomPattern;
	const int trialTime = 90;// seconds
	const int subTrialTime = 30;// seconds
	const int ITI = 20; // seconds, inter-trial interval, TODO: implement the concept of trials
	const int baselineTrial = 6;
	const int trainingTrial = 12;
	const int testTrial = 6;
	const int frameRate = 10;

	// Preparation before experiment starts

	fileWriterObj.initialize(UIobj.baseNames, cv::Size(cameraObj.frameWidth, cameraObj.frameHeight), (trialTime + ITI), (baselineTrial + trainingTrial + testTrial),cameraObj.frameRate,6);
	fileWriterObj.writeOutExpSettings(UIobj, cameraObj, ArenaObj, UIobj.baseNames);

	for (timerObj.resetCount(); timerObj.getCount() < cameraObj.time2IdxFrame(prepareTime); timerObj.addCount()) // TODO: write a macro to encapsulate this `for`
	{
		cameraObj.grabPylonImg();

		ArenaObj.getImgFromCamera(cameraObj.frameWidth, cameraObj.frameHeight, (uint8_t*)cameraObj.getPtr2buffer());

		ArenaObj.buildBgImg();

	}


	/*
	Control the pattern displayed
	0=leftPatch; 1=rightPatch; 2=blackPatch
	*/
	vector<int> PatternIdx = { 2,2,2,2,2,2};

	timerObj.start();
	cv::namedWindow("Live");

	// baseline
	for (; trialCount < baselineTrial; trialCount++) {
		// single trial
		for (timerObj.resetCount(), randomPattern = (std::rand() % 2); timerObj.getCount() < (ITI + trialTime) * frameRate; timerObj.addCount()) {

			timerObj.getTime();

			//refresh screen
			if (timerObj.getCount() == 0) {
				for (int i = 0; i < ArenaObj.numFish; i++) {
					ArenaObj.allFish[i].idxCase = randomPattern;
					PatternIdx[i] = ArenaObj.allFish[i].idxCase;
				}
				screenObj.refresh(PatternIdx);
			}

			if ((timerObj.getCount() + 1) == (subTrialTime * frameRate) || (timerObj.getCount() + 1) == (subTrialTime * frameRate)*2) {
				randomPattern = !(randomPattern);
				for (int i = 0; i < ArenaObj.numFish; i++) {
					ArenaObj.allFish[i].idxCase = randomPattern;
					PatternIdx[i] = ArenaObj.allFish[i].idxCase;
				}
				screenObj.refresh(PatternIdx);
			}

			if ((timerObj.getCount() + 1)  == trialTime* frameRate) {

				for (int i = 0; i < ArenaObj.numFish; i++) {
					ArenaObj.allFish[i].idxCase = blackPatch;
					PatternIdx[i] = ArenaObj.allFish[i].idxCase;
				}
				screenObj.refresh(PatternIdx);
			}

			cameraObj.grabPylonImg();

			ArenaObj.getImgFromCamera(cameraObj.frameWidth, cameraObj.frameHeight, (uint8_t*)cameraObj.getPtr2buffer());
			ArenaObj.buildBgImg();
			ArenaObj.findAllFish(ArenaObj.corner_x, ArenaObj.corner_y, ArenaObj.width, ArenaObj.height);

			fileWriterObj.writeOutFrame(ArenaObj, 0);
			fileWriterObj.saveFrame(timerObj, ArenaObj, 0, trialCount);

			if (timerObj.getCount() % 10 == 0) {
				ArenaObj.displayImg("Live");
			}

		}
		cout << trialCount << endl;
	}

	for (; trialCount < baselineTrial+ trainingTrial; trialCount++) {
		for (timerObj.resetCount(), randomPattern = (std::rand() % 2); timerObj.getCount() < (ITI + trialTime) * frameRate; timerObj.addCount()) {
			timerObj.getTime();

			//refresh screen
			if (timerObj.getCount() == 0) {
				for (int i = 0; i < ArenaObj.numFish; i++) {
					ArenaObj.allFish[i].idxCase = randomPattern;
					PatternIdx[i] = ArenaObj.allFish[i].idxCase;
					ArenaObj.allFish[i].lastTimeUpdatePattern = timerObj.sElapsed;
				}
				screenObj.refresh(PatternIdx);
			}

			if ((timerObj.getCount() + 1) == (subTrialTime * frameRate) || (timerObj.getCount() + 1) == (subTrialTime * frameRate) * 2) {
				randomPattern = !(randomPattern);
				for (int i = 0; i < ArenaObj.numFish; i++) {
					ArenaObj.allFish[i].idxCase = randomPattern;
					PatternIdx[i] = ArenaObj.allFish[i].idxCase;
					ArenaObj.allFish[i].lastTimeUpdatePattern = timerObj.sElapsed;
				}
				screenObj.refresh(PatternIdx);
			}

			if ((timerObj.getCount() + 1) == trialTime * frameRate) {
				for (int i = 0; i < ArenaObj.numFish; i++) {
					ArenaObj.allFish[i].idxCase = blackPatch;
					PatternIdx[i] = ArenaObj.allFish[i].idxCase;
					ArenaObj.allFish[i].lastTimeUpdatePattern = timerObj.sElapsed;
				}
				screenObj.refresh(PatternIdx);
			}

			cameraObj.grabPylonImg();

			ArenaObj.getImgFromCamera(cameraObj.frameWidth, cameraObj.frameHeight, (uint8_t*)cameraObj.getPtr2buffer());
			ArenaObj.buildBgImg();
			ArenaObj.findAllFish(ArenaObj.corner_x, ArenaObj.corner_y, ArenaObj.width, ArenaObj.height);

			/*
			Control the electric shock
			0=no shock; 1=0.1s electric shock
			channel 2 and 5 is not being used
			*/
			vector<bool> channels2open;
			for (int i = 0; i < ArenaObj.numFish; i++) {
				if(i==2||i==5)
					channels2open.push_back(0);
				channels2open.push_back(ArenaObj.allFish[i].checkIfGiveShock2(timerObj.sElapsed));
			}
			relayObj.givePulse(channels2open);
			channels2open.clear();

			fileWriterObj.writeOutFrame(ArenaObj, 0);
			fileWriterObj.saveFrame(timerObj, ArenaObj, 0, trialCount);

			if (timerObj.getCount() % 10 == 0) {
				ArenaObj.displayImg("Live");
			}
		}
		cout << trialCount << endl;
	}

	for (; trialCount < baselineTrial+ trainingTrial+ testTrial; trialCount++) {
		for (timerObj.resetCount(), randomPattern = (std::rand() % 2); timerObj.getCount() < (ITI + trialTime) * frameRate; timerObj.addCount()) {
			timerObj.getTime();

			//refresh screen
			if (timerObj.getCount() == 0) {
				for (int i = 0; i < ArenaObj.numFish; i++) {
					ArenaObj.allFish[i].idxCase = randomPattern;
					PatternIdx[i] = ArenaObj.allFish[i].idxCase;
				}
				screenObj.refresh(PatternIdx);
			}

			if ((timerObj.getCount() + 1) == (subTrialTime * frameRate) || (timerObj.getCount() + 1) == (subTrialTime * frameRate) * 2) {
				randomPattern = !(randomPattern);
				for (int i = 0; i < ArenaObj.numFish; i++) {
					ArenaObj.allFish[i].idxCase = randomPattern;
					PatternIdx[i] = ArenaObj.allFish[i].idxCase;
				}
				screenObj.refresh(PatternIdx);
			}

			if ((timerObj.getCount() + 1) == trialTime * frameRate) {

				for (int i = 0; i < ArenaObj.numFish; i++) {
					ArenaObj.allFish[i].idxCase = blackPatch;
					PatternIdx[i] = ArenaObj.allFish[i].idxCase;
				}
				screenObj.refresh(PatternIdx);
			}

			cameraObj.grabPylonImg();

			ArenaObj.getImgFromCamera(cameraObj.frameWidth, cameraObj.frameHeight, (uint8_t*)cameraObj.getPtr2buffer());
			ArenaObj.buildBgImg();
			ArenaObj.findAllFish(ArenaObj.corner_x, ArenaObj.corner_y, ArenaObj.width, ArenaObj.height);

			fileWriterObj.writeOutFrame(ArenaObj, 0);
			fileWriterObj.saveFrame(timerObj, ArenaObj, 0, trialCount);

			if (timerObj.getCount() % 10 == 0) {
				ArenaObj.displayImg("Live");
			}
		}
		cout << trialCount << endl;
	}
	fileWriterObj.writeOutData(UIobj.baseNames);

	cout << "Experiment ended. " << endl;
	//cv::waitKey();
}

void Experiment::runAntiOLexp() {
	const int prepareTime = 1 * 60; // seconnds, default 1 min
	//const int testTime = 1 * 60;
	const int baselineEndTime = 10 * 60; // seconds, default 10 mins
	const int trainingEndTime = 20 * 60; // seconds, default 20 mins
	const int blackoutEndTime = 1 * 60; // seconds, default 1 min
	const int testEndTime = 18 * 60; // seconds, default 18 mins (including memory extinction period)
	const int expEndTime = testEndTime;

	int trialCount = 0;
	bool randomPattern;
	const int trialTime = 60;// seconds
	const int ITI = 20; // seconds, inter-trial interval, TODO: implement the concept of trials
	const int baselineTrial = 6;
	const int trainingTrial = 24;
	const int testTrial = 12;
	const int frameRate = 50;

	const int baselineInterval = 30; // seconds
	const int testInterval = 30; // seconds
	// Preparation before experiment starts
	// TODO: write the implementations
	for (timerObj.resetCount(); timerObj.getCount() < cameraObj.time2IdxFrame(prepareTime); timerObj.addCount()) // TODO: write a macro to encapsulate this `for`
	{
		cameraObj.grabPylonImg(); // TODO: update the return type


		ArenaObj.getImgFromCamera(cameraObj.frameWidth, cameraObj.frameHeight, (uint8_t*)cameraObj.getPtr2buffer());

		ArenaObj.buildBgImg();

	}

	timerObj.start();
	bool a[8] = { 1,0,0,0,0,0,0,0 };
	vector<int> IdxCase3 = { 2,2,2,2 };
	vector<bool> channels2open;
	for (auto i : a) {
		channels2open.push_back(i);
	}
	cv::namedWindow("Live");


	// baseline
	for (; trialCount < baselineTrial; trialCount++) {
		for (timerObj.resetCount(), randomPattern = (std::rand() % 2); timerObj.getCount() < (ITI + trialTime) * frameRate; timerObj.addCount()) {
			timerObj.getTime();
			//refresh screen
			if (timerObj.getCount() == 0) {

				ArenaObj.allFish[0].idxCase = randomPattern;
				IdxCase3[2] = ArenaObj.allFish[0].idxCase;
				screenObj.refresh(IdxCase3);

			}
			if ((timerObj.getCount() + 1) == (20 * frameRate) || (timerObj.getCount() + 1) == (20 * frameRate) * 2) {
				randomPattern = !(randomPattern);
				ArenaObj.allFish[0].idxCase = randomPattern;
				IdxCase3[2] = ArenaObj.allFish[0].idxCase;
				screenObj.refresh(IdxCase3);
			}
			if ((timerObj.getCount() + 1) == trialTime * frameRate) {

				ArenaObj.allFish[0].idxCase = blackPatch;
				IdxCase3[2] = ArenaObj.allFish[0].idxCase;
				screenObj.refresh(IdxCase3);
			}


			cameraObj.grabPylonImg();

			ArenaObj.getImgFromCamera(cameraObj.frameWidth, cameraObj.frameHeight, (uint8_t*)cameraObj.getPtr2buffer());
			//ArenaObj.alignImg(270);
			ArenaObj.buildBgImg();
			ArenaObj.findSingleFish();
			ArenaObj.annotateFish();
			fileWriterObj.writeOutFrame(timerObj, ArenaObj, 0, trialCount);
			if (timerObj.getCount() % 50 == 0) {
				ArenaObj.displayImg("Live");


			}
			//ArenaObj.displayImg("Live");
			//cout << timerObj.getCount() << endl;

		}
		cout << trialCount << endl;
	}

	for (; trialCount < baselineTrial + trainingTrial; trialCount++) {
		for (timerObj.resetCount(), randomPattern = (std::rand() % 2); timerObj.getCount() < (ITI + trialTime) * frameRate; timerObj.addCount()) {
			timerObj.getTime();
			//refresh screen
			if (timerObj.getCount() == 0) {

				ArenaObj.allFish[0].idxCase = randomPattern;
				IdxCase3[2] = ArenaObj.allFish[0].idxCase;
				screenObj.refresh(IdxCase3);
				ArenaObj.allFish[0].lastTimeUpdatePattern = timerObj.sElapsed;
			}
			if ((timerObj.getCount() + 1) == (20 * frameRate) || (timerObj.getCount() + 1) == (20 * frameRate) * 2) {
				randomPattern = !(randomPattern);
				ArenaObj.allFish[0].idxCase = randomPattern;
				IdxCase3[2] = ArenaObj.allFish[0].idxCase;
				screenObj.refresh(IdxCase3);
				ArenaObj.allFish[0].lastTimeUpdatePattern = timerObj.sElapsed;
			}
			if ((timerObj.getCount() + 1) == trialTime * frameRate) {

				ArenaObj.allFish[0].idxCase = blackPatch;
				IdxCase3[2] = ArenaObj.allFish[0].idxCase;
				screenObj.refresh(IdxCase3);
				ArenaObj.allFish[0].lastTimeUpdatePattern = timerObj.sElapsed;
			}


			cameraObj.grabPylonImg();

			ArenaObj.getImgFromCamera(cameraObj.frameWidth, cameraObj.frameHeight, (uint8_t*)cameraObj.getPtr2buffer());
			//ArenaObj.alignImg(270);
			ArenaObj.buildBgImg();
			ArenaObj.findSingleFish();
			if (ArenaObj.allFish[0].checkIfGiveShockForAnti(timerObj.sElapsed)) {
				relayObj.givePulse(channels2open);
			}



			ArenaObj.annotateFish();
			fileWriterObj.writeOutFrame(timerObj, ArenaObj, 0, trialCount);

			if (timerObj.getCount() % 50 == 0) {
				ArenaObj.displayImg("Live");


			}
			//ArenaObj.displayImg("Live");
			//cout << timerObj.getCount() << endl;

		}
		cout << trialCount << endl;
	}

	for (; trialCount < baselineTrial + trainingTrial + testTrial; trialCount++) {
		for (timerObj.resetCount(), randomPattern = (std::rand() % 2); timerObj.getCount() < (ITI + trialTime) * frameRate; timerObj.addCount()) {
			timerObj.getTime();
			//refresh screen
			if (timerObj.getCount() == 0) {

				ArenaObj.allFish[0].idxCase = randomPattern;
				IdxCase3[2] = ArenaObj.allFish[0].idxCase;
				screenObj.refresh(IdxCase3);

			}
			if ((timerObj.getCount() + 1) == (20 * frameRate) || (timerObj.getCount() + 1) == (20 * frameRate) * 2) {
				randomPattern = !(randomPattern);
				ArenaObj.allFish[0].idxCase = randomPattern;
				IdxCase3[2] = ArenaObj.allFish[0].idxCase;
				screenObj.refresh(IdxCase3);
			}
			if ((timerObj.getCount() + 1) == trialTime * frameRate) {

				ArenaObj.allFish[0].idxCase = blackPatch;
				IdxCase3[2] = ArenaObj.allFish[0].idxCase;
				screenObj.refresh(IdxCase3);
			}


			cameraObj.grabPylonImg();

			ArenaObj.getImgFromCamera(cameraObj.frameWidth, cameraObj.frameHeight, (uint8_t*)cameraObj.getPtr2buffer());
			//ArenaObj.alignImg(270);
			ArenaObj.buildBgImg();
			ArenaObj.findSingleFish();
			ArenaObj.annotateFish();
			fileWriterObj.writeOutFrame(timerObj, ArenaObj, 0, trialCount);

			if (timerObj.getCount() % 50 == 0) {
				ArenaObj.displayImg("Live");


			}
			//ArenaObj.displayImg("Live");
			//cout << timerObj.getCount() << endl;

		}
		cout << trialCount << endl;
	}

	cout << "Experiment ended. " << endl;
}

void Experiment::videoclip() {
	cv::VideoCapture cap("F:/FishExpData/20200903_2121_Arena2_GCaMP6s_7dpf_OLexp.avi");

	cap.set(CV_CAP_PROP_POS_FRAMES, 33860);
	Mat frame;
	string videoName = "F:/FishExpData/20200903_2121_Arena2_GCaMP6s_7dpf_OLexp_clip.avi";
	cv::VideoWriter vObj(videoName, CV_FOURCC('D', 'I', 'V', 'X'), 50, cv::Size(800,500), true);


	tryCatchFalse(vObj.isOpened(), "Video file writer CANNOT be opened!");
	for (int i = 0; i < 1500; i++) {
		cap>>frame;

		vObj << frame;
	}
	////AObj.getImgFromVideo(cap);
	////AObj.alignImg(270);
	//vector<int> xDivs = { 1376 };
	//vector<string> fishIDs = { "test1" };
}

cv::Mat  img_setROI, img_setThreshold;
cv::Point cur_ROIPt;

int threshold_val = 22;

void on_mouse_setROI(int event, int x, int y, int flags, void* ustc)
{
	if (event == CV_EVENT_LBUTTONDBLCLK) {
		cur_ROIPt = Point(x, y);
		circle(img_setROI, cur_ROIPt, 2, Scalar(255, 0, 0, 0), CV_FILLED, CV_AA, 0);
		imshow("setROI", img_setROI);
		cout << cur_ROIPt.x << ',' << cur_ROIPt.y << endl;
	}
}

static void on_trackbar_setThreshold(int, void*) {
	int max_val = 255;
	Mat binaryzation = Mat::zeros(img_setThreshold.size(), CV_8UC1);
	threshold(img_setThreshold, binaryzation, threshold_val, max_val, CV_THRESH_BINARY);

	imshow("setThreshold", binaryzation);

}

bool setThreshold() {
	namedWindow("setThreshold", CV_WINDOW_NORMAL);
	cv::createTrackbar("Threshold", "setThreshold", &threshold_val, 255, on_trackbar_setThreshold);
	on_trackbar_setThreshold(threshold_val, 0);
	cout << "Press 'q' to exit." << endl;
	while (char(cv::waitKey(1)) != 'q') {}
	return true;
}



void Experiment::ShadowDetection() {
	cv::Ptr<cv::BackgroundSubtractor> pMOG = cv::createBackgroundSubtractorMOG2(600, 16, false);
	cv::VideoCapture cap("F:/FishExpData/20200928_1910_Arena1_g6f_20dpf_OLexp.avi");
	cv::Mat opencvImg, subImg,maskedImg,closedImg;
	cv::Mat rawImg;

	for (int i = 0; i < 600; i++) {

		cap >> rawImg;
		cv::cvtColor(rawImg, opencvImg, cv::COLOR_RGB2GRAY);
		pMOG->apply(opencvImg, subImg);
	}

	cv::namedWindow("Live");
	for (int i = 0; i < 3000; i++) {
		cap >> rawImg;
		cv::cvtColor(rawImg, opencvImg, cv::COLOR_RGB2GRAY);

		pMOG->apply(opencvImg, subImg);
		maskedImg = cv::Mat::zeros(cv::Size(1640, 582), CV_8UC1);
		opencvImg.copyTo(maskedImg, subImg);
		cv::Point BrightPt;

		//cv::minMaxLoc(maskedImg, NULL, NULL, NULL, &BrightPt);
		//cv::floodFill(maskedImg, BrightPt, Scalar(0), 0, Scalar(10), Scalar(10));
		cv::morphologyEx(subImg, closedImg, MORPH_CLOSE,NULL);

		//vector<vector<Point>> contours;
		//maskedImg.copyTo(img_setThreshold);
		//setThreshold();
		//Mat binaryzation = Mat::zeros(img_setThreshold.size(), CV_8UC1);
		//cv::threshold(maskedImg, binaryzation, threshold_val, 255, CV_THRESH_BINARY);
		//cv::imshow("Live", binaryzation);
		//cv::waitKey();
		//cout << binaryzation.type() << endl;
		//cv::waitKey();
		//findContours(binaryzation, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
		//cv::drawContours(maskedImg, contours, -1, cv::Scalar::all(255));
		//for (auto& contour : contours) {
		//	cout << contour.size() << ';';
		//}

		cv::imshow("Live", closedImg);
		cv::waitKey(10);
	}
}




void Experiment::test2() {
	vector<int> xDivs = { 410 };
	vector<string> fishIDs = { "test1" };
	UIobj.enquireInfoFromUser();
	cameraObj.initialize();
	ArenaObj.initialize(xDivs, fishIDs);
	screenObj.initialize();

	fileWriterObj.initialize(UIobj.baseNames, cv::Size(cameraObj.frameWidth, cameraObj.frameHeight), cameraObj.frameRate);
	for (int i = 0; i < 3600; i++) {
		cameraObj.grabPylonImg(); // TODO: update the return type


		ArenaObj.getImgFromCamera(cameraObj.frameWidth, cameraObj.frameHeight, (uint8_t*)cameraObj.getPtr2buffer());

		fileWriterObj.writeOutFrame(ArenaObj,0);
	}


}


void Experiment::test1() {
	vector<int> xDivs = { 208 ,574,208,574};
	vector<string> fishIDs = { "test1","test2","test3","test4" };

	UIobj.enquireInfoFromUser();
	//ArenaObj(389, 370, 4);
	ArenaObj.initialize(xDivs, fishIDs);
	//screenObj.initialize();
	cameraObj.initialize();

	//fileWriterObj.initialize(UIobj.baseNames, cv::Size(784, 784), 10, 10000,4);


	fileWriterObj.writeOutExpSettings(UIobj, cameraObj, ArenaObj, UIobj.baseNames);
	cv::VideoCapture cap("F:/fishData/20181015_1238_Arena1_WT_7dpf_OLcontrol_RGB32.avi");

	const int prepareTime = 1 * 60; // seconnds, default 1 min
	//const int testTime = 1 * 60;
	const int baselineEndTime = 10 * 60; // seconds, default 10 mins
	const int trainingEndTime = 20 * 60; // seconds, default 20 mins
	const int blackoutEndTime = 1 * 60; // seconds, default 1 min
	const int testEndTime = 18 * 60; // seconds, default 18 mins (including memory extinction period)
	const int expEndTime = testEndTime;

	int trialCount = 0;
	bool randomPattern;
	const int trialTime = 80;// seconds
	const int ITI = 20; // seconds, inter-trial interval, TODO: implement the concept of trials
	const int baselineTrial = 10;
	const int trainingTrial = 24;
	const int testTrial = 12;
	const int frameRate = 50;

	const int baselineInterval = 30; // seconds
	const int testInterval = 30; // seconds


	for (timerObj.resetCount(); timerObj.getCount() < 2000; timerObj.addCount()) // TODO: write a macro to encapsulate this `for`
	{
		//cameraObj.grabPylonImg(); // TODO: update the return type
		ArenaObj.getImgFromVideo(cap);

		//ArenaObj.getImgFromCamera(cameraObj.frameWidth, cameraObj.frameHeight, (uint8_t*)cameraObj.getPtr2buffer());

		ArenaObj.buildBgImg();

	}

	timerObj.start();
	bool a[8] = { 1,0,0,0,0,0,0,0 };
	vector<int> IdxCase3 = { 2,2,2,2 };
	vector<bool> channels2open;
	for (auto i : a) {
		channels2open.push_back(i);
	}
	cv::namedWindow("Live");


	// baseline
	for (; trialCount < baselineTrial; trialCount++) {
		for (timerObj.resetCount(), randomPattern = (std::rand() % 2); timerObj.getCount() < 1000; timerObj.addCount()) {
			timerObj.getTime();
			//refresh screen


			ArenaObj.getImgFromVideo(cap);

			//ArenaObj.getImgFromCamera(cameraObj.frameWidth, cameraObj.frameHeight, (uint8_t*)cameraObj.getPtr2buffer());
			//ArenaObj.alignImg(270);
			ArenaObj.buildBgImg();
			ArenaObj.findAllFish();
			//ArenaObj.annotateFish();
			//fileWriterObj.writeOutFrame(ArenaObj,0);
			fileWriterObj.saveFrame(timerObj, ArenaObj, 0, trialCount);

			//if (timerObj.getCount() % 50 == 0) {
			//	ArenaObj.displayImg("Live");
			//	//cv::waitKey();

			//}
			//ArenaObj.displayImg("Live");
			//cout << timerObj.getCount() << endl;

		}
		cout << trialCount << endl;
	}

	fileWriterObj.writeOutData(UIobj.baseNames);
	cv::waitKey();

}
//void Experiment::runOLexp()
//{
//	const int prepareTime = 1 * 60; // seconnds, default 1 min
//	const int baselineEndTime = 2 * 60; // seconds, default 10 mins
//	const int trainingEndTime = 8 * 60; // seconds, default 20 mins
//	const int blackoutEndTime = 9 * 60; // seconds, default 1 min
//	const int testEndTime = 15 * 60; // seconds, default 18 mins (including memory extinction period)
//	const int expEndTime = testEndTime;
//
//	int ITI = 10; // seconds, inter-trial interval, TODO: implement the concept of trials
//	const int baselineInterval = 30; // seconds
//	const int testInterval = 30; // seconds
//
//	// Preparation before experiment starts
//	// TODO: write the implementations
//	for (timerObj.resetCount(); timerObj.getCount() < cameraObj.time2IdxFrame(prepareTime); timerObj.addCount()) // TODO: write a macro to encapsulate this `for`
//	{
//		camerasObj.grabPylonImg(); // TODO: update the return type
//		int cIdx = camerasObj.cIdx;
//		fishAnalysisObj.aIdx = cIdx;
//
//		fishAnalysisObj.getImgFromCamera(camerasObj.frameWidth, camerasObj.frameHeight, (uint8_t*)camerasObj.getPtr2buffer());
//		// TODO: include the following two lines in the method "preprocessImg"
//		fishAnalysisObj.alignImg();
//		fishAnalysisObj.buildBgImg();
//
//	}
//
//	timerObj.start(); //TODO, reset seconds and count
//
//	for (timerObj.resetCount(); timerObj.getCount() < camerasObj.time2IdxFrame(prepareTime); timerObj.addCount())
//	{
//		timerObj.getTime();
//		camerasObj.grabPylonImg(); // grabbing
//		int cIdx = camerasObj.cIdx;
//		fishAnalysisObj.aIdx = cIdx;
//
//		fishAnalysisObj.getImgFromCamera(camerasObj.frameWidth, camerasObj.frameHeight, (uint8_t*)camerasObj.getPtr2buffer());
//		// TODO: include the following two lines in the method "preprocessImg"
//		fishAnalysisObj.alignImg();
//		fishAnalysisObj.buildBgImg();
//
//		fishAnalysisObj.findAllFish();
//
//		// run baseline session
//		if (timerObj.getCount() < camerasObj.time2IdxFrame(baselineEndTime))
//		{
//			timerObj.expPhase = 0;
//			screenObj.reverse(timerObj.sElapsed, baselineInterval);
//		}
//		else if (timerObj.getCount() < camerasObj.time2IdxFrame(trainingEndTime))
//		{
//			timerObj.expPhase = 1;
//			vector<vector<bool>> fish2shock = fishAnalysisObj.checkWhich2GiveShock(timerObj.sElapsed);
//			// TODO: map 2D fish2shock to 1D
//			vector<bool> channels2open;
//			for (vector<bool> v : fish2shock)
//				for (auto i : v)
//					channels2open.push_back(i);
//
//			relayObj.givePulse(channels2open);
//			// update pattern indices based on shock status, fish positions, and current pattern
//			vector<vector<bool>> fish2reversePattern = fishAnalysisObj.checkWhich2ReversePattern(timerObj.sElapsed); // TODO: is there a better place to put this method?
//			screenObj.reverse(fish2reversePattern);
//		}
//		else if (timerObj.getCount() < camerasObj.time2IdxFrame(trainingEndTime))
//		{
//			timerObj.expPhase = 3;
//			screenObj.reverse(timerObj.sElapsed - trainingEndTime, testInterval);
//		}
//		//fileWriterObj.writeOutFrame(timerObj, fishAnalysisObj, cIdx);
//		fishAnalysisObj.annotateImg();
//		fishAnalysisObj.displayImg("Live");
//
//	}
//	cout << "Experiment ended. " << endl;
//
//}
//
//void Experiment::runUnpairedOLexp()
//{
//	const int prepareTime = 1 * 60; // seconnds, default 1 min
//	const int baselineEndTime = 1 * 60; // seconds, default 10 mins
//	const int trainingEndTime = 8 * 60; // seconds, default 20 mins
//	const int blackoutEndTime = 8 * 60; // seconds, default 1 min
//	const int testEndTime = 10 * 60; // seconds, default 18 mins (including memory extinction period)
//	const int expEndTime = testEndTime;
//	const int baselineInterval = 30; // seconds
//	const int testInterval = 30; // seconds
//
//	const int numShocks = 20; // comparable to shocks fish received in the normal OLexp group
//
//
//	// write a function to sample frames2giveShock
//	vector<bool> frames2giveShock = RNGsampleFrames2giveShock(numShocks, trainingEndTime - baselineEndTime);
//	for (timerObj.resetCount(); timerObj.getCount() < camerasObj.time2IdxFrame(prepareTime); timerObj.addCount()) // TODO: write a macro to encapsulate this `for`
//	{
//		camerasObj.grabPylonImg(); // TODO: update the return type
//		int cIdx = camerasObj.cIdx;
//		fishAnalysisObj.aIdx = cIdx;
//
//		fishAnalysisObj.getImgFromCamera(camerasObj.frameWidth, camerasObj.frameHeight, (uint8_t*)camerasObj.getPtr2buffer());
//		// TODO: include the following two lines in the method "preprocessImg"
//		fishAnalysisObj.alignImg();
//		fishAnalysisObj.buildBgImg();
//
//	}
//	timerObj.start();
//
//	for (timerObj.resetCount(); timerObj.getCount() < camerasObj.time2IdxFrame(prepareTime); timerObj.addCount())
//	{
//		timerObj.getTime();
//		camerasObj.grabPylonImg(); // grabbing
//		int cIdx = camerasObj.cIdx;
//		fishAnalysisObj.aIdx = cIdx;
//
//		fishAnalysisObj.getImgFromCamera(camerasObj.frameWidth, camerasObj.frameHeight, (uint8_t*)camerasObj.getPtr2buffer());
//		// TODO: include the following two lines in the method "preprocessImg"
//		fishAnalysisObj.alignImg();
//		fishAnalysisObj.buildBgImg();
//
//		fishAnalysisObj.findAllFish();
//
//		// run baseline session
//		if (timerObj.getCount() < camerasObj.time2IdxFrame(baselineEndTime))
//		{
//			timerObj.expPhase = 0;
//			screenObj.reverse(timerObj.sElapsed, baselineInterval);
//		}
//		else if (timerObj.getCount() < camerasObj.time2IdxFrame(trainingEndTime))
//		{
//			timerObj.expPhase = 1;
//			vector<bool> channels2open;
//			if (frames2giveShock[timerObj.getCount() - camerasObj.time2IdxFrame(baselineEndTime)])
//				channels2open.resize(12, true);//TODO: the magic number means max number of channels, consider to have this variable in the Relay class
//			else
//				channels2open.resize(12, false);
//
//			relayObj.givePulse(channels2open);
//			// update pattern indices based on shock status, fish positions, and current pattern
//			vector<vector<bool>> fish2reversePattern = fishAnalysisObj.checkWhich2ReversePattern(timerObj.sElapsed); // TODO: is there a better place to put this method?
//			screenObj.reverse(fish2reversePattern);
//		}
//		else if (timerObj.getCount() < camerasObj.time2IdxFrame(trainingEndTime))
//		{
//			timerObj.expPhase = 3;
//			screenObj.reverse(timerObj.sElapsed - trainingEndTime, testInterval);
//		}
//		//fileWriterObj.writeOutFrame(timerObj, fishAnalysisObj, cIdx);
//		fishAnalysisObj.annotateImg();
//		fishAnalysisObj.displayImg("Live");
//
//	}
//	cout << "Experiment ended. " << endl;
//}
//
//vector<bool> Experiment::RNGsampleFrames2giveShock(int numShocks, int sampleDuration, int startTime)
//{
//	// First create an instance of an engine.
//	random_device rnd_device;
//	// Specify the engine and distribution.
//	mt19937 g(rnd_device());
//	vector<int> vec(sampleDuration);
//	iota(vec.begin(), vec.end(), 1);
//
//	shuffle(vec.begin(), vec.end(), g);
//	vector<int> rndVec(vec.begin(), vec.begin() + numShocks);
//
//	for (auto& i : rndVec)
//		i += startTime;
//
//	int frameRate = cameraObj.frameRate;
//	int numCameras = UIobj.numOpenCameras;
//	vector<bool> frames2giveShock(sampleDuration * cameraObj.frameRate);
//	for (auto& i : rndVec)
//		fill(frames2giveShock.begin() + frameRate * i, frames2giveShock.begin() + frameRate * i + numCameras, true);
//
//	return frames2giveShock;
//
//
//}

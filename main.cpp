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
* Filename: main.cpp
* Abstract: this file contains all functions used in constructing final
*			behavioral learning experiment in zebrafish
*
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: Apr. 28, 2018
* Replaced Version: 1.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
*/
// Include 3rd party libraries
#include "3rdPartyLibs/Utilities/Timer.h"

// Include user-defined libraries
#include "MyLibs/experiment.h"
#include "MyLibs/talk2screen.h"
#include "MyLibs/talk2camera.h"
#include <ctime>


// Include standard libraries
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	/* Get current date and time string from chrono system clock */
	// Get system time
	time_t rawtime;
	struct tm timeinfo;
	char buffer[80];

	time(&rawtime);

	int errCode = localtime_s(&timeinfo, &rawtime);
	strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M", &timeinfo);
	string timeStr = buffer;


	int numCameras;
	cout << "Number of cameras to use? (3, 2, 1)" << endl;
	cin >> numCameras;
	cout << endl; // separated with an empty line

	int expMins;
	cout << "How many minutes to record? (30 mins?)" << endl;
	cin >> expMins;
	cout << endl; // separated with an empty line

	int fishAge;
	cout << "Enter age for all fish. (A number, e.g. 9)" << endl;
	cout << "(Assume they are at the same age)" << endl;
	cin >> fishAge;
	cout << endl; // separated with an empty line
	
	string strainName;
	cout << "Enter the strain name for all fish. (A number, e.g. GCaMP6f)" << endl;
	cout << "(Assume they are the same strain)" << endl;
	cin >> strainName;

	string expTask;
	cout << "Enter task for all fish (e.g. fishScreen)" << endl;
	cin >> expTask;

	vector<string> fileNames(2);
	fileNames[0] = timeStr + "_" + "Arena" + to_string(1)
		+ "_" + strainName + "_"
		+ to_string(fishAge) + "dpf_"
		+ expTask;
	
	fileNames[1] = timeStr + "_" + "Arena" + to_string(2)
		+ "_" + strainName + "_"
		+ to_string(fishAge) + "dpf_"
		+ expTask;



	// string pathName = "F:/cym/PreyCaptureScreen/";
	string pathName = "E:/Fish Data/"; 
	vector<string> videoNames(2);
	videoNames[0] = pathName + fileNames[0] + ".avi";
	videoNames[1] = pathName + fileNames[1] + ".avi";

	CameraData cams;

	int expTime = expMins * 60; // seconds
	int frameWidth = 600;//800;
	int frameHeight = 600;//750;
	int frameRate = 140; 
	cams.initialize(numCameras, frameWidth, frameHeight, frameRate);

	// initialize the output videos
	vector<VideoWriter> videoVec;
	for (int i = 0; i < numCameras; i++)
	{
		VideoWriter vObj(videoNames[i], CV_FOURCC('D', 'I', 'V', 'X'), frameRate, Size(frameWidth, frameHeight), false);
		videoVec.push_back(vObj);
	}
	

	vector<Mat> imgVec(2);
	int fIdx = 0; // time index
	while (fIdx < expTime * frameRate * numCameras)
	{
		fIdx++;
		int timeInSec = fIdx / (frameRate * numCameras);
		cams.grabPylonImg();

		cout << "Recording... " << timeInSec << " in " << expTime << " s" << endl;

		int cIdx = cams.cIdx;

		/* Convert Pylon image to opencvImg */
		Mat rawImg = Mat(cams.ptrGrabResult->GetHeight(), cams.ptrGrabResult->GetWidth()
			, CV_8UC1, (uint8_t*)cams.pylonImg.GetBuffer());
		
		if (cIdx == 0)
		{
			rawImg.copyTo(imgVec[0]);
			videoVec[0] << imgVec[0];
		}
		else if (cIdx == 1)
		{	
			Mat temp, dst;
			flip(rawImg, temp, 0);
			transpose(temp, dst);
			dst.copyTo(imgVec[1]);
			videoVec[1] << imgVec[1];
		}
			



		if (fIdx > 2)
		{ 
			/*Display videos*/
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

			w = 2; h = 1;
			size = 300;

			// Create a new 1 channel image
			Mat DispImage = Mat::zeros(Size(100 + size * w, 60 + size * h), CV_8UC1);
			for (i = 0, m = 20, n = 20; i < numCameras; i++, m += (20 + size)) {
				// Get the Pointer to the IplImage
				Mat img = imgVec[i];
				
				if (img.empty()) {
					std::cout << "Invalid arguments" << std::endl;
			
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
				Mat temp; 
				resize(img, temp, Size(ROI.width, ROI.height));
				temp.copyTo(DispImage(ROI));
			}
			// Create a new window, and show the Single Big Image
			//namedWindow("Display", 1);
			//imshow("Display", DispImage);
			//waitKey(1);
		}
	}

	
	
	/*TODO:
	1. degrade load_image_to_buffers to a method of AreaData
	2. put all positions and patterns coordinates into ScreenData
	3. do a similar update to the FishData class.
	4. reduce unnecessary data-type conversions
	5. remove unnecessary code
	*/
	//vector<const char*> CS_patterns =
	//{
	//	"Images/redBlackCheckerboard.jpg",
	//	"Images/whiteBlackCheckerboard.jpg",
	//	"Images/fullBlue.jpg"
	//};
	/*
	Mat I = imread("Images/fullBlue.jpg");

	namedWindow("Display", 1);
	imshow("Display", I);
	waitKey(0);
	*/


	/* Blue test 
	string pathName = "F:/FishExpData/";
	ExperimentData exp(pathName);

	if (!exp.initialize())
	{
		cout << "Experiment Initialization Failed." << endl;
		exit(0);
	}
	else {
		cout << "Experiment initialized." << endl;
	}

	exp.runBlueTest();
	*/






	/* The block for the real experiment 
	string pathName = "F:/FishExpData/";
	vector<string> CS_Patterns;
	CS_Patterns.push_back("redBlackCheckerboard");
	CS_Patterns.push_back("whiteBlackCheckerboard");
	CS_Patterns.push_back("fullBlue");
	//string CS_Pattern = "redBlackCheckerboard";
	ExperimentData exp(CS_Patterns,pathName);

	if (!exp.initialize())
	{
		cout << "Experiment Initialization Failed." << endl;
		exit(0);
	}
	else {
		cout << "Experiment initialized." << endl;
	}

	exp.runOLexp();
	*/


	/* 
	string CS_Pattern = "RGB96";
	ExperimentData exp(CS_Pattern);

	if (!exp.initialize())
	{
	cout << "Experiment Initialization Failed." << endl;
	exit(0);
	}
	else {
	cout << "Experiment initialized." << endl;
	}

	exp.runOLexp();
	The real main function */

	/* Test serial port
	string CS_Pattern = "redBlackCheckerboard";
	ExperimentData exp(CS_Pattern);
	if (!exp.thePort.initialize(COM_NUM)) {
		cout << "Initialize failed" << endl;
		return false;
	}
	int flag;
	cout << endl; // separated with an empty line
	for (int i = 0; i < 12; i++)
	{
		flag = exp.thePort.givePulse(i);
		waitKey(100);
		cout << "Give shock:" << i << endl;
		if (flag = 0)
		{
			cout << "Failed" << endl;
		}
	}
	*/


	/* main function
	string CS_Pattern = "redBlackCheckerboard";
	ExperimentData exp(CS_Pattern);

	if (!exp.initialize())
	{
		cout << "Experiment Initialization Failed." << endl;
		exit(0);
	}
	else {
		cout << "Experiment initialized." << endl;
	}

	exp.runOLexp();
	*/




	/* Test screen function
	Timer expTimer;
	ScreenData myScreen;
	vector<const char*> CSpatterns =
	{
		"Images/redBlackCheckerboard.jpg",
		"Images/whiteBlackCheckerboard.jpg",
		"Images/fullBlue.jpg"
	};

	myScreen.initialize(CSpatterns);

	expTimer.start();
	while (1)
	{
		int timeInSec = expTimer.getElapsedTimeInSec();
		cout << "Time (s) : " << timeInSec << endl;
		int areaIdx = rand() % 3;
		if (timeInSec % 10 == 0)
			myScreen.allAreas[areaIdx].allPatches[0].pIdx = !myScreen.allAreas[areaIdx].allPatches[0].pIdx;
		myScreen.allAreas[areaIdx].allPatches[0].updatePattern();
		myScreen.renderTexture();
	}

	*/




	/* Test OL Procedure
	ExperimentData exp;

	const char imgName[] = "Images/redCheckerBoard.jpg";
	try {
		exp.initialize(imgName);
		exp.prepareBgImg();
		exp.runOLexp();
	}
	catch (const GenericException &e)
	{
		// Error handling
		cerr << "An exception occurred." << endl
			<< e.GetDescription() << endl;
	}
	*/


	/*

	int testVar = 10;
	vector<int> headVec(4, 0);
	headVec[1] = 1;
	headVec[2] = 2;
	headVec[3] = 3;
	fs << "Frame" << "[";
	string vName; // variable name
	for (int i = 0; i < headVec.size(); i++)
	{
		vName = "Head" + to_string(i);
		fs << "{:" << vName << headVec[i] << "}";
	}
	fs << "]";
	*/
	//yaml << "Frames" << "[";
	//writeOutVarInline<int>(fs, testVar, "testVar");

	/* Timer.start can be used as reset
	Timer expTimer;
	expTimer.start();
	while (1)
	{
		int timeInSec = expTimer.getElapsedTimeInSec();
		cout << "Time (s): " << timeInSec << endl;
		if (timeInSec > 10)
			expTimer.start();
	}
	*/

	/* Test camera function

	Timer expTimer;
	expTimer.start();
	CameraData cams;
	cams.initialize();
	while (cams.grabPylonImg())
	{
		cout << "Time (s) : " << expTimer.getElapsedTimeInSec() << endl;
#ifdef PYLON_WIN_BUILD
		// Display the grabbed image.
		Pylon::DisplayImage(1, cams.ptrGrabResult);
#endif
	}
	*/







	// Enquire how many cameras to use
	// and enter filenames respectively

	/*
	try{ // Handle with missing frames of Pylon cameras
		ExperimentData myExp;
		myExp.initialize();
		myExp.prepareBgImg();
		myExp.runOLexp(); // run operant learning experiment
	}
	catch (const GenericException &e)
	{
		// Error handling
		cerr << "An exception occurred." << endl
			<< e.GetDescription() << endl;
	}
	*/
}

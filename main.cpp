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


// Include standard libraries
#include <iostream>

using namespace std;
using namespace cv;

int main()
{
	
	string pathName = "F:/forXuyan/";
	ExperimentData exp(pathName);

	if (!exp.initialize())
	{
		cout << "Experiment Initialization Failed." << endl;
	}
	else {
		cout << "Experiment initialized." << endl;
	}
	exp.monitorSponLocomotion();

	


	/* Test screen function

	ScreenData myScreen;
	vector<string> CSpatterns =
	{
	"Images/fullBlack.jpg",
	"Images/RBC.jpg",
	"Images/fullBlue.jpg"
	};
	myScreen.initialize(CSpatterns);
	while (1)
	{
	myScreen.renderTexture();
	}


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

	/*
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
	exp.runOLexp();
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

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
* Filename: experiment.h
* Abstract: this file contains all classes and function declarations 
*		used in constructing final behavioral learning experiment in zebrafish
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: Apr. 28, 2018

* Replaced Version: 1.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
*/

#ifndef _GUARD_EXPERIMENT_H
#define _GUARD_EXPERIMENT_H
// Include 3rd party libraries
#include "../3rdPartyLibs/Utilities/Timer.h"
// Include user-defined libraries
#include "fishAnalysis.h"
#include "talk2camera.h"
#include "talk2relay.h"
#include "talk2screen.h"
#include "writeOutFish.h"
// User-defined macros
#define COM_NUM 4
#define WIDTH 784 // frame width for all cameras and video files
#define HEIGHT 784 // frame height for all cameras and video files
#define FRAMERATE 10 // frame rate for all cameras to capture videos

class ExperimentData
{
private:
	/* Stands for different experiment phases */
	enum { baseline = 0, training = 1, blackout = 2, test = 3 };
	;// nothing for now
public:
	// methods
	ExperimentData(std::string texName)
		: CSpattern(texName)
	{
		numCameras = 1;

		idxFrame = -1;
		sElapsed = -1;
		msRemElapsed = 0;
		expPhase = -1;
		lastScreenPatternUpdate = -1;
		baselineInterval = 0;
		ITI = 0;
	}
	/* Initialize the experiment */
	bool initialize();
	/* Prepare background image for MOG subtractor */
	void prepareBgImg(const int prepareTime);
	/* Run unpaired training in the operant learning procedure */
	void runUnpairedOLexp();
	/* Run the entire operant learning procedure */
	void runOLexp();
	/* Update visual pattern in the baseline session */
	void updatePatternInBaseline();
	/* Update visual pattern in the training session */
	void updatePatternInTraining(int fishIdx);
	/* Update visual pattern in the test session */
	void updatePatternInTest();
	/* Decide whether to give shock to the fish */
	bool ifGiveShock(int fishIdx);
	/* Give the fish a electric pulse */
	void giveFishShock(int fishIdx);
	/* Write out info of a frame to disk */
	void writeOutFrame();
	/* Decorate images with fish's heads, tails and visual pattern's info */
	void annotateFishImgs();
	/* Present fish images with annotations. The code is adapted from code in stackfow*/
	void displayFishImgs(std::string title);
	

	//my function
	void ConvertPylonToOpencv(cv::Mat rawImg, int cIdx);

	// properties

	// constant ones
	const std::string CSpattern;
	int numCameras;
	

	int idxFrame;
	int sElapsed;
	int msRemElapsed;
	int expPhase;
	/* Update time for the entire screen */
	int lastScreenPatternUpdate;
	/* Interval for updating pattern in baseline session, which is a random number in range */
	int baselineInterval;
	/* Inter-trial Interval */
	int ITI;

	Timer expTimer;
	CameraData cams;
	std::vector<ArenaData> allArenas;
	ScreenData screen;
	PortData thePort;
	WriteOutData writeOut;

	cv::VideoCapture cap;



};


#endif _GUARD_EXPERIMENT_H

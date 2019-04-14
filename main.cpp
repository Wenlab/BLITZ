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


	// /// JUST FOR TEST
	Timer expTimer;
	ScreenData Board;
	vector<string> CSpattern = 
	{ "Images/WBC.jpg",
	  "Images/WBC.jpg",
	  "Images/WBC.jpg"
	};

	Board.initialize(CSpattern);

	expTimer.start();

	while (1)
	{
		int timeInSec = expTimer.getElapsedTimeInSec();
		cout << "Time (s) : " << timeInSec << endl;
		Board.getTheta(timeInSec * 10.0 / 180 * 3.14);
		Board.updatePattern();
		Board.renderTexture();
		
	}

}


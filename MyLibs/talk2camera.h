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
* Filename: talk2camera.h
* Abstract: this file contains all classes and functions' declarations
*			used in implementing Basler Pylon USB cameras
*
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: Apr. 28, 2018

* Replaced Version: 1.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
*/



#ifndef _GUARD_TALK2CAMERA_H
#define _GUARD_TALK2CAMERA_H

// Include user-defined libraries
#include "errorHandling.h"

// Include Basler Pylon libraries
#include <pylon/PylonIncludes.h>
#include <pylon/ImageFormatConverter.h>
#include <pylon/usb/BaslerUsbInstantCameraArray.h>
#ifdef PYLON_WIN_BUILD
#	include <pylon/PylonGUI.h>
#endif

// Include standard library
#include <vector>
#include <string>
#include <iostream>

// User-defined macros
#define MAX_CAMERAS 3
#define FRAMEWIDTH 800
#define FRAMEHEIGHT 600
#define FRAMERATE 10

/* Define Basler Pylon properties and related methods for multiple (or single) USB cameras */
class Cameras
{
private:
	;// nothing for now
public:
	// methods
	Cameras() // constructor
	{

	}

	// TODO: consider to allow users open any cameras combination
	// What is user input? ask the opening of each camera in order, middle -> left -> right
	// the first parameter should be a boolean array that corresponds to each camera status
	/* Initialize a single camera, device unspecific */
	void initialize()

	/* Initialize camera array objests */
	void initialize(std::vector<bool> cameras2open); // status array that indicate whether a camera should be open

	/* Grab Pylon image from cameras */
	void grabPylonImg();

	/* Get the pointer to image buffer, for a single camera */
	getPtr2buffer(); //TODO: find out the return type, write the implementation

	/* Get the pointer to image buffer, for a single camera */
	getPtr2buffer(int idxCamera); //TODO: find out the return type, write the implementation

	

	// properties
	Pylon::CBaslerUsbInstantCameraArray cameras;
	Pylon::IPylonDevice* aCam;// a single camera object
	Pylon::CGrabResultPtr  ptrGrabResult;
	Pylon::CPylonImage pylonImg;
	intptr_t cIdx;// index of camera where the frame is grabbed from

	// TODO: for developers, to have a private parameter list? (e.g., offsets, serialNums, ...)


};
// Global functions





#endif // !_GUARD_TALK2CAMERA_H

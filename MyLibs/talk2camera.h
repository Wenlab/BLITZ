/*
* Copyright 2019 Wenbin Yang <bysin7@gmail.com> (This project started from Jan., 2018.)
* This file is part of [BLITZ (Behavioral Learning In The Zebrafish)](https://github.com/Wenlab/BLITZ),
* which is adapted from MindControl (Andrew Leifer et al., 2011) <leifer@fas.harvard.edu>
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
* Current Version: 3.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 15, 2018
* Modified on: Apr. 20, 2019
*/

#ifndef _GUARD_TALK2CAMERA_H
#define _GUARD_TALK2CAMERA_H

// Include Basler Pylon libraries
#include <pylon/PylonIncludes.h>
#include <pylon/ImageFormatConverter.h>
#include <pylon/usb/BaslerUsbInstantCameraArray.h>
#ifdef PYLON_WIN_BUILD
#	include <pylon/PylonGUI.h>
#endif

// Include user-defined libraries
#include "errorHandling.h"

// Include standard library
#include <vector>
#include <string>
#include <iostream>

// User-defined macros
#define MAX_CAMERAS 3
#define FRAMEWIDTH 800
#define FRAMEHEIGHT 600
#define FRAMERATE 10

//TODO: do I need a general camera class for inheritances?
// Decide after writing up all implementations
/* Define Basler Pylon properties and related methods for multiple (or single) USB cameras */
class Cameras
{
private:
	// TODO: for developers, to have a private parameter list? (e.g., offsets, serialNums, ...)
	;// nothing for now
public:
	// methods
	Cameras() // constructor
	{

	}

	/* Initialize a single camera, device unspecific */
	void initialize();

	/* Initialize multiple cameras */
	void initialize(std::vector<bool> cameras2open); // status array that indicate whether a camera should be open

	/* Grab Pylon image from cameras */
	void grabPylonImg();

	/* Get the pointer to image buffer */
	getPtr2buffer(); //TODO: find out the return type, write the implementation

	/* Get the pointer to image buffer, for a single camera */
	getPtr2buffer(int idxCamera); //TODO: find out the return type, write the implementation


	// properties
	Pylon::CBaslerUsbInstantCameraArray cameras;
	Pylon::IPylonDevice* aCam;// a single camera object
	Pylon::CGrabResultPtr  ptrGrabResult;
	Pylon::CPylonImage pylonImg;
	intptr_t cIdx;// index of camera where the frame is grabbed from
};

/* Multiple USB cameras with a single pointer,
 	Basler Pylon provides threads control among cameras */
class MultiUSBCameras
{
public:
	MultiUSBCameras()
	{
		frameRate = 10;
		frameWidth = 800;
		frameHeight = 600;
		serialNums = { "21552672","22510229","22510230" };
		offSetXs = {463, 390, 944};
		offSetYs = {0, 0, 227};
	}
	/* Initialize multiple cameras, open all cameras by default */
	void initialize(); // TODO: implement this with the other overload method

	/* Initialize multiple cameras */
	void initialize(std::vector<bool> cameras2open); // status array that indicate whether a camera should be open

	/* Grab Pylon image from cameras */
	Pylon::CGrabResultPtr grabPylonImg();

	/* Get the pointer to image buffer */
	getPtr2buffer(); //TODO: find out the return type, write the implementation

	/* Get the pointer to image buffer, for a specific camera */
	getPtr2buffer(int idxCamera); //TODO: find out the return type, write the implementation

private:
	int frameRate;
	int frameWidth;
	int frameHeight;
	std::vector<std::string> serialNums;
	std::vector<int> offSetXs;
	std::vector<int> offSetYs;

	Pylon::CBaslerUsbInstantCameraArray cameras;
	Pylon::CGrabResultPtr  ptrGrabResult; //TODO: write a public method to get this variable or return it from an old method
	Pylon::CPylonImage pylonImg;// TODO: do I use this?
	intptr_t cIdx;// index of camera where the frame is grabbed from
}

/* Device unspecific single camera */
class SingleCamera
{
public:
	/* Initialize a single camera, device unspecific */
	void initialize();

	/* Grab Pylon image from camera */
	void grabPylonImg();

	/* Get the pointer to image buffer */
	getPtr2buffer(); //TODO: find out the return type, write the implementation

private:
	Pylon::IPylonDevice* cam;// a single camera object
	// TODO: finish this properties list
}

// Global functions





#endif // !_GUARD_TALK2CAMERA_H

#pragma once
#ifndef _GUARD_TALK2CAMERA_H
#define _GUARD_TALK2CAMERA_H

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

//opencv libs
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;
using namespace Pylon;

/* Device unspecific single camera */
class Camera
{
public:
	Camera()
	{
		frameRate = 140;
		exposureTime = 5000;
		frameWidth = 400;
		frameHeight = 400;
		offSetX = 0;
		offSetY = 0;
		pixelFormat = "Mono8";
	}

	/* Initialize a single camera, device unspecific */
	void initialize();

	/* Grab Pylon image from camera */
	bool grabPylonImg();

	/* Get the pointer to image buffer */
	//Pylon::CGrabResultPtr getPtr2buffer();
	cv::Mat getMat();

	int getFrameRate();
	cv::Size getFrameSize();

private:

	Pylon::CBaslerUsbInstantCamera cam;
	Pylon::CImageFormatConverter formatConverter;
	// TODO: finish this properties list
	float frameRate;
	int frameWidth;
	int frameHeight;
	int offSetX;
	int offSetY;
	int exposureTime = 5000;
	std::string pixelFormat;
	Pylon::CGrabResultPtr  ptrGrabResult;
	Pylon::CPylonImage pylonImg;
	cv::Mat opencvImg;
	
};






#endif // !_GUARD_TALK2CAMERA_H
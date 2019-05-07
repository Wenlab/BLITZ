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
* Abstract: this file contains all function declarations
*			used in implementing Basler Pylon USB cameras
*
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: Apr. 28, 2018

* Replaced Version: 1.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
* Modified on: May. 5, 2019
*/


// Include user-defined libraries
#include "talk2camera.h"

using namespace std;
using namespace Pylon;

void MultiUSBCameras::initialize()
{
	int maxCameras = 3;
	Pylon::PylonInitialize();
	Pylon::CTlFactory&TlFactory = Pylon::CTlFactory::GetInstance();


	cameras.Initialize(maxCameras);

	for (int i = 0; i < maxCameras; i++)
	{
		Pylon::CDeviceInfo di;
		di.SetDeviceClass(cameraType.c_str());
		// TODO: write in OO style
		di.SetSerialNumber(serialNums[i].c_str());

		// Attach devices and set camera parameters
		cameras[i].Attach(TlFactory.CreateDevice(di));
		cameras[i].Open();
		cameras[i].Width = frameWidth;
		cameras[i].Height = frameHeight;
		cameras[i].OffsetX = offSetXs[i];
		cameras[i].OffsetY = offSetYs[i];
		cameras[i].AcquisitionFrameRateEnable.SetValue(true);
		cameras[i].AcquisitionFrameRate.SetValue(frameRate);
		cameras[i].PixelFormat = pixelFormat.c_str();
		// Print the model name of the camera.
		std::cout << "Using camera " << cameras[i].GetDeviceInfo().GetSerialNumber() << std::endl;
	}

	cameras.StartGrabbing();

	cout << "Cameras initialization succeeded." << endl << endl;
}




void MultiUSBCameras::initialize(vector<bool> cameras2open)
{
	const int maxCameras = cameras2open.size();
	Pylon::PylonInitialize();
	Pylon::CTlFactory&TlFactory = Pylon::CTlFactory::GetInstance();

	// Make sure cameras are attached in order
	vector<int> matches;
	for (int i = 0; i < maxCameras; i++)
	{
		if (cameras2open[i])
			matches.push_back(i);
	}
	int numCameras2use = matches.size();
	cameras.Initialize(numCameras2use);

	for (int i = 0; i < numCameras2use; i++)
	{
		int idxCamera = matches[i];
		Pylon::CDeviceInfo di;
		di.SetDeviceClass(cameraType.c_str());
		// TODO: write in OO style
		di.SetSerialNumber(serialNums[idxCamera].c_str());

		// Attach devices and set camera parameters
		cameras[i].Attach(TlFactory.CreateDevice(di));
		cameras[i].Open();
		cameras[i].Width = frameWidth;
		cameras[i].Height = frameHeight;
		cameras[i].OffsetX = offSetXs[idxCamera];
		cameras[i].OffsetY = offSetYs[idxCamera];
		cameras[i].AcquisitionFrameRateEnable.SetValue(true);
		cameras[i].AcquisitionFrameRate.SetValue(frameRate);
		cameras[i].PixelFormat = pixelFormat.c_str();
		// Print the model name of the camera.
		std::cout << "Using camera " << cameras[i].GetDeviceInfo().GetSerialNumber() << std::endl;

	}

	cameras.StartGrabbing();

	cout << "Cameras initialization succeeded." << endl << endl;
}

void MultiUSBCameras::grabPylonImg()
{

	// Error handling
	try {
		cameras.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
		if (!ptrGrabResult->GrabSucceeded())
			cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
	}
	catch (const GenericException &e) {
		cerr << "An exception occurred." << endl
			<< e.GetDescription() << endl;

	}
	Pylon::CImageFormatConverter formatConverter;
	cIdx = ptrGrabResult->GetCameraContext();
	formatConverter.Convert(pylonImg, ptrGrabResult);

	tryCatchFalse(cameras.IsGrabbing(), "Error! The camera is NOT grabbing!");

	
}

void* MultiUSBCameras::getPtr2buffer()
{
	return pylonImg.GetBuffer();
}



void SingleCamera::initialize()
{
	
	PylonInitialize();
	// create an instant camera object
	cam.Attach(CTlFactory::GetInstance().CreateFirstDevice());
	// Print the model name of the camera.
	cout << "Using device " << cam.GetDeviceInfo().GetModelName() << endl;

	// Allow all the names in the namespace GenApi to be used without qualification.
	using namespace GenApi;

	// Get the camera control object.
	INodeMap &control = cam.GetNodeMap();

	// Get the parameters for setting the image area of interest (Image AOI).
	const CIntegerPtr camWidth = control.GetNode("Width");
	const CIntegerPtr camHeight = control.GetNode("Height");
	const CIntegerPtr camOffsetX = control.GetNode("OffsetX");
	const CIntegerPtr camOffsetY = control.GetNode("OffsetY");
	const CIntegerPtr camFrameRate = control.GetNode("FrameRate");// TODO: test this line

	// Set the parameters
	if (IsWritable(camOffsetX))
	camOffsetX->SetValue(offSetX);
	if (IsWritable(camOffsetY))
	camOffsetY->SetValue(offSetY);
	camWidth->SetValue(frameWidth);
	camHeight->SetValue(frameHeight);
	camFrameRate->SetValue(frameRate);

	// Set the pixel data format.
	CEnumerationPtr(control.GetNode("PixelFormat"))->FromString(pixelFormat.c_str());


	cam.StartGrabbing();
	cout << "Camera initialization succeeded." << endl << endl;
	
}
// TODO: rewrite with pylonImg
Pylon::CGrabResultPtr SingleCamera::grabPylonImg()
{
	// Error handling
	try {
		cam.RetrieveResult(5000, ptrGrabResult, TimeoutHandling_ThrowException);
		if (!ptrGrabResult->GrabSucceeded())
			cout << "Error: " << ptrGrabResult->GetErrorCode() << " " << ptrGrabResult->GetErrorDescription() << endl;
	}
	catch (const GenericException &e) {
		cerr << "An exception occurred." << endl
			<< e.GetDescription() << endl;
	}

	tryCatchFalse(cam.IsGrabbing(), "Error! The camera is NOT grabbing!");

	return ptrGrabResult;
}


Pylon::CGrabResultPtr SingleCamera::getPtr2buffer()
{
	return ptrGrabResult;
}
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
//#include "3rdPartyLibs/Utilities/Timer.h"

// Include user-defined libraries
#include "experiment_cv.h"

// Include standard libraries
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

using namespace std;
using namespace cv;

int main()
{
	Experiment test;
	test.initialize();
	test.runOLexp();

	//std::string  baseName = "t20200923_2133_Arena1_test_9dpf_OLexp3";

	//std::string  matname = "F:/FishExpData/"+ baseName+".mat";
	//const char* matName = matname.c_str();
	//const char* fileName = baseName.c_str();
	//MATFile* pmatFile = matOpen(matName, "w");
	////pMxArray = mxCreateNumericMatrix(arraySize, (4 + numFish * 9), mxINT32_CLASS, mxREAL);
	//const mwSize dims[] = { 3,2 };
	//mxArray* pMxArray = mxCreateNumericMatrix(2, 3, mxINT32_CLASS, mxREAL);
	////mxArray* pMxArray = mxCreateNumericArray(2, dims, mxINT32_CLASS, mxREAL);
	//int* a = new int[6];
	//for (int i = 0; i < 6; i++) {
	//	a[i] = i;
	//}

	//mxSetData(pMxArray, a);
	//matPutVariable(pmatFile, "output", pMxArray);
	//matClose(pmatFile);

	//Experiment testExp1;
	////testExp1.test1();
	//testExp1.ShadowDetection();
	//testExp1.videoclip();

	//UserInterface UIObj;
	//UIObj.enquireInfoFromUser();

	//for (; getchar() == 'q';) {}
	/*Arena AObj(1);
	cv::VideoCapture cap("F:/fishData/20181015_1238_Arena1_WT_7dpf_OLcontrol_RGB32.avi");
	AObj.getImgFromVideo(cap);*/
	//AObj.alignImg(270);
	//vector<int> xDivs = { 1376 };
	//vector<string> fishIDs = { "test1" };

	//AObj.initialize(xDivs, fishIDs);

	//for (int i = 0; i < 500; i++) {
	//	AObj.getImgFromVideo(cap);
	//	//AObj.alignImg(270);
	//	AObj.buildBgImg();

	//	//cv::imshow("test", AObj.subImg);
	//	//cv::waitKey(1);
	//}

	////img_setThreshold = AObj.subImg.clone();
	//cv::namedWindow("test", CV_WINDOW_NORMAL);
	////setThreshold();

	//for (int i = 0; i < 400; i++) {
	//	AObj.getImgFromVideo(cap);
	//	//AObj.alignImg(270);
	//	AObj.buildBgImg();
	//
	//	vector<vector<Point>> contours;
	//	findContours(AObj.subImg, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	//	int contourSizeThre = 1;
	//	int j = 0;
	//	for (auto& contour : contours)
	//	{
	//		int contourSize = contour.size();
	//		if (contourSize > contourSizeThre) {
	//			cv::drawContours(AObj.subImg, contours, j, cv::Scalar(0, 255, 0), 1);
	//			cout << contour.size() << endl;
	//		}
	//

	//		j++;
	//
	//	}
	//	cv::imshow("test", AObj.subImg);
	//	cv::waitKey();
	//	//cv::imshow("test", AObj.subImg);
	//	//cv::waitKey(1);
	//}


	//AObj.buildBgImg();

	//AObj.findAllFish();



	//
	//SingleCamera cameraObj;
	//cameraObj.initialize();


	//
	//Arena ArenaObj;
	////vector<int> xDivs = { 290 };

	////ArenaObj.initialize(xDivs, fishIDs);
	////vector<int> xDivs = { 290 };

	//cv::namedWindow("Live");

	//while (1) {
	//	cameraObj.grabPylonImg();
	//	ArenaObj.getImgFromCamera(cameraObj.frameWidth, cameraObj.frameHeight, (uint8_t*)cameraObj.getPtr2buffer());
	//
	//	ArenaObj.displayImg("Live");


	//
	//}
	//cv::Mat opencvImg;
	//cv::Mat rawImg =cv::imread("C:/Users/USER/Pictures/test/Image__2020-11-09__21-22-55.bmp");
	//cv::cvtColor(rawImg, opencvImg, cv::COLOR_RGB2GRAY);
	//AObj.subImg = opencvImg;
	//AObj.findAllFish(AObj.corner_x, AObj.corner_y, AObj.width, AObj.height);

	//vector<int> xDivs = { 433 };
	//vector<string> fishIDs = { "test1" };

	//AObj.initialize(xDivs, fishIDs);

	//for (int i=0; i<1000; i++) // TODO: write a macro to encapsulate this `for`
	//{
	//	cameraObj.grabPylonImg(); // TODO: update the return type
	//	//int cIdx = cameraObj.cIdx;
	//	//ArenaObj.aIdx = cIdx;

	//	AObj.getImgFromCamera(cameraObj.frameWidth, cameraObj.frameHeight, (uint8_t*)cameraObj.getPtr2buffer());
	//
	//	// TODO: include the following two lines in the method "preprocessImg"
	//	//AObj.alignImg(180);
	//
	//	AObj.buildBgImg();

	//}
	//
	//cv::namedWindow("test", CV_WINDOW_NORMAL);
	//

	//while (1) {
	//	cameraObj.grabPylonImg(); // TODO: update the return type
	//	//int cIdx = cameraObj.cIdx;
	//	//ArenaObj.aIdx = cIdx;

	//	AObj.getImgFromCamera(cameraObj.frameWidth, cameraObj.frameHeight, (uint8_t*)cameraObj.getPtr2buffer());

	//	//AObj.alignImg(180);
	//	AObj.buildBgImg();
	//	AObj.findSingleFish();
	//	AObj.annotateFish();
	//	AObj.displayImg("test");
	//	cv::waitKey(1);
	//}
	//

	//Arena AObj(4);
	//cv::VideoCapture cap("D:/working/Data/NewTestData-with4fish/201822_1029_Arena2_GCaMP_8dpf_OLexp_RBC.avi");
	////cv::namedWindow("test");



	//vector<string> baseNames = { "test1" };
	//FileWriter FObj;
	////cout << cap.get(cv::CAP_PROP_FOURCC) << endl;


	//FObj.initialize(baseNames, cv::Size(cap.get(cv::CAP_PROP_FRAME_WIDTH), cap.get(cv::CAP_PROP_FRAME_HEIGHT)), cap.get(cv::CAP_PROP_FPS));
	//

	//ExpTimer ETObj;
	//int testTime = 1 * 60;
	//
	//vector<int> xDivs = {207,207,582,582};
	//vector<string> fishIDs = {"test1","test2","test3","test4"};

	//AObj.initialize(xDivs, fishIDs);
	//for (int i = 0; i < 10;i++) {
	//	AObj.getImgFromVideo(cap);
	//	//AObj.alignImg(270);
	//	AObj.buildBgImg();

	//	//cv::imshow("test", AObj.subImg);
	//	//cv::waitKey(1);
	//}
	//ETObj.start();
	//for (ETObj.resetCount(); ETObj.getCount() < testTime * 1; ETObj.addCount()) {
	//	ETObj.getTime();

	//
	//	AObj.getImgFromVideo(cap);
	//	//AObj.alignImg(270);
	//	AObj.buildBgImg();

	//	AObj.findAllFish();
	//	//AObj.checkWhich2GiveShock(62);
	//	//AObj.checkWhich2ReversePattern(62);
	//	FObj.writeOutFrame(ETObj, AObj,0);
	//	AObj.annotateFish();
	//	AObj.displayImg("Live");
	//	cv::waitKey(10);
	//}
	//return 1;
	//




	/*
	Experiment OLexp;
	OLexp.initialize();
	OLexp.runOLexp();
	*/










	//Screen SObj;
	//vector<int> IdxCase1 = { 1,2,2,2 };
	//vector<int> IdxCase2 = { 2,1,2,2 };
	//vector<int> IdxCase3 = { 2,2,1,2 };
	//vector<int> IdxCase4 = { 2,2,2,1 };
	//cv::Mat pattern = cv::imread("D:/working/free-moving learning/code/free-moving/free-moving/test.jpg");
	////cv::imshow("test", pattern);
	////cv::waitKey();

	//SObj.initialize();

	//SObj.refresh(IdxCase1);

	//SObj.refresh(IdxCase2);

	//SObj.refresh(IdxCase3);
	//
	//SObj.refresh(IdxCase4);
	//for (; getchar() == 'q';){}






	/*Relay RObj;
	ExpTimer TObj;
	bool a[8] = { 0,0,0,0,0,0,0,1};
	vector<bool> channels2open;
	for (auto i:a) {
		channels2open.push_back(i);
	}
	RObj.initialize(5);
	for (TObj.resetCount(); TObj.getCount() < 60; TObj.addCount()) {

		RObj.givePulse(channels2open,1);

		cout << TObj.getCount() << endl;
		for (; getchar() == 'q';) {

		}
	}


	*/
	/*


	Screen sObj;
	sObj.initialize(
		"Images/RBC.jpg"
		, "vr"
		, { 0.5f, -0.5f, 1.0f, 1.0f }

	);

	while (1)
	{

		sObj.allAreas[0].allPatches[0]->setTheta(rand() / 10.0);
		sObj.allAreas[0].allPatches[0]->setXDis(rand()/10.0);
		sObj.allAreas[0].allPatches[0]->setYDis(rand()/10.0);
		sObj.allAreas[0].updateVrPattern();
		sObj.show();

	}

	*/

	/*
	SingleCamera cObj;
	cObj.initialize();
	int numFrames2grab = 1000;

	// giant grabbing loop
	for (int i = 0; i < numFrames2grab; i++)
	{

		cObj.grabPylonImg();
	#ifdef PYLON_WIN_BUILD
	// Display the grabbed image.
	Pylon::DisplayImage(1, cObj.pylonImg);
	#endif

	}
	*/

	// TODO: consider to add an end-message and wait for user's input to end the session
}

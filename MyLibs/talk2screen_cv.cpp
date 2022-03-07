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
* Filename: talk2screen.h
* Abstract: this file contains all function definitions
*			used to present visual stimulus to fish
*
* A great portion of code was adapted from learnopengl.com, which is a great
* website to learn OpenGL
*
* Current Version: 2.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: May 12, 2018

* Replaced Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
*/

#include <vector> // include range-based for loop
#include <algorithm>
#include <string>

#include "talk2screen_cv.h"

using namespace std;
using namespace cv;

int x, y, width, height, x_window, y_window;
cv::Mat testImage = cv::Mat(340, 560, CV_8UC3, cv::Scalar(0, 0, 0));
static void on_trackbar_setScreenROI(int, void*) {
	//int max_val = 255;
	//Mat binaryzation = Mat::zeros(img_setThreshold.size(), CV_8UC1);
	//threshold(img_setThreshold, binaryzation, threshold_val, max_val, CV_THRESH_BINARY);
	testImage = cv::Mat(340, 560, CV_8UC3, cv::Scalar(0, 0, 0));
	//cv::rectangle(testImage, cv::Point(x, y), cv::Point(x+ width, y+ height), cv::Scalar(255, 0, 0), -1);
	//cv::rectangle(testImage, cv::Point(x+ width/2-2, y), cv::Point(x + width / 2 + 2, y + height), cv::Scalar(255, 255, 255), -1);
	cv::circle(testImage, cv::Point(x, y), 6, cv::Scalar(255, 255, 255));

	imshow("test", testImage);
	cv::moveWindow("test", x_window, y_window);
}

bool setScreenROI() {
	cv::namedWindow("test");
	cv::namedWindow("test_control");
	testImage = cv::Mat(340, 560, CV_8UC3, cv::Scalar(0, 0, 0));
	cv::imshow("test", testImage);
	cv::waitKey(10);
	cv::moveWindow("test", 0, 0);
	createTrackbar("x_window", "test_control", &x_window, 2*1920, on_trackbar_setScreenROI);
	createTrackbar("y_window", "test_control", &y_window, 1080, on_trackbar_setScreenROI);
	createTrackbar("x", "test_control", &x, 560, on_trackbar_setScreenROI);
	createTrackbar("y", "test_control", &y, 340, on_trackbar_setScreenROI);
	createTrackbar("width", "test_control", &width, 400, on_trackbar_setScreenROI);
	createTrackbar("height", "test_control", &height, 300, on_trackbar_setScreenROI);

	on_trackbar_setScreenROI(0, 0);
	cout << "Press 'q' to exit." << endl;
	while (char(waitKey(1)) != 'q') {}
	return true;
}


void Patch::drawRect(int* corner, int* color) {
	cv::rectangle(PatchImage, cv::Point(corner[0], corner[1]), cv::Point(corner[2], corner[3]), cv::Scalar(color[0], color[1], color[2]), -1);

	cv::rectangle(PatchImage, cv::Point(0, 0), cv::Point(corner_x, corner_y), Scalar(white[0], white[1], white[2]), 2);
	//cv::rectangle(srcImage, cv::Point(midLine[0], midLine[1]), cv::Point(midLine[2], midLine[3]), cv::Scalar(white[0], white[1], white[2]), -1);
}

void Patch::drawRect(int* corner, cv::Mat pattern) {
	cv::Mat ROI = PatchImage(cv::Rect(cv::Point(corner[0], corner[1]), cv::Point(corner[2], corner[3])));
	cv::addWeighted(ROI, 0, pattern, 1, 0, ROI);
}

cv::Mat Patch::getImage()
{
	return PatchImage;
}

void Screen::test() {
	setScreenROI();
}
void Screen::initialize() {
	//initialize the Window
	cv::namedWindow("test");
	cv::imshow("test", srcImage);
	cv::waitKey(10);
	//cv::moveWindow("test", 1920, 0);
	cv::moveWindow("test", 2148, 324);

	//initialize patches

	leftPatch = new Patch(ROI_col, ROI_row);
	leftPatch->drawRect(leftPatchCorner, blue);
	rightPatch = new Patch(ROI_col, ROI_row);
	rightPatch->drawRect(rightPatchCorner, blue);
	blackPatch = new Patch(ROI_col, ROI_row);
	blackPatch->drawRect(fullPatchCorner, black);

}

void Screen::initializeFullPattren(int* fullPattern, int corner_x, int corner_y) {
	leftPatch = new Patch(corner_x, corner_y);
	leftPatch->drawRect(leftPatchCorner, fullPattern);
	
	rightPatch = new Patch(corner_x, corner_y);
	rightPatch->drawRect(rightPatchCorner, fullPattern);
	blackPatch = new Patch(corner_x, corner_y);
	blackPatch->drawRect(fullPatchCorner, black);
	fullPatch = new Patch(corner_x, corner_y);
	fullPatch->drawRect(fullPatchCorner, fullPattern);
}

void Screen::initializePattren(Mat pattern, int corner_x, int corner_y) {
	leftPatch = new Patch(corner_x, corner_y);
	leftPatch->drawRect(leftPatchCorner, pattern);
	rightPatch = new Patch(corner_x, corner_y);
	rightPatch->drawRect(rightPatchCorner, pattern);
	blackPatch = new Patch(corner_x, corner_y);
	blackPatch->drawRect(fullPatchCorner, black);
}

void Screen::initialize(int pattern) {
	ROI_col = 200;
	ROI_row = 200;
	srcImage = cv::Mat(700, 700, CV_8UC3, cv::Scalar(0, 0, 0));


	ROI = srcImage(cv::Rect(200+windowX, 200+windowY, ROI_col, ROI_row));


	leftPatchCorner[0] = 0;
	leftPatchCorner[1] = 0;
	leftPatchCorner[2] = 100;
	leftPatchCorner[3] = 200;

	rightPatchCorner[0] = 100;
	rightPatchCorner[1] = 0;
	rightPatchCorner[2] = 200;
	rightPatchCorner[3] = 200;

	fullPatchCorner[0] = 0;
	fullPatchCorner[1] = 0;
	fullPatchCorner[2] = 200;
	fullPatchCorner[3] = 200;

	//initialize the Window
	cv::namedWindow("test2");
	cv::imshow("test2", srcImage);
	cv::waitKey(10);
	//cv::moveWindow("test", 1920, 0);
	cv::moveWindow("test2", 894, 644);
	//cv::moveWindow("test", 2148 + 1920, 324);
	switch (pattern)
	{
	case(1):
		initializeFullPattren(red, ROI_col, ROI_row);
		break;
	case(2):
		initializeFullPattren(blue, ROI_col, ROI_row);
		break;
	case(3):
		initializeFullPattren(green, ROI_col, ROI_row);
		break;
	case(4):
		initializeFullPattren(yellow, ROI_col, ROI_row);
		break;
	case(5):
		initializeFullPattren(white, ROI_col, ROI_row);
		break;
	//case(11):
		//initializePattren(test_pattern, ROI_col, ROI_row);
		//break;
	default:
		break;
	}
	//initialize patches

}


void Screen::refresh(int IdxCase, string window_name, int platformX, int platformY) {

	if (platformX < 100 || platformX>350 || platformY < 100 || platformY>250) {
		return;
	}
	ROI = srcImage(cv::Rect(platformX + windowX, platformY + windowY, ROI_col, ROI_row));


	switch (IdxCase)
	{
	case 0:
		cv::addWeighted(ROI, 0, leftPatch->getImage(), 1, 0, ROI);
		//circle(srcImage, cv::Point(200, 200), 5, Scalar(255, 255, 255), 2);
		break;
	case 1:
		cv::addWeighted(ROI, 0, rightPatch->getImage(), 1, 0, ROI);
		//circle(srcImage, cv::Point(200, 200), 5, Scalar(255, 255, 255), 2);
		break;
	case 2:
		cv::addWeighted(ROI, 0, blackPatch->getImage(), 1, 0, ROI);
		//circle(srcImage, cv::Point(200, 200), 5, Scalar(255, 255, 255), 2);
		break;
	case 3:
		cv::addWeighted(ROI, 0, fullPatch->getImage(), 1, 0, ROI);

		break;
	}

	

	cv::imshow(window_name, srcImage);
	cv::waitKey(1);


}

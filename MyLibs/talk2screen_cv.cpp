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
//using namespace cv;

void Patch::drawRect(int* corner, int* color) {
	cv::rectangle(srcImage, cv::Point(corner[0], corner[1]), cv::Point(corner[2], corner[3]), cv::Scalar(color[0], color[1], color[2]), -1);
}

void Patch::drawRect(int* corner, cv::Mat pattern) {
	cv::Mat ROI = srcImage(cv::Rect(cv::Point(corner[0], corner[1]), cv::Point(corner[2], corner[3])));
	cv::addWeighted(ROI, 0, pattern, 1, 0, ROI);
}

cv::Mat Patch::getImage()
{
	return srcImage;
}

void Screen::initialize() {
	//initialize the Window
	cv::namedWindow("test");
	cv::imshow("test", srcImage);
	cv::waitKey(10);
	cv::moveWindow("test", 1920, 0);


	//initialize patches

	leftPatch = new Patch();
	leftPatch->drawRect(leftPatchCorner, blue);
	rightPatch = new Patch();
	rightPatch->drawRect(rightPatchCorner, blue);
	blackPatch = new Patch();
	blackPatch->drawRect(fullPatchCorner, black);

}
void Screen::initializeFullPattren(int* fullPattern){
	leftPatch = new Patch();
	leftPatch->drawRect(leftPatchCorner, fullPattern);
	rightPatch = new Patch();
	rightPatch->drawRect(rightPatchCorner, fullPattern);
	blackPatch = new Patch();
	blackPatch->drawRect(fullPatchCorner, black);
}

void Screen::initializePattren(Mat pattern) {
	leftPatch = new Patch();
	leftPatch->drawRect(leftPatchCorner, pattern);
	rightPatch = new Patch();
	rightPatch->drawRect(rightPatchCorner, pattern);
	blackPatch = new Patch();
	blackPatch->drawRect(fullPatchCorner, black);
}

void Screen::initialize(int pattern) {
	//initialize the Window
	cv::namedWindow("test");
	cv::imshow("test", srcImage);
	cv::waitKey(100);
	cv::moveWindow("test", 1920, 0);

	switch (pattern)
	{
	case(1):
		initializeFullPattren(red);
		break;
	case(11):
		initializePattren(test_pattern);
		break;
	default:
		break;
	}
	//initialize patches

}

void Screen::refresh(vector<int> IdxCase) {
	for (int i = 0; i < 6; i++) {
		switch (IdxCase[i])
		{
		case 0:
			cv::addWeighted(allROI[i], 0, leftPatch->getImage(), 1, 0, allROI[i]);
			continue;
			break;
		case 1:
			cv::addWeighted(allROI[i], 0, rightPatch->getImage(), 1, 0, allROI[i]);
			continue;
			break;
		case 2:
			cv::addWeighted(allROI[i], 0, blackPatch->getImage(), 1, 0, allROI[i]);
			continue;
			break;
		}
	}
	cv::namedWindow("test");
	cv::imshow("test", srcImage);
	cv::waitKey(1);


}

#pragma once
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
* Filename: fishAnalysis.h
* Abstract: this file contains all classes and functions' declarations
*			used in analyzing fish behavioral parameters, such as
*			fish's head, tail and center.
*
* Current Version: 3.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 15, 2018
* Modified on: Apr. 20, 2019
*/

#ifndef _GUARD_FISHANALYSIS_H
#define _GUARD_FISHANALYSIS_H

// Include OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>

// Include standard libraries
#include <stdio.h> 
#include <iostream>

#include <vector>

#include <Python.h>


using namespace std;



class FixedFish {

private:
	; // nothing for now
public:
	// methods
	FixedFish(cv::Point head, cv::Point center,int threshold_val,cv::Mat mask) // constructor
		: fishHead(head)
		, fishCenter(center)
		, threshold_val(threshold_val)
		, fishROI(mask)
	{
		count = 0;
		boutFrames = 44;
	
		shockOn = false;
		idxCase = 0;
		fishTail = cv::Point(-1, -1);
		tailingAngle.resize(500000,3);
		tailingFlag.resize(500000, 0);
	}
	

	bool fishAngleAnalysis(int counter);
	// properties
	// const properties
	void getImgFromVideo(cv::VideoCapture cap);
	void getMat(cv::Mat curImg);
	void predictDirection();


	const std::string ID;


	void resetCount();

	int getCount();

	void addCount(int increase=1);

	

					// status properties
	
	bool shockOn,leftTrue;
	int idxCase, threshold_val, count;
	int boutFrames;
	vector<double> tailingAngle;
	vector<int> tailingFlag;

	std::vector<cv::Point> fishContour;
	cv::Point fishHead, fishTail, fishCenter;
	cv::Mat fishImg, fishImgFromVideo,fishROI,ROIimage;
	
};

int findClosestPt(vector<cv::Point>& contour, cv::Point point);

double getPt2LineDistance(cv::Point2f P, cv::Point2f A, cv::Point2f B);

vector<int> findPtsLineIntersectContour(vector<cv::Point>& contour, cv::Point2f A, cv::Point2f B);

#endif // !_GUARD_FISHANALYSIS_H
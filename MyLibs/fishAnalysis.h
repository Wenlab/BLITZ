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
#include <Windows.h>  
// Include standard libraries
#include <vector>


// Include user-defined libraries
#include "errorHandling.h"

/* Define related methods and properties for a single fish */
// Write every frame updated info at here? No! Create another class in fileWriter class
class Fish {

private:
	; // nothing for now
public:
	// methods
	Fish() // constructor
	{
		xDiv = 0;
		lastTimeUpdatePattern = -1;
		lastTimeInCS = -1;
		lastShockTime = -1;
		pauseFrames = -1;
		shockOn = false;
		inCD = false;
		idxCase = 0;
		head = cv::Point(200, 200);
	}
	/*
	Find the head, center, tail and headingAngle of the fish
	by finding the end-points of the contour
	*/

	void getPosition(int platformX, int platformY);

	/* Find head and tail with endIndices */
	void findHeadAndTail(std::vector<int> endIndices);

	/* Calculate heading angle of the fish */
	void calcHeadingAngle();

	/*Determine which side is fish's head by measuring the area of each half*/
	bool findHeadSide(cv::Point2f* M);

	void findFishTail();
	void findTail();

	/* Decide whether to give this fish shock */
	//bool checkIfGiveShock(int sElapsed);
	bool checkIfGiveShock2(int sElapsed);
	bool checkIfGiveShock2Test(int sElapsed);
	bool checkIfGiveShockForAnti(int sElapsed);

	int generatePattern(int Count);

	/* Decide if to reverse the pattern;
	reverse the pattern if fish stays in non-CS zone too long
	*/
	bool checkIfReversePattern(int sElapsed);
	void setNewPattern();
	void setNewPatternForAnti();




	// properties
	// const properties
	std::string ID;
	//const int yDiv; // the division pos between CS and NCS pattern
	int xDiv;
	// status properties
	int lastTimeUpdatePattern;
	int lastTimeInCS;
	int lastShockTime;
	int pauseFrames;
	bool shockOn;
	bool inCD;
	int idxCase; // indicate the situation (e.g., pattern) that fish is in
	int head_idx;
	int pattern;
	// motion parameters

	cv::Point head;

};





// Global functions


/* TODO: consider to convert the following global functions to methods of FishAnalysis */
/*Find the closest point on the contour to the reference point*/
int findClosestPt(std::vector<cv::Point>& contour, cv::Point point);

/* Rotate image with a given angle */
void rotateImg(cv::Mat src, cv::Mat dst, int deg2rotate);
/* Rotate the image 90 degrees clockwise */
void rot90CW(cv::Mat src, cv::Mat dst);
/* Get the Euclidean distance from point P to line AB */
double getPt2LineDistance(cv::Point2f P, cv::Point2f A, cv::Point2f B);
/* Find 2 intersection points of a line (AB) and contour */
std::vector<int> findPtsLineIntersectContour(std::vector<cv::Point>& contour, cv::Point2f A, cv::Point2f B);

void TrackbarInitialize();


#endif // !_GUARD_FISHANALYSIS_H

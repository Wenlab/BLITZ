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
* Filename: fishAnalysis.h
* Abstract: this file contains all classes and functions' declarations
*			used in analyzing fish behavioral parameters, such as
*			fish's head, tail and center.
*
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: Apr. 28, 2018

* Replaced Version: 1.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
*/
#ifndef _GUARD_FISHANALYSIS_H
#define _GUARD_FISHANALYSIS_H

// Include OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/highgui/highgui.hpp>

// Include standard libraries
#include <vector>

#define MAX_FISH_PER_ARENA 4
#define X_CUT 385 // x position to separate fish 0,1 and 2,3
#define Y_CUT 385 // y position to separate fish 0,1 and 2,3

/* Define related methods and properties for a single fish */
class FishData {

private:
	; // nothing for now
public:
	// methods
	FishData(std::string fishID = "", int fishAge = 0, int yDivide = 0) // constructor
		: ID(fishID)
		, age(fishAge)
		, yDiv(yDivide)
	{
		lastBlackoutStart = -1;
		lastTimeUpdatePattern = -1;
		lastTimeInCS = -1;
		lastShockTime = -1;
		pauseFrames = -1;
		shockOn = false;
		head = cv::Point(-1, -1);
		tail = cv::Point(-1, -1);
		center = cv::Point(-1, -1);
		headingAngle = -360;
	}
	/*
	Find the head, center, tail and headingAngle of the fish
	by finding the end-points of the contour
	*/
	void findPosition();
	/*Determine which side is fish's head by measuring the area of each half*/
	bool findHeadSide(cv::Point2f* M);

	bool ifGiveShock(int pIdx, int sElapsed);

	int updatePatternInTraining(int sElapsed, int pIdx, int ITI);

	// properties
	// const properties
	const std::string ID;
	const int age;
	// TODO: assign yDivide in the initialization function
	int yDiv; // the division pos between CS and NCS pattern

	int lastBlackoutStart;
	
	int lastTimeUpdatePattern;
	int lastTimeInCS;
	int lastShockTime;
	int pauseFrames;
	bool shockOn;

	std::vector<cv::Point> fishContour;
	cv::Point head, tail, center;
	int headingAngle;
};

/* Define all infos including fish for a single arena */
class ArenaData {

private:
	;// nothing for now
public:
	// methods
	ArenaData(int BWthre = 30, int n = 1) // constructor
		: numFish(n)
	{
		binThre = BWthre;
		allFish.reserve(numFish); // allocate memory
	}

	void initialize(std::vector<std::string> fishIDs, int fishAge, std::vector<int> yDivs);
	
	/* find all fish contours in the arena at the same time
	by finding the largest #fish contours in all contours.
	Involved parameters:
	1.Threshold for contour size,
	2.Moments of contours
	Scheme for fish positions in arena
	|		|		|
	|	0	|	1	|
	|		|		|
	|---------------|
	|		|		|
	|	2	|	3	|
	|		|		|
	*/
	bool findAllFish();
	
	void prepareBgImg(int width, int height, int cIdx, uint8_t* buffer);

	void annotateFish(std::vector<int> pIdx);
	
	void resetShocksOn();
								   
	// properties
	const int numFish;
	int binThre; // in the future, this might be adjusted in the GUI

	cv::Ptr<cv::BackgroundSubtractor> pMOG; // one pMOG for one arena
	cv::Mat opencvImg, HUDSimg, subImg;
	std::vector<FishData> allFish;

};
/* Initialize all arenas will be used in the experiment */
std::vector<ArenaData> initializeAllArenas(std::vector<std::vector<int>> yDivs, std::vector<std::vector<std::string>> fishIDs, int fishAge);
/*Find the closest point on the contour to the reference point*/
int findClosestPt(std::vector<cv::Point>& contour, cv::Point point);
/* Rotate the image 90 degrees clockwise */
void rot90CW(cv::Mat src, cv::Mat dst);
/* Get the Euclidean distance from point P to line AB */
double getPt2LineDistance(cv::Point2f P, cv::Point2f A, cv::Point2f B);
/* Find 2 intersection points of a line (AB) and contour */
std::vector<int> findPtsLineIntersectContour(std::vector<cv::Point>& contour, cv::Point2f A, cv::Point2f B);


#endif // !_GUARD_FISHANALYSIS_H

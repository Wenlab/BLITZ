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

// Include standard libraries
#include <vector>

#define MAX_FISH_PER_ARENA 4


/* Define related methods and properties for a single fish */
class FishData {

private:
	; // nothing for now
public:
	// methods
	FishData() // constructor
	{
		lastBlackoutStart = -1;
		lastFishPatternUpdate = -1;
		patternIndex = -1;
		lastTimeInCS = -1;
		lastTimeInNCS = -1;
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
	bool findHeadSide(cv::Point2f* M, cv::Point2f refPt);
	// properties


	int lastBlackoutStart;
	int lastFishPatternUpdate;
	int patternIndex;
	int lastTimeInCS;
	int lastTimeInNCS;
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
	ArenaData(int BWthre = 30, int n = 4) // constructor
	{
		xCut = 390;
		yCut = 383;
		numFish = n;
		binThre = BWthre;
		allFish.resize(numFish); // allocate memory
	}

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

	TODO:
	1. Abolish fishFlag?
	2. Consize the recursive ifs
	*/
	bool findAllFish(); 
	
								   
	// properties
	int xCut;
	int yCut;

	int numFish;
	int binThre;
	//int xCut, yCut; // the partition positions in the arena
	
	cv::Ptr<cv::BackgroundSubtractor> pMOG; // one pMOG for one arena
	cv::Mat opencvImg, HUDSimg, subImg;
	std::vector<FishData> allFish;


};


/*Find the closest point on the contour to the reference point*/
int find_closest_point_on_contour(std::vector<cv::Point>& contour, cv::Point point);
/* Get the Euclidean distance between two Points */
double getDistance(cv::Point A, cv::Point B);
/* Rotate the image 90 degrees clockwise */
void rot_90_CW(cv::Mat src, cv::Mat dst);



#endif // !_GUARD_FISHANALYSIS_H

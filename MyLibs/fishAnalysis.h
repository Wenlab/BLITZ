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
#include <vector>

#define MAX_FISH_PER_ARENA 4
#define X_CUT 385 // x position to separate fish 0,1 and 2,3
#define Y_CUT 385 // y position to separate fish 0,1 and 2,3

/* the highest class that encapsulates everything about imaging processing */
class FishAnalysis
{
public:
		// Methods
		// y division pos for all fish
		FishAnalysis = {
			yDivs =
			{
				{ 200, 200, 558, 558 },
				{ 223, 223, 588, 588 },
				{ 223, 223, 588, 588 }
			}; // TODO: make this variable private
		}


		// Methods
		/* Initialize all arenas will be used in the experiment */
		std::vector<ArenaData> initialize(); // TODO: update the implementation

		/* Prepare background image for MOG subtractor */
		void prepareBgImg(const int prepareTime); // TODO: consider to make `prepareTime` a local variable?

		/* Find all fish in all arenas */
		void findAllFish();

		/* Decorate images with fish's heads, tails and visual pattern's info */
		void annotateImgs(); // TODO: update the implementation

		/* Present fish images with annotations. The code is adapted from code in stackfow. */
		void displayImgs(std::string title); // TODO: update the implementation

		/* Check which fish to give shock */
		// TODO: write the implementation
		std::vector<bool> checkIfGiveShock();

		/* Check to reverse whose patterns */
		// TODO: write the implementation
		std::vector<bool> checkIfReversePattern();

		/* Reset shocksOn to false for all fish */
		// TODO: write the implementation
		void resetShocksOn();

		// Properties
		std::vector<ArenaData> allArenas; // TODO: consider to make this private?
private:
		std::vector<std::vector<int>> yDivs; 	// TODO: move this into the imaging processing module


}

/* Define all infos including fish for a single arena */
class Arena {

private:
	;// nothing for now
public:
	// methods
	Arena(int n = 1) // constructor
		: numFish(n)
	{
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
	/* Find all fish in this arena */
	bool findAllFish();

	/* getImgFromCamera */
	bool getImgFromCamera(uint8_t* ptr2buffer);

	/* Align all images to user's view */
	void alignImgs(int width, int height, int cIdx, uint8_t* buffer); // TODO: consider to remove width, and height

	/* Decorate images with fish's heads, tails and visual pattern's info */
	void annotateFish(); // TODO: engineer the relation between this and the-same-name function in FishAnalysis class

	/* Present fish images with annotations. The code is adapted from code in stackfow. */
	void displayImgs(std::string title); // TODO: update the implementation

	/* Reset shocksOn to false for all fish */
	// TODO: write the implementation
	void resetShocksOn();

	// properties
	const int numFish;

	cv::Ptr<cv::BackgroundSubtractor> pMOG; // background subtractor for detecting moving object
	cv::Mat opencvImg, HUDSimg, subImg;
	std::vector<FishData> allFish;

};



/* Define related methods and properties for a single fish */
// Write every frame updated info at here? No! Create another class in fileWriter class
class Fish {

private:
	; // nothing for now
public:
	// methods // TODO: rewrite
	Fish(int yDivide = 0, std::string fishID = "", int fishAge = 0) // constructor
		: ID(fishID)
		, age(fishAge)
		, yDiv(yDivide)
	{
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

	/* Decide whether to give this fish shock */
	bool ifGiveShock(int pIdx, int sElapsed);

	// properties
	// const properties
	const std::string ID;
	const int age;

	// TODO: consider to make it const
	int yDiv; // the division pos between CS and NCS pattern

// status properties
	int lastTimeInCS;
	int lastShockTime;
	int pauseFrames;// TODO: consider to change this variable
	bool shockOn;
	int idxCase; // indicate the situation (e.g., pattern) that fish is in

	// motion parameters
	std::vector<cv::Point> fishContour;
	cv::Point head, tail, center;
	int headingAngle;
};

// Global functions


/* TODO: consider to convert the following global functions to methods of FishAnalysis */
/*Find the closest point on the contour to the reference point*/
int findClosestPt(std::vector<cv::Point>& contour, cv::Point point);
/* Rotate the image 90 degrees clockwise */
void rot90CW(cv::Mat src, cv::Mat dst);
/* Get the Euclidean distance from point P to line AB */
double getPt2LineDistance(cv::Point2f P, cv::Point2f A, cv::Point2f B);
/* Find 2 intersection points of a line (AB) and contour */
std::vector<int> findPtsLineIntersectContour(std::vector<cv::Point>& contour, cv::Point2f A, cv::Point2f B);




#endif // !_GUARD_FISHANALYSIS_H

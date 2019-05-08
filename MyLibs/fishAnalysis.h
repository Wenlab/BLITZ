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

// Include user-defined libraries
#include "errorHandling.h"

/* Define related methods and properties for a single fish */
// Write every frame updated info at here? No! Create another class in fileWriter class
class Fish {

private:
	; // nothing for now
public:
	// methods
	Fish(int yDivide = 0, std::string fishID = "") // constructor
		: ID(fishID)
		, yDiv(yDivide)
	{
		lastTimeUpdatePattern = -1;
		lastTimeInCS = -1;
		lastShockTime = -1;
		pauseFrames = -1;
		shockOn = false;
		idxCase = 0;
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

	/* Find head and tail with endIndices */
	void findHeadAndTail(std::vector<int> endIndices);

	/* Calculate heading angle of the fish */
	void calcHeadingAngle();

	/*Determine which side is fish's head by measuring the area of each half*/
	bool findHeadSide(cv::Point2f* M);

	/* Decide whether to give this fish shock */
	bool checkIfGiveShock(int sElapsed);

	/* Decide if to reverse the pattern;
	reverse the pattern if fish stays in non-CS zone too long
	*/
	bool checkIfReversePattern(int sElapsed);

	// properties
	// const properties
	const std::string ID;
	const int yDiv; // the division pos between CS and NCS pattern

					// status properties
	int lastTimeUpdatePattern;
	int lastTimeInCS;
	int lastShockTime;
	int pauseFrames;
	bool shockOn;
	int idxCase; // indicate the situation (e.g., pattern) that fish is in

				 // motion parameters
	std::vector<cv::Point> fishContour;
	cv::Point head, tail, center;
	int headingAngle;
};


/* Define all infos including fish for a single arena */
class Arena {

private:
	;// nothing for now
public:
	// methods
	Arena(int n = 1) // constructor
		: numFish(n)
		, maxNumFish(4)
		, xCut(385)
		, yCut(385)
	{
		// TODO: replace with a better errorhandling

		allFish.reserve(numFish); // allocate memory
		binThre = 40;
		historyLen = 2000;
	}

	/* Initialize with given fishIDs */
	void initialize(std::vector<int> yDivs, std::vector<std::string> fishIDs);

	/* Initialize with y division positions only */
	void initialize(std::vector<int> yDivs);

	/* initialization for common image processing purpose */
	void initialize();

	/* Get image from camera to opencvImg */
	void getImgFromCamera(int width, int height, uint8_t* buffer);// TODO: consider to remove width, and height

																  /* Get image from videos */
	void getImgFromVideo(cv::VideoCapture cap);

	/* Build background image model with MOG operator */
	void buildBgImg();

	/* Align all images to user's view */
	void alignImg(int deg2rotate);

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
	void findAllFish();

	/* Find the single fish in the entire image,
	NOT applicable to multi-fish scenarios.
	*/
	bool findSingleFish();

	/* Calculate which quadratile this contour is */
	int findQuadratile(std::vector<cv::Point> contour);

	/* Decorate images with fish's heads, tails and visual pattern's info */
	void annotateFish(); // TODO: engineer the relation between this and the-same-name function in FishAnalysis class

						 /* Present fish images with annotations.
						 The code is adapted from code in stackfow. */
	void displayImg(std::string title); // TODO: update the implementation

										/* Check which fish to give shock */
	std::vector<bool> checkWhich2GiveShock(int sElapsed);

	/* Check which fish to reverse its underneath pattern */
	std::vector<bool> checkWhich2ReversePattern(int sElapsed);

	/* Reset shocksOn to false for all fish */
	void resetShocksOn();

	/* Get number of fish in arena */
	int getNumFish();

	// properties
	const int numFish;
	const int maxNumFish;
	const int xCut;// x position to separate fish 0,1 and 2,3
	const int yCut;
	int binThre; // binary threshold
	int historyLen; // used in MOG substractor

	cv::Ptr<cv::BackgroundSubtractor> pMOG; // background subtractor for detecting moving object
	cv::Mat opencvImg, subImg;
	std::vector<Fish> allFish;

};





/* the highest class that encapsulates everything about imaging processing */
class FishAnalysis {
public:
		// Methods
		// y division pos for all fish
		FishAnalysis()
		{
			yDivs =
			{
				{ 200, 200, 558, 558 },
				{ 223, 223, 588, 588 },
				{ 223, 223, 588, 588 }
			}; // TODO: make this variable private

			aIdx = 0;
		}

		// Methods
		/* Initialize with allFishIDs */
		void initialize(std::vector<std::vector<std::string>> allFishIDs); // TODO: update the implementation

		/* Initialize all arenas will be used */
		void initialize(std::vector<int> numFishInArenas);

		/* Process image from camera */
		void preprocessImg();

		/* Get image from camera */
		void getImgFromCamera(int width, int height, uint8_t* buffer);

		/* Align images to user's view */
		void alignImg();

		/* Build up the model for background image */
		void buildBgImg();

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
		/* Find all fish in all arenas */
		void findAllFish();

		/* Decorate images with fish's heads, tails and visual pattern's info */
		void annotateImg(); // TODO: update the implementation

		/* Present fish images with annotations. The code is adapted from code in stackfow. */
		void displayImg(std::string title); // TODO: update the implementation

		/* Check which fish to give shock */
		std::vector<std::vector<bool>> checkWhich2GiveShock(int sElapsed);

		/* Check to reverse whose patterns */
		// TODO: write the implementation
		std::vector<std::vector<bool>> checkWhich2ReversePattern(int sElapsed);

		/* Reset shocksOn to false for all fish */
		// TODO: write the implementation
		void resetShocksOn();

		// Properties
		std::vector<Arena> allArenas; // TODO: consider to make this private?
		int numArenas;
		int aIdx; // index of arena to process
private:
		std::vector<std::vector<int>> yDivs;
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

/*This function return a radian to describe the fishtailing motion */
bool fishAngleAnalysis(Mat fishImg, Point fishHead, Point fishCenter, Point * fishTail_return, double* fishAngle,int threshold_val);


#endif // !_GUARD_FISHANALYSIS_H

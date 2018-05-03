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
* Filename: fishAnalysis.cpp
* Abstract: this file contains all function definitions
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


// Include user-defined libraries
#include "fishAnalysis.h"

// Include standard libraries
#include <iostream>

// User-defined macros
#define PI 3.14159

using namespace std;
using namespace cv;

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
bool ArenaData::findAllFish() 
{
	bool fishFlag = false;
	vector<vector<Point>> contours;
	findContours(subImg, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
	
	// the size of fish contour should above this threshold
	const int conThre = 20;
	for (int i = 0; i < contours.size(); i++)
	{
		vector<Point> tempContour = contours[i];
		int contourSize = tempContour.size();
		if ( contourSize < conThre)
			continue;

		// record largest contour of each quadratile and its index
		// with initial values of -1 (not found)
		vector<vector<int>> maxContours;
		maxContours.resize(2, vector<int>(4, -1));

		Moments M = moments(tempContour); // to find the center
		int x = int(M.m10 / M.m00);
		int y = int(M.m01 / M.m00);

		if (y < yCut)
		{ 
			if (x < xCut)
			{
				if (maxContours[0][0] < contourSize)
				{
					maxContours[0][0] = contourSize;
					maxContours[1][0] = i;
				}
			}
			else
			{
				if (maxContours[0][1] < contourSize)
				{
					maxContours[0][1] = contourSize;
					maxContours[1][1] = i;
				}
			}
		}
		else
		{
			if (x < xCut)
			{
				if (maxContours[0][2] < contourSize)
				{
					maxContours[0][2] = contourSize;
					maxContours[1][2] = i;
				}
			}
			else {
				if (maxContours[0][3] < contourSize)
				{
					maxContours[0][3] = contourSize;
					maxContours[1][3] = i;
				}
			}
		}
		
		// assign largest contour to each fish
		for (int i = 0; i < numFish; i++)
		{
			if (maxContours[1][i] == -1)
			{
				allFish[i].pauseFrames++;
				cout << "Fish: " << i << " not found! " << endl;
				fishFlag = false;
			}
			else
			{
				allFish[i].pauseFrames = 0;
				allFish[i].fishContour = contours[maxContours[1][i]];
				allFish[i].findPosition();
				fishFlag = true;
			}
		}
	}
	return fishFlag;
}
/*
	Find the head, center, tail and headingAngle of the fish
	by finding the end-points of the contour
*/
void FishData::findPosition()
{
	RotatedRect R = minAreaRect(fishContour);
	Point2f BPs[4]; // box points
	R.points(BPs);

	Point2f M[2];
	int longEdgeIdx = 1;
	int shortEdgeIdx = 3;
	if (R.size.height < R.size.width)
	{
		longEdgeIdx = 3;
		shortEdgeIdx = 1;
	}
	M[0] = (BPs[longEdgeIdx] + BPs[0]) / 2;
	M[1] = (BPs[shortEdgeIdx] + BPs[2]) / 2;

	vector<Point2f> refPts = { (BPs[0] + BPs[shortEdgeIdx]) / 2, (BPs[2] + BPs[longEdgeIdx]) / 2 };
	if (findHeadSide(M, refPts[0]))
	{
		int headIdx = find_closest_point_on_contour(fishContour, refPts[0]);
		head = fishContour[headIdx];
		int tailIdx = find_closest_point_on_contour(fishContour, refPts[1]);
		tail = fishContour[tailIdx];	
	}
	else {
		int headIdx = find_closest_point_on_contour(fishContour, refPts[1]);
		head = fishContour[headIdx];
		int tailIdx = find_closest_point_on_contour(fishContour, refPts[0]);
		tail = fishContour[tailIdx];
	}
	Moments Mom = moments(fishContour);
	center = Point(Mom.m10 / Mom.m00, Mom.m01 / Mom.m00);
	Point T2H = head - tail; // tail to head
	headingAngle = atan2(T2H.y, T2H.x) * 180 / PI;
	
}

/*
	Determine which side is fish's head 
	by measuring the area of each half
*/
bool FishData::findHeadSide(Point2f* M, Point2f refPt)
{
	Point2f vM = M[1] - M[0];
	Point2f vRef = refPt - M[0];// contour half of negative skewness contains refPt
	bool skewness = vM.x*vRef.y - vM.y*vRef.x > 0;
	int idx0 = find_closest_point_on_contour(fishContour, Point(M[0]));
	int idx1 = find_closest_point_on_contour(fishContour, Point(M[1]));

	vector<vector<Point>> contourHalves(2);
	if (idx0 < idx1)
	{
		// contour half of negative skewness contains refPt
		contourHalves[0].insert(contourHalves[0].end(), fishContour.begin() + idx0, fishContour.begin() + idx1);
		contourHalves[1].insert(contourHalves[1].end(), fishContour.begin() + idx1, fishContour.end());
		contourHalves[1].insert(contourHalves[1].end(),fishContour.begin(),fishContour.begin()+idx0);
	}
	else if (idx0 == idx1){
		cout << "Something wrong with midline." << endl;
	}
	else {
		// contour half of negative skewness contains refPt
		contourHalves[0].insert(contourHalves[0].end(), fishContour.begin() + idx0, fishContour.end());
		contourHalves[0].insert(contourHalves[0].end(), fishContour.begin(), fishContour.begin() + idx1);
		contourHalves[1].insert(contourHalves[1].end(), fishContour.begin() + idx1, fishContour.begin() + idx0);
	}
	vector<int> areas(2,0);
	areas[0] = contourArea(contourHalves[0]);
	areas[1] = contourArea(contourHalves[1]);
	if (skewness < 0)
		return areas[0] > areas[1];
	else
		return areas[1] > areas[0];
}

/*Find the closest point on the contour to the reference point*/
int find_closest_point_on_contour(vector<Point>& contour, Point point)
{
	double minDist = 1000000;
	double tempDist = 0;
	Point temp;
	int goodIndex = 0;
	for (int i = 0; i < contour.size(); i++)
	{
		temp = contour[i];
		tempDist = getDistance(contour[i], point);

		if (tempDist < minDist)
		{
			goodIndex = i;
			minDist = tempDist;
		}
	}

	return goodIndex;
}

/* Get the Euclidean distance between two Points */
double getDistance(Point A, Point B)
{
	double distance;
	distance = sqrtf(powf((A.x - B.x), 2) + powf((A.y - B.y), 2));
	return distance;
}

void rot_90_CW(Mat src, Mat dst)
{
	Mat temp;
	flip(src, temp, 0);
	transpose(temp, dst);
}

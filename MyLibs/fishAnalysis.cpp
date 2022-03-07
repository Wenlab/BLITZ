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

//using namespace std;
using std::string;
using std::cout;
using std::endl;
using std::vector;
using std::cerr;
//using namespace cv;
using cv::Mat;
using cv::Size;
using cv::Rect;
using cv::namedWindow;
using cv::Point;
using cv::RETR_EXTERNAL;
using cv::CHAIN_APPROX_NONE;
using cv::Moments;
using cv::Scalar;
using cv::Vec4f;
using cv::Point2f;

int sum_curv_baseline_positive = 0;
int sum_curv_baseline_negative = 0;
int Magnification_forward = 10;
int Magnification_turn = 10;
int checkbout = 0;
int experiment = 0;
double sum_curv_baseline;
//
//void FishAnalysis::initialize(vector<vector<string>> allFishIDs)
//{// TODO: consider to remove this attribute?
//	numArenas = allFishIDs.size();
//	for (int i = 0; i < numArenas; i++)
//	{
//		vector<string> fishIDs = allFishIDs[i];
//		int numFish = fishIDs.size();
//		Arena arena(numFish);
//		arena.initialize(yDivs[i], fishIDs);
//		allArenas.push_back(arena);
//	}
//
//}
//
//void FishAnalysis::initialize(vector<int> numFishInArenas)
//{
//	numArenas = numFishInArenas.size();
//	for (int i = 0; i < numArenas; i++)
//	{
//		int numFish = numFishInArenas[i];
//		Arena arena(numFish);
//		arena.initialize(yDivs[i]);
//		allArenas.push_back(arena);
//	}
//}
//
//void FishAnalysis::getImgFromCamera(int width, int height, uint8_t* buffer)
//{
//	allArenas[aIdx].getImgFromCamera(width, height, buffer);
//}
//
//void FishAnalysis::preprocessImg()
//{
//	alignImg();
//	buildBgImg();
//}
//
//void FishAnalysis::alignImg()
//{
//	if (aIdx != 0)
//		allArenas[aIdx].alignImg(270);
//}
//
//void FishAnalysis::buildBgImg()
//{
//	allArenas[aIdx].buildBgImg();
//}
//
//void FishAnalysis::findAllFish()
//{
//	for (Arena& arena : allArenas)
//		arena.findAllFish();
//}
//
//void FishAnalysis::annotateImg()
//{
//	for (Arena& arena : allArenas)
//		arena.annotateFish();
//}
//
//void FishAnalysis::displayImg(string title)
//{
//	int size;
//	int i;
//	int m, n;
//	int x, y;
//
//	// w - Maximum number of images in a row
//	// h - Maximum number of images in a column
//	int w, h;
//
//	// scale - How much we have to resize the image
//	float scale;
//	int max;
//
//	// If the number of arguments is lesser than 0 or greater than 12
//	// return without displaying
//	if (numArenas <= 0) {
//		printf("Number of arguments too small....\n");
//		return;
//	}
//	else if (numArenas > 3) {
//		printf("Number of arguments too large, can only handle maximally 12 images at a time ...\n");
//		return;
//	}
//	else if (numArenas == 1) {
//		w = h = 1;
//		size = 300;
//	}
//	else if (numArenas == 2) {
//		w = 2; h = 1;
//		size = 300;
//	}
//	else if (numArenas == 3) {
//		w = 2; h = 2;
//		size = 300;
//	}
//	// Create a new 1 channel image
//	Mat DispImage = Mat::zeros(Size(100 + size * w, 60 + size * h), CV_8UC1);
//	// Loop for cams.numCameras number of arguments
//	for (i = 0, m = 20, n = 20; i < numArenas; i++, m += (20 + size)) {
//		// Get the Pointer to the IplImage
//		Mat img = allArenas[i].opencvImg;
//		if (img.empty()) {
//			std::cout << "Invalid arguments" << std::endl;
//			return;
//		}
//		x = img.cols;
//		y = img.rows;
//		// Find whether height or width is greater in order to resize the image
//		max = (x > y) ? x : y;
//		// Find the scaling factor to resize the image
//		scale = (float)((float)max / size);
//		if (i % w == 0 && m != 20) {
//			m = 20;
//			n += 20 + size;
//		}
//		Rect ROI(m, n, (int)(x / scale), (int)(y / scale));
//		Mat temp; resize(img, temp, Size(ROI.width, ROI.height));
//		temp.copyTo(DispImage(ROI));
//	}
//	// Create a new window, and show the Single Big Image
//	namedWindow(title, 1);
//	imshow(title, DispImage);
//}
//
//vector<vector<bool>> FishAnalysis::checkWhich2GiveShock(int sElapsed)
//{
//	vector<vector<bool>> fish2giveShock;
//	for (Arena& arena : allArenas)
//		fish2giveShock.push_back(arena.checkWhich2GiveShock(sElapsed));
//	return fish2giveShock;
//}
//
//vector<vector<bool>> FishAnalysis::checkWhich2ReversePattern(int sElapsed)
//{
//	vector<vector<bool>> patterns2reverse;
//	for (Arena& arena : allArenas)
//		patterns2reverse.push_back(arena.checkWhich2ReversePattern(sElapsed));
//
//	return patterns2reverse;
//}
//
//void FishAnalysis::resetShocksOn()
//{
//	for (Arena& arena : allArenas)
//		arena.resetShocksOn();
//}
//
//
//void Arena::initialize(vector<int> yDivs, vector<string> fishIDs)
//{
//	pMOG = cv::createBackgroundSubtractorMOG2(historyLen, binThre, false);
//	cv::namedWindow("MOG2");
//
//	for (int i = 0; i < numFish; i++)
//	{
//		Fish fish(yDivs[i], fishIDs[i]);
//		allFish.push_back(fish);
//	}
//}
//
//void Arena::initialize(vector<int> yDivs)
//{
//	pMOG = cv::createBackgroundSubtractorMOG2(historyLen, binThre, false);
//	for (int i = 0; i < numFish; i++)
//	{
//		Fish fish(yDivs[i]);
//		allFish.push_back(fish);
//	}
//}
//
//void Arena::initialize()
//{
//	// numFish = 1;
//	pMOG = cv::createBackgroundSubtractorMOG2(historyLen, binThre, false);
//	Fish fish;
//	allFish.push_back(fish);
//	sum_curv.resize(20000);
//
//	hShareMem = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4, L"SharedMemory");
//	hShareMem2 = CreateFileMapping(INVALID_HANDLE_VALUE, NULL, PAGE_READWRITE, 0, 4, L"SharedMemory2");
//	derta_angle = static_cast<int*>(MapViewOfFile(hShareMem, FILE_MAP_ALL_ACCESS, 0, 0, 4));
//	derta_dis = static_cast<int*>(MapViewOfFile(hShareMem2, FILE_MAP_ALL_ACCESS, 0, 0, 4));
//	*derta_angle = 0;
//	*derta_dis = 0;
//
//}
//
//void Arena::getImgFromCamera(int width, int height, uint8_t* buffer)
//{
//	// TODO: get the width and height from the image
//	Mat rawImg = Mat(height, width, CV_8UC1, buffer);
//	rawImg.copyTo(opencvImg);
//}
//
//void Arena::getImgFromVideo(cv::VideoCapture cap)
//{
//	cv::Mat rawImg;
//	cap >> rawImg;
//	cv::cvtColor(rawImg, opencvImg, cv::COLOR_RGB2GRAY);//TODO: test this usage
//	cv::GaussianBlur(opencvImg, opencvImg, cv::Size(0, 0), 0.9);
//
//}
//
//void Arena::alignImg(int deg2rotate)
//{
//	Mat rotatedImg;
//	cv::flip(opencvImg, rotatedImg, -1);
//	//rotateImg(opencvImg, rotatedImg, deg2rotate); //TODO: write the implementation
//	//cout << rotatedImg.size() << endl;
//	//cout << opencvImg.size() << endl;
//	opencvImg = rotatedImg; // TODO: check if hardcopy needed?
//}
//
//void Arena::buildBgImg()
//{
//	pMOG->apply(opencvImg, subImg);
//}
//void Arena::buildBgImg_noupdate()
//{
//	pMOG->apply(opencvImg, subImg, 0);
//}
//
//void Arena::showBgImg()
//{
//	//pMOG->apply(opencvImg, subImg);
//	cv::imshow("MOG2", subImg);
//	cv::waitKey(1);
//}
//
//void Arena::findAllFish()
//{
//	vector<vector<Point>> contours;
//	findContours(subImg, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
//
//	const int conSizeThre = 20;
//	vector<vector<Point>> maxContours(4);
//	vector<int> maxContourSizes(4);
//
//	for (auto& contour : contours)
//	{
//		int contourSize = contour.size();
//		if (contourSize < conSizeThre)
//			continue; // skip this turn
//
//		int qIdx = findQuadratile(contour);
//		if (maxContourSizes[qIdx] < contourSize)
//		{
//			maxContourSizes[qIdx] = contourSize;
//			maxContours[qIdx] = contour;
//		}
//	}
//
//	// assign the largest contours to each fish
//	for (int i = 0; i < numFish; i++)
//	{
//		if (maxContourSizes[i] == 0) // fish not found due to no movement
//		{
//			allFish[i].pauseFrames++;
//		}
//		else
//		{
//			allFish[i].pauseFrames = 0;
//			allFish[i].fishContour = maxContours[i];
//			allFish[i].findPosition();
//		}
//	}
//
//}
//
//bool Arena::findSingleFish()
//{
//	tryCatchFalse(numFish == 1,
//		"Wrong numFish. This method is for finding the single fish in the entire image.");
//
//	vector<vector<Point>> contours;
//	findContours(subImg, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
//
//	int contourSizeThre = 20;
//	int maxContourSize = 0;
//	vector<Point> maxContour;
//	for (auto& contour : contours)
//	{
//		int contourSize = contour.size();
//		if (contourSize < contourSizeThre)
//			continue; // skip this turn
//
//		if (maxContourSize < contour.size())
//		{
//			maxContourSize = contour.size();
//			maxContour = contour;
//		}
//	}
//
//	if (maxContourSize == 0)
//	{
//		allFish[0].pauseFrames++;
//		return false; // No fish found
//	}
//	else
//	{
//		allFish[0].pauseFrames = 0;
//		allFish[0].fishContour = maxContour;
//		allFish[0].findPosition();
//		return true; // fish found
//	}
//
//
//}
//bool Arena::findSingleFish4test()
//{
//	tryCatchFalse(numFish == 1,
//		"Wrong numFish. This method is for finding the single fish in the entire image.");
//
//	Mat out, out2;
//	Mat element = getStructuringElement(cv::MORPH_RECT, Size(7, 7));
//	morphologyEx(subImg, out, cv::MORPH_CLOSE, element);
//	//morphologyEx(out, out2, cv::MORPH_OPEN, element);
//	vector<vector<Point>> contours;
//	findContours(out, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
//
//	int contourSizeThre = 50;
//	int contourSizeThre_max = 700;
//	int maxContourSize = 0;
//	vector<Point> maxContour;
//	for (auto& contour : contours)
//	{
//		int contourSize = contour.size();
//		if (contourSize < contourSizeThre)
//			continue; // skip this turn
//		if (contourSize > contourSizeThre_max)
//			continue; // skip this turn
//
//		if (maxContourSize < contour.size())
//		{
//			maxContourSize = contour.size();
//			maxContour = contour;
//		}
//	}
//
//	if (maxContourSize == 0)
//	{
//		allFish[0].pauseFrames++;
//		return false; // No fish found
//	}
//	else
//	{
//		allFish[0].pauseFrames = 0;
//		allFish[0].fishContour = maxContour;
//		allFish[0].findPosition();
//		allFish[0].findTail();
//
//
//		cv::drawContours(opencvImg, contours, -1, Scalar(0, 0, 0), 3);
//		for (auto tailPoint : allFish[0].fishTail) {
//			cv::circle(opencvImg, (tailPoint + Point(50, 0)), 2, Scalar(0, 0, 0), -1);
//		}
//
//
//		std::stringstream ss;
//		string str;
//		ss << allFish[0].sum_curv;
//		str = ss.str();
//		cv::putText(opencvImg, str, Point(0, 40), cv::FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 0));
//		cv::imshow("test", opencvImg);
//		cv::waitKey(33);
//		return true; // fish found
//
//	}
//
//
//}
//
//void Arena::findAllFish(std::vector<int> corner_x, std::vector<int> corner_y, int width, int height) {
//	for (int i = 0; i < numFish; i++) {
//		cv::Rect rect(corner_x[i], corner_y[i], width, height);
//		Mat ROI = subImg(rect);
//
//		//cv::imwrite("E:/test/" + ImgName[i], ROI);
//
//		vector<vector<Point>> contours;
//		findContours(ROI, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);
//
//		int contourSizeLowerThre = 20;
//		int contourSizeUpperThre = 150;
//
//		int maxContourSize = 0;
//		vector<Point> maxContour;
//		for (auto& contour : contours)
//		{
//			int contourSize = contour.size();
//			if (contourSize < contourSizeLowerThre || contourSize>contourSizeUpperThre)
//				continue; // skip this turn
//
//			if (maxContourSize < contour.size())
//			{
//				maxContourSize = contour.size();
//				maxContour = contour;
//			}
//		}
//
//		if (maxContourSize == 0)
//		{
//			allFish[i].pauseFrames++;
//			//return false; // No fish found
//		}
//		else
//		{
//			allFish[i].pauseFrames = 0;
//			allFish[i].fishContour = maxContour;
//			allFish[i].findPosition();
//			//return true; // fish found
//		}
//
//		if (allFish[i].head.x == -1) // invalid fish analysis data
//			continue;
//		circle(opencvImg, allFish[i].head + cv::Point(corner_x[i], corner_y[i]), 5, Scalar(255), 2);
//		circle(opencvImg, allFish[i].tail + cv::Point(corner_x[i], corner_y[i]), 3, Scalar(255), 2);
//
//		if (allFish[i].inCD) {
//			cv::putText(opencvImg, "shockOn", cv::Point(corner_x[i], corner_y[i] + 10), cv::FONT_HERSHEY_SIMPLEX, 1, Scalar(255));
//		}
//
//		switch (allFish[i].idxCase)
//		{
//		case 0:
//			cv::rectangle(opencvImg, cv::Point(corner_x[i], corner_y[i]), cv::Point(corner_x[i], corner_y[i]) + cv::Point(width / 2, height), Scalar(255), 2);
//			break;
//		case 1:
//			cv::rectangle(opencvImg, cv::Point(corner_x[i], corner_y[i]) + cv::Point(width / 2, 0), cv::Point(corner_x[i], corner_y[i]) + cv::Point(width, height), Scalar(255), 2);
//			break;
//		default:
//			break;
//		}
//
//	}
//
//
//}
//
//
//int Arena::findQuadratile(vector<Point> contour)
//{
//	Moments M = moments(contour); // to find the center
//	int x = int(M.m10 / M.m00);
//	int y = int(M.m01 / M.m00);
//	Point center = Point(x, y);
//
//	int signNum = (x - xCut > 0) + 2 * (y - yCut > 0);
//	int qIdx = 0; // Quadrantic index
//	switch (signNum)
//	{
//	case 0:// No.0 Quadrant
//		qIdx = 0;
//		return qIdx;
//		break;
//	case 1:
//		qIdx = 1;
//		return qIdx;
//		break;
//	case 2:
//		qIdx = 2;
//		return qIdx;
//		break;
//	case 3:
//		qIdx = 3;
//		return qIdx;
//		break;
//	default:
//		cout << "Fish Analysis Error" << endl;
//		exit(0);
//	}
//}
//
//void Arena::annotateFish() {
//	for (int j = 0; j < numFish; j++)
//	{
//		/*
//		int pIdx = screen.allAreas[i].allPatches[j].pIdx;
//		if (pIdx == 0)
//		putText(allArenas[i].opencvImg, "CS TOP", Point(10, 45), FONT_HERSHEY_TRIPLEX, 1, Scalar::all(255), 2);
//		else if (pIdx == 1)
//		putText(allArenas[i].opencvImg, "CS BOTTOM", Point(10, 45), FONT_HERSHEY_TRIPLEX, 1, Scalar::all(255), 2);
//		*/
//		if (allFish[j].head.x == -1) // invalid fish analysis data
//			continue;
//		circle(opencvImg, allFish[j].head, 5, Scalar(255), 2);
//		circle(opencvImg, allFish[j].tail, 3, Scalar(255), 2);
//
//		if (allFish[j].idxCase)
//			cv::rectangle(opencvImg, cv::Point(410, 1), cv::Point(799, 499), Scalar(255), 2);
//		else
//			cv::rectangle(opencvImg, cv::Point(1, 1), cv::Point(410, 499), Scalar(255), 2);
//
//	}
//}
//
//void Arena::displayImg(std::string title)
//{
//	imshow(title, opencvImg);
//	cv::waitKey(1);
//}
//
//vector<bool> Arena::checkWhich2GiveShock(int sElapsed)
//{
//	vector<bool> fish2giveShock;
//	for (Fish& fish : allFish)
//		fish2giveShock.push_back(fish.checkIfGiveShock2(sElapsed));
//	return fish2giveShock;
//}
//
//vector<bool> Arena::checkWhich2ReversePattern(int sElapsed)
//{
//	vector<bool> patterns2reverse;
//	for (Fish& fish : allFish)
//		patterns2reverse.push_back(fish.checkIfReversePattern(sElapsed));
//	return patterns2reverse;
//}
//
//
//void Arena::resetShocksOn() {
//	for (Fish& fish : allFish)
//		fish.shockOn = 0;
//}
//
//int Arena::getNumFish()
//{
//	return allFish.size();
//}
//
//void Arena::checkBout(int Frame_idx) {
//	sum_curv[Frame_idx] = allFish[0].sum_curv;
//	if (fabs(allFish[0].sum_curv) > 0.6 && (!bout_start)) {
//		bout_start = true;
//		bout_start_idx = Frame_idx;
//		cout << "find bout" << endl;
//	}
//	if (Frame_idx == bout_start_idx + 13) {
//		bout_start = false;
//		cout << "predict_angle" << endl;
//		//cout << bout_start_idx << endl;
//		//cout << sum_curv[bout_start_idx] << endl;
//		predict_angle(&(sum_curv[bout_start_idx]));
//		//predict_distance(&(sum_curv[bout_start_idx]));
//		cout << "predict_end" << endl;
//
//		bout_start_idx = -20;
//
//	}
//}
//void Arena::checkBout_frame(int Frame_idx) {
//	sum_curv[Frame_idx] = allFish[0].sum_curv;
//	if (fabs(allFish[0].sum_curv) > 0.6 && (!bout_start)) {
//		bout_start = true;
//		bout_start_idx = Frame_idx;
//		allFish[0].virtual_fish_headingAngle = allFish[0].headingAngle;
//		allFish[0].virtual_fish_head = allFish[0].head;
//		cout << "find bout" << endl;
//	}
//	if (bout_start) {
//		//derta_angle=predict_angle(&(sum_curv[Frame_idx -4]));
//		//cout << "derta_angle:" << derta_angle << endl;
//		*derta_angle = int(1.46 * (sum_curv[Frame_idx] + 0.9 * sum_curv[Frame_idx - 1] + 0.81 * sum_curv[Frame_idx - 2]));
//		*derta_dis = 20;
//		//derta_dis = predict_distance(&(sum_curv[Frame_idx -4]));
//		//cout << "derta_dis:" << derta_dis << endl;
//		//Point C2H = allFish[0].head - allFish[0].center; // tail to head, only applied to small fish
//
//		//allFish[0].derta_x = 3.8 * cos(allFish[0].virtual_fish_headingAngle * 180 / PI);
//		//allFish[0].derta_y = 3.8 * sin(allFish[0].virtual_fish_headingAngle * 180 / PI);
//		//allFish[0].virtual_fish_head.x += allFish[0].derta_x;
//		//allFish[0].virtual_fish_head.y += allFish[0].derta_y;
//
//	}
//	if (Frame_idx == bout_start_idx + 10) {
//		bout_start = false;
//		cout << "predict_end" << endl;
//		bout_start_idx = -20;
//		cout << "virtual_fish_headingAngle:" << allFish[0].virtual_fish_headingAngle << endl;
//		cout << "headingAngle:" << allFish[0].headingAngle << endl;
//
//		cout << "virtual_fish_head:" << allFish[0].virtual_fish_head.x << ',' << allFish[0].virtual_fish_head.y << endl;
//		cout << "fish_head" << allFish[0].head.x << ',' << allFish[0].head.y << endl;
//
//
//	}
//}


//
//
//
//void Fish::findPosition()
//{
//	Vec4f lineVec;
//	fitLine(fishContour, lineVec, CV_DIST_L2, 0, 0.01, 0.1);
//	double stepSize = 100;
//	vector<int> endIdx = findPtsLineIntersectContour(
//		fishContour,
//		Point2f(lineVec[2] - stepSize * lineVec[0], lineVec[3] - stepSize * lineVec[1]), // on one side, far from the center point
//		Point2f(lineVec[2] + stepSize * lineVec[0], lineVec[3] + stepSize * lineVec[1])// on the other side, far from the center point
//	);
//
//	findHeadAndTail(endIdx);
//	calcHeadingAngle();
//
//}
//
//
//void Fish::findHeadAndTail(vector<int> endIndices)
//{
//	Moments M = moments(fishContour); // to find the center
//	center = Point(M.m10 / M.m00, M.m01 / M.m00);
//	Point EP1 = fishContour[endIndices[0]];
//	Point EP2 = fishContour[endIndices[1]];
//	if (norm(EP1 - center) < norm(EP2 - center))
//	{
//		head = EP1;
//		head_idx = endIndices[0];
//		tail = EP2;
//	}
//	else
//	{
//		head = EP2;
//		head_idx = endIndices[1];
//		tail = EP1;
//	}
//}
//
//void Fish::findTail()
//{
//	Point curPt; // current point
//	double distThre = 1.5; // threshold to decide whether it is an intersection pt
//	double headSize = 34.0;
//	int idxA = 0;
//	int idxB = fishContour.size();
//	int tailSize;
//	int sub_tailSize;
//	int tailPt_a_idx;
//	int tailPt_b_idx;
//	bool findPt = false;
//	for (int i = 0; i < fishContour.size(); i++)
//	{
//		curPt = fishContour[i];
//
//		double pt2head = abs(cv::norm(curPt - head) - headSize);
//		if (pt2head < distThre)
//		{
//			if (i > idxA)
//				idxA = i;
//			if (i < idxB)
//				idxB = i;
//			findPt = true;
//		}
//
//	}
//	if (!findPt)
//		return;
//	fishTail.resize(14);
//	if (head_idx< idxA && head_idx>idxB) {
//		tailSize = fishContour.size() - (idxA - idxB);
//		sub_tailSize = floor(tailSize / 26);
//		for (int i = 0; i < 14; i++) {
//			if ((idxA + i * sub_tailSize) >= fishContour.size())
//				tailPt_a_idx = idxA + i * sub_tailSize - fishContour.size();
//			else
//				tailPt_a_idx = idxA + i * sub_tailSize;
//			if ((idxB - i * sub_tailSize) < 0)
//				tailPt_b_idx = idxB - i * sub_tailSize + fishContour.size();
//			else
//				tailPt_b_idx = idxB - i * sub_tailSize;
//			fishTail[i] = ((fishContour[tailPt_a_idx] + fishContour[tailPt_b_idx]) / 2);
//		}
//	}
//	else {
//		tailSize = idxA - idxB;
//		sub_tailSize = floor(tailSize / 26);
//		for (int i = 0; i < 14; i++) {
//			tailPt_a_idx = idxA - i * sub_tailSize;
//			tailPt_b_idx = idxB + i * sub_tailSize;
//			fishTail[i] = ((fishContour[tailPt_a_idx] + fishContour[tailPt_b_idx]) / 2);
//		}
//	}
//	Point fishVector_a, fishVector_b;
//	double tailAngle_a, tailAngle_b;
//	//fishHeadVector = center - head;
//	double subAngle;
//	sum_curv = 0;
//	for (int i = 0; i < 11; i++) {
//		fishVector_a = fishTail[i] - fishTail[i + 1];
//		tailAngle_a = atan2(fishVector_a.y, fishVector_a.x);
//		fishVector_b = fishTail[i + 1] - fishTail[i + 2];
//		tailAngle_b = atan2(fishVector_b.y, fishVector_b.x);
//		subAngle = tailAngle_b - tailAngle_a;
//		if (subAngle > PI)
//			subAngle = subAngle - PI * 2.0;
//		else if (subAngle < PI * (-1))
//			subAngle = subAngle + PI * 2.0;
//
//		sum_curv = sum_curv + subAngle;
//
//	}
//	if (sum_curv > 1000 || sum_curv < -1000) {
//		sum_curv = 0;
//	}
//
//}
//
//void Fish::calcHeadingAngle()
//{
//	Point C2H = head - center; // tail to head, only applied to small fish
//	headingAngle = atan2(C2H.y, C2H.x) * 180 / PI;
//}
//
///*
//	Determine which side is fish's head
//	by measuring the area of each half
//*/
//bool Fish::findHeadSide(Point2f* M)
//{
//	vector<int> indices = findPtsLineIntersectContour(fishContour, M[0], M[2]);
//	int idxMidPt = (indices[0] + indices[1]) / 2;
//	Point2f midPtContour = fishContour[idxMidPt];
//	double dist2line = getPt2LineDistance(M[1], M[0], M[2]);
//	double dist2pt = norm(midPtContour - M[1]);
//
//	vector<vector<Point>> contourHalves(2);
//	if (indices[0] == indices[1])
//	{
//		cout << "The contour is too thin to cut in-between" << endl;
//		return false;
//	}
//	else {
//		// contour half of negative skewness contains refPt
//		contourHalves[0].insert(contourHalves[0].end(), fishContour.begin() + indices[0], fishContour.begin() + indices[1]);
//		contourHalves[1].insert(contourHalves[1].end(), fishContour.begin() + indices[1], fishContour.end());
//		contourHalves[1].insert(contourHalves[1].end(), fishContour.begin(), fishContour.begin() + indices[0]);
//	}
//
//	vector<int> areas(2);
//	areas[0] = contourArea(contourHalves[0]);
//	areas[1] = contourArea(contourHalves[1]);
//
//	int aIdx; // index of area where refPt is in.
//	if (dist2line < dist2pt) // refPt is not in-between idx0 -> idx1
//		aIdx = 1;
//	else
//		aIdx = 0;
//	return areas[aIdx] > areas[!aIdx];
//}
//void Fish::findFishTail() {
//
//	//Find the tail of fish
//	Point topEnd, tailPt_a, tailPt_b;
//
//	vector<int> tail_idx;
//	vector<double> tailingAngle;
//	//double Pt2center = norm(fishContour[0] - center);
//	//topEnd = fishContour[0];
//
//	/*for (int i = 1; i < fishContour.size(); i++)
//	{
//		double curPt2center = norm(fishContour[i] - center);
//		if (Pt2center < curPt2center) {
//			topEnd = fishContour[i];
//			Pt2center = curPt2center;
//		}
//	}*/
//	fishTail.resize(13);
//	Point tailAxis = head - tail;
//	//fishTail[0] = tail;
//	for (int i = 0; i < 13; i++) {
//		tailPt_a = tail + tailAxis * i / 20 + Point(tailAxis.y, -tailAxis.x) / 4;
//		tailPt_b = tail + tailAxis * i / 20 + Point(-tailAxis.y, tailAxis.x) / 4;
//		tail_idx = findPtsLineIntersectContour(fishContour, tailPt_a, tailPt_b);
//		fishTail[i] = ((fishContour[tail_idx[0]] + fishContour[tail_idx[1]]) / 2);
//	}
//
//	//Calculate the angle
//	Point fishVector_a, fishVector_b;
//	//fishHeadVector = center - head;
//	double sinfi;
//	/*for (auto tailPoint : fishTail) {
//		fishTailVector = tailPoint - center;
//		sinfi = -(fishHeadVector.x * fishTailVector.y - fishTailVector.x * fishHeadVector.y) / (norm(fishHeadVector) * norm(fishTailVector));
//		tailingAngle.push_back(asin(sinfi));
//	}*/
//	sum_curv = 0;
//	for (int i = 0; i < 11; i++) {
//		fishVector_a = fishTail[i] - fishTail[i + 1];
//		fishVector_b = fishTail[i + 1] - fishTail[i + 2];
//		sinfi = -(fishVector_a.x * fishVector_b.y - fishVector_b.x * fishVector_a.y) / (norm(fishVector_a) * norm(fishVector_b));
//		sum_curv = sum_curv + asin(sinfi);
//	}
//}


//bool Fish::checkIfGiveShock(int sElapsed) {
//	/* Control parameters */
//	int thinkingTime = 7; // seconds, give fish some thinking time
//	int shockCD = 3; // seconds
//	/* Give fish a shock whenever it stays in CS area too long */
//	int CStimeThre = 10;
//	shockOn = false;
//    if (idxCase == 2) // blackout
//		return false;
//	if (idxCase < lastTimeUpdatePattern + thinkingTime)
//		return false;
//	if (idxCase < lastShockTime + shockCD)
//		return false;
//	if (head.x == -1) // invalid frame
//		return false;
//	if (idxCase) // patternIdx == 1, since 2 is already excluded
//	{ 
//		if (head.y < yDiv) // in non-CS area
//			shockOn = false;
//		else {
//			lastTimeInCS = sElapsed;
//			if (sElapsed - lastShockTime > CStimeThre)
//				shockOn = true;
//			else {
//				if (headingAngle < 0) // fish is trying to escape CS area
//					shockOn = false;
//				else
//					shockOn = true;
//			}
//		}
//	}-
//	else
//	{
//		if (head.y > yDiv)
//			shockOn = false;
//		else {
//			lastTimeInCS = sElapsed;
//			if (sElapsed - lastShockTime > CStimeThre)
//				shockOn = true;
//			else {
//				if (headingAngle > 0) // fish is trying to escape CS area
//					shockOn = false;
//				else
//					shockOn = true;
//			}
//		}
//	}
//	if (shockOn)
//		lastShockTime = sElapsed;
//
//	return shockOn;
//}
//
//bool Fish::checkIfGiveShock2(int sElapsed) {
//	/* Control parameters */
//	int thinkingTime = 5; // seconds, give fish some thinking time
//	int shockCD = 3; // seconds
//	/* Give fish a shock whenever it stays in CS area too long */
//	//int CStimeThre = 10;
//	shockOn = false;
//	inCD = false;
//	if (idxCase == 2) // blackPatch
//		return false;
//	if (sElapsed < lastTimeUpdatePattern + thinkingTime)
//		return false;
//	if (sElapsed < lastShockTime + shockCD)
//		inCD = true;
//	return false;
//	if (head.x == -1) // invalid frame
//		return false;
//	if (idxCase) // patternIdx == 1, since 2 is already excluded
//	{
//		if (head.x > xDiv) // in non-CS area
//			shockOn = false;
//		else {
//			if (headingAngle > (-90) && headingAngle < 90)
//				shockOn = false;
//			else
//				shockOn = true;
//		}
//	}
//	else
//	{
//		if (head.x < xDiv)
//			shockOn = false;
//		else {
//			if (!(headingAngle > (-90) && headingAngle < 90))
//				shockOn = false;
//			else
//				shockOn = true;
//		}
//	}
//	if (shockOn) {
//		lastShockTime = sElapsed;
//		inCD = true;
//	}
//
//
//	return shockOn;
//}
bool Fish::checkIfGiveShock2Test(int sElapsed) {
	/* Control parameters */
	int thinkingTime = 5; // seconds, give fish some thinking time
	int shockCD = 3; // seconds
	/* Give fish a shock whenever it stays in CS area too long */
	//int CStimeThre = 10;
	shockOn = false;
	inCD = false;
	if (idxCase == 2) // blackPatch
		return false;
	if (idxCase == 3) // fullPatch
		return false;
	if (sElapsed < lastTimeUpdatePattern + thinkingTime)
		return false;
	if (sElapsed < lastShockTime + shockCD) {
		inCD = true;
		return false;
	}

	if (head.x == -1) // invalid frame
		return false;
	if (idxCase) // patternIdx == 1, since 2 is already excluded
	{
		if (head.x > xDiv) // in non-CS area
			shockOn = false;
		else {

			shockOn = true;
		}
	}
	else
	{
		if (head.x < xDiv)
			shockOn = false;
		else {

			shockOn = true;
		}
	}
	if (shockOn) {
		lastShockTime = sElapsed;
		inCD = true;
	}


	return shockOn;
}
//int Fish::generatePattern(int Count) {
//
//
//	if (idxCase == 2) {
//
//		pattern = 2;
//		return pattern;
//	}
//	if (head.x == -1) {
//		pattern = 2;
//		return pattern;
//	}
//	bool high_frequency;
//	if (idxCase) // patternIdx == 1, since 2 is already excluded
//	{
//		if (head.x > xDiv) // in non-CS area
//			high_frequency = false;
//		else {
//
//			high_frequency = true;
//		}
//	}
//	else
//	{
//		if (head.x < xDiv)
//			high_frequency = false;
//		else {
//
//			high_frequency = true;
//		}
//	}
//
//	if (high_frequency) {
//		if (Count % 2 == 0) {
//			pattern = 3;
//		}
//		else
//			pattern = 2;
//	}
//	else {
//		//if (Count % 6 == 0) {
//		//	pattern = 3;
//		//}
//		//else
//			pattern = 2;
//	}
//
//	return pattern;
//}
void Fish::getPosition(int platformX, int platformY) {
	head.x = 246 - platformX;
	head.y = 167 - platformY;
}
int Fish::generatePattern(int Count) {


	if (idxCase == 2) {

		pattern = 2;
		return pattern;
	}
	if (idxCase == 3) {

		pattern = 3;
		return pattern;
	}
	/*if (head.x == -1) {
		pattern = 2;
		return pattern;
	}*/
	bool high_frequency;
	if (idxCase) // patternIdx == 1, since 2 is already excluded
	{
		if (head.x > xDiv) // in non-CS area
			high_frequency = true;
		else {

			high_frequency = false;
		}
	}
	else
	{
		if (head.x < xDiv)
			high_frequency = true;
		else {

			high_frequency = false;
		}
	}

	if (high_frequency) {
		//if (Count % 2 == 0) {
		pattern = 3;
		//}
		//else
			//pattern = 2;
	}
	else {
		//if (Count % 6 == 0) {
		//	pattern = 3;
		//}
		//else
		pattern = 2;
	}

	return pattern;
}

bool Fish::checkIfGiveShockForAnti(int sElapsed) {
	/* Control parameters */
	int thinkingTime = 3; // seconds, give fish some thinking time
	int shockCD = 3; // seconds
	/* Give fish a shock whenever it stays in CS area too long */
	//int CStimeThre = 10;
	shockOn = false;
	inCD = false;
	if (idxCase == 2) // blackPatch
		return false;
	if (sElapsed < lastTimeUpdatePattern + thinkingTime)
		return false;
	if (sElapsed < lastShockTime + shockCD) {
		inCD = true;
		return false;
	}

	if (head.x == -1) // invalid frame
		return false;
	if (idxCase) // patternIdx == 1, since 2 is already excluded
	{
		if (head.x < xDiv) // in non-CS area
			shockOn = false;
		else {

			shockOn = true;
		}
	}
	else
	{
		if (head.x > xDiv)
			shockOn = false;
		else {

			shockOn = true;
		}
	}
	if (shockOn) {
		lastShockTime = sElapsed;
		inCD = true;
	}


	return shockOn;
}

bool Fish::checkIfReversePattern(int sElapsed)
{
	int NCStimeThre = 48; // seconds
	if (sElapsed - lastTimeInCS > NCStimeThre)
	{
		idxCase = !idxCase;
		return true;
	}
	else
		return false;
}

void Fish::setNewPattern() {
	if (head.x < xDiv)
		idxCase = 1;
	else
		idxCase = 0;
}
void Fish::setNewPatternForAnti() {
	if (head.x < xDiv)
		idxCase = 0;
	else
		idxCase = 1;
}


/*Find the closest point on the contour to the reference point, return the index findClosestPt*/
int findClosestPt(vector<Point>& contour, Point point)
{
	double minDist = 1000000;
	double tempDist = 0;
	Point temp;
	int goodIndex = 0;
	for (int i = 0; i < contour.size(); i++)
	{
		temp = contour[i];
		tempDist = norm(contour[i] - point);

		if (tempDist < minDist)
		{
			goodIndex = i;
			minDist = tempDist;
		}
	}

	return goodIndex;
}



/* Get the Euclidean distance from point P to line AB */
double getPt2LineDistance(Point2f P, Point2f A, Point2f B)
{
	Point2f BA = B - A; // the vector from A to B
	//Point2f PA = P - A; // the vector from A to P
	double dist = abs(BA.y * P.x - BA.x * P.y + B.cross(A)) / norm(BA);
	return dist;
}

/* Find 2 intersection points of a line (AB) and contour */
vector<int> findPtsLineIntersectContour(vector<Point>& contour, Point2f A, Point2f B)
{
	vector<int> goodIndices(2);
	Point2f curPt; // current point
	vector<Point> ptList; // store potential intersection points
	vector<int> idxList; // store indices of potential intersection points
	double distThre = 1.0; // threshold to decide whether it is an intersection pt
	for (int i = 0; i < contour.size(); i++)
	{
		curPt = contour[i];
		double pt2line = getPt2LineDistance(curPt, A, B);
		if (pt2line < distThre)
		{
			ptList.push_back(contour[i]);
			idxList.push_back(i);
		}

	}
	if (ptList.empty()) {
		curPt = contour[0];
		double closestPt2line = getPt2LineDistance(curPt, A, B);
		int closestPt = 0;
		for (int i = 1; i < contour.size(); i++)
		{
			curPt = contour[i];
			double pt2line = getPt2LineDistance(curPt, A, B);
			if (pt2line < closestPt2line)
			{
				closestPt = i;
			}

		}
		ptList.push_back(contour[closestPt]);
		idxList.push_back(closestPt);
	}

	// assign intersection points to each side
	int idxA = findClosestPt(ptList, A); // the one closest to A
	int idxB = findClosestPt(ptList, B); // the one closest to B

	if (idxA < idxB)
	{
		goodIndices[0] = idxList[idxA];
		goodIndices[1] = idxList[idxB];
	}
	else
	{
		goodIndices[0] = idxList[idxB];
		goodIndices[1] = idxList[idxA];
	}


	return goodIndices;

}

void rotateImg(Mat src, Mat dst, int deg2rotate)
{
	if (deg2rotate == 0)
		dst = src;
	else if (deg2rotate == 90)
		transpose(src, dst);
	else if (deg2rotate == 180)
		flip(src, dst, -1);
	else if (deg2rotate == 270)
	{
		Mat temp;
		flip(src, temp, 0);
		transpose(temp, dst);
	}
	else {
		cout << "Invalid deg2rotate! Only 0, 90, 180, 270 degrees are available!" << endl;
	}
}

void rot90CW(Mat src, Mat dst)
{
	Mat temp;
	flip(src, temp, 0);
	transpose(temp, dst);
}

void onChange(int, void*) {
	if (sum_curv_baseline_positive > 0)
		sum_curv_baseline = sum_curv_baseline_positive / 100.0;
	if (sum_curv_baseline_negative > 0)
		sum_curv_baseline = -1.0 * sum_curv_baseline_negative / 100.0;

}



void onChange2(int, void*) {

}


void TrackbarInitialize() {
	cv::namedWindow("setValue", cv::WINDOW_NORMAL);
	cv::createTrackbar("sum_curv_baseline_positive", "setValue", &sum_curv_baseline_positive, 400, onChange);
	cv::createTrackbar("sum_curv_baseline_negative", "setValue", &sum_curv_baseline_negative, 400, onChange);
	onChange(0, 0);
	cv::createTrackbar("Magnification_forward", "setValue", &Magnification_forward, 30, onChange2);
	//onChange3(0, 0);
	cv::createTrackbar("Magnification_turn", "setValue", &Magnification_turn, 30, onChange2);
	cv::createTrackbar("checkbout_start", "setValue", &checkbout, 1, onChange2);
	cv::createTrackbar("experiment_start", "setValue", &experiment, 1, onChange2);
	//onChange4(0, 0);
}
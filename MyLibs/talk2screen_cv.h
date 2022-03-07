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
* Filename: talk2screen.h
* Abstract: this file contains all classes and functions' declarations
*			used to present visual patterns
*
* Current Version: 3.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 15, 2018
* Modified on: Apr. 20, 2019
*/

#ifndef _GUARD_TALK2SCREEN_CV_H
#define _GUARD_TALK2SCREEN_CV_H


/* Disable warning to function std::copy */
#pragma warning(disable : 4996)

// Include OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

// Include standard libraries
#include <vector>
#include <iostream>



using namespace std;
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

#define RED 1;
#define BLUE 2;
#define BLACK 10;
#define TEST_PATTERN 11;




class Patch
{
protected:
	//Point corner;//upper-left corner
	cv::Mat PatchImage;
public:
	Patch(
		int corner_x,
		int corner_y
		//cv::Mat patchImage = cv::Mat(88, 158, CV_8UC3, cv::Scalar(0, 0, 0))
	)
		:corner_x(corner_x),
		corner_y(corner_y)
	{
		PatchImage = cv::Mat(corner_y, corner_x, CV_8UC3, cv::Scalar(0, 0, 0));

	}
	void drawRect(int* corner, int* color);
	void drawRect(int* corner, cv::Mat pattern);
	cv::Mat getImage();

	int midLine[4] = { 78,0,80,88 };
	int white[3] = { 255,255,255 };
	int corner_x;
	int corner_y;
};

class Screen
{
protected:
	//cv::Mat srcImage;
public:
	Screen()
	{


	}
	void initialize();
	void initialize(int pattern);

	void initializeFullPattren(int* fullPattern, int corner_x, int corner_y);
	void initializePattren(Mat pattern, int corner_x, int corner_y);
	//void refresh();

	void refresh(int IdxCase, string window_name, int x, int y);


	void test();

	//void show();

	Patch* leftPatch;
	Patch* rightPatch;
	Patch* blackPatch;
	Patch* fullPatch;

	int ROI_col;
	int ROI_row;
	cv::Mat srcImage;
	cv::Mat pattern;
	cv::Mat ROI;
	int leftPatchCorner[4];
	int rightPatchCorner[4];
	int fullPatchCorner[4];
	//pattern
	int red[3] = { 0,0,255 };
	int black[3] = { 0,0,0 };
	int blue[3] = { 255,0,0 };
	int green[3] = { 0,255,0 };
	int yellow[3] = { 0,255,255 };
	int white[3] = { 255,255,255 };
	int windowX = -100;
	int windowY = -100;
	//cv::Mat test_pattern = cv::imread("D:/working/free-moving learning/code/free-moving/free-moving/test.jpg");


};


#endif // !_GUARD_TALK2SCREEN_CV_H


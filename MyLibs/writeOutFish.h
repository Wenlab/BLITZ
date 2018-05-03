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
* Filename: writeOutFish.h
* Abstract: this file contains all classes and functions' declarations
*			used to write experiment info and extracted motion parameters
*			into yaml files and save videos for each camera
*
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: Apr. 28, 2018

* Replaced Version: 1.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
*/


#ifndef _GUARD_WRITEOUTFISH_H
#define _GUARD_WRITEOUTFISH_H

// Include OpenCV libraries
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/video.hpp>

// Include standard libraries
#include <iostream>


class WriteOutData
{
private:
	;// nothing for now
public:
	// methods
	WriteOutData()
	{

	}
	bool initialize(int numCameras, int frameRate, int width, int height);
	
	
	// properties
	std::vector<cv::FileStorage> yamlVec;
	std::vector<cv::VideoWriter> videoVec;
};

template <typename T>
void writeOutVarInline(cv::FileStorage fs, T var, std::string vName)
{
	fs << "Frame" << "[";
	fs << "{:" << vName << var << "}";
	fs << "]";
};



#endif // !_GUARD_WRITEOUTFISH_H


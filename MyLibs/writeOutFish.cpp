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
* Abstract: this file contains all function definitions
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

// Include user-defined libraries
#include "writeOutFish.h"


using namespace std;
using namespace cv;

bool WriteOutData::initialize(int numCameras, int width, int height, int frameRate = 10)
{
	const string pathName = "D:/ExpData/";
	for (int i = 0; i < numCameras; i++)
	{
		string expName, videoName, yamlName;
		cout << "Enter the fileName (Date_fish1ID_Age_fish2ID_Age_task)" << endl;
		cin >> expName;
		videoName = pathName + expName + ".avi";
		yamlName = pathName + expName + ".yaml";
		FileStorage fObj(yamlName, FileStorage::WRITE);
		yamlVec.push_back(fObj);
		VideoWriter vObj(videoName, CV_FOURCC('D', 'I', 'V', 'X'), frameRate, Size(width, height), false);
		videoVec.push_back(vObj);
	}
	return true;
}
/*
template <typename T>
void writeOutVarInline(FileStorage yaml, T var, string vName)
{
	yaml << "{:" << vName << var << "}";
}
*/

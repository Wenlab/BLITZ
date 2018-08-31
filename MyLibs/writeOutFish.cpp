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
#include <chrono>
#include <algorithm>  // include the algorithm reverse


using namespace std;
using namespace cv;

bool WriteOutData::initialize(string pathName,string contentName, int width, int height, int frameRate)
{
	string videoName = pathName + contentName + ".avi";
	string yamlName = pathName + contentName + ".yaml";
	FileStorage fObj(yamlName, FileStorage::WRITE);
	if (!fObj.isOpened())
		return false;
	yamlVec.push_back(fObj);
	VideoWriter vObj(videoName, CV_FOURCC('D', 'I', 'V', 'X'), frameRate, Size(width, height), false);
	if (!vObj.isOpened())
		return false;
	videoVec.push_back(vObj);
	return true;
	
}

/* Get strain name of fish */
string get_strainName(char firstChar)
{
	string strainName;
	if (firstChar == 'G' || firstChar == 'g') // GCaMP fish
		strainName += "GCaMP";
	else if (firstChar == 'S' || firstChar == 's')
		strainName += "WT";
	return strainName;
}

/* Get CS string to append to the filenames of yaml and video files */
string get_CS_string(string CSpattern)
{
	// TODO: get the abbreviation by erasing all lower-case alphabetas
	string CSstr;
	if (CSpattern.compare("redBlackCheckerboard") == 0)
		CSstr = "RBC";
	else if (CSpattern.compare("whiteBlackCheckerboard") == 0)
		CSstr = "WBC";
	else if (CSpattern.compare("pureBlack") == 0)
		CSstr = "PB";
	else
	{
		CSstr = CSpattern;
		cout << "CS pattern is: " << CSstr << endl;

	}
	return CSstr;
}

/* Extract the pattern name from the filename 
 by erasing all lower-case alphabetas
*/
string extractPatternName(const char* fileName)
{
	int startIdx, endIdx;
	string s(fileName);
	std::reverse(s.begin(), s.end());
	int foundIdx = s.find('/');
	if (foundIdx != string::npos)
		startIdx = s.size() - foundIdx;
	else
		startIdx = 0;

	int foundIdx = s.find('.');
	if (foundIdx != string::npos)
		endIdx = s.size() - foundIdx;
	else {
		cout << "Wrong input pattern directory!" << endl;
		cout << "Please check!" << endl;
		exit(0);
	}
	
	std::reverse(s.begin(), s.end()); // reverse back
	string patternName = s.substr(startIdx + 1, endIdx - 1);


}

/* Get current date and time string from chrono system clock */
string get_current_date_time()
{
	// TODO: use a more elegant way to get the date and time (precise to seconds)
	// Get system time
	chrono::system_clock::time_point p = chrono::system_clock::now();
	time_t t = chrono::system_clock::to_time_t(p);
	char timeS[26]; // time string
	ctime_s(timeS, sizeof timeS, &t);

	vector<string> timeVec;
	istringstream ss(timeS);
	while (ss.good())
	{
		string subStr;
		getline(ss, subStr, ' ');
		timeVec.push_back(subStr);
	}

	string timeStr = "2018"; // add year
	if (timeVec[1].compare("May")==0) // add month
		timeStr += "05";
	else if (timeVec[1].compare("Jun")==0)
		timeStr += "06";
	else if (timeVec[1].compare("Jul")==0)
		timeStr += "07";

	// add day, separator, to separator date and time
	timeStr += timeVec[2] + "_";
	ss.clear();
	ss.str(timeVec[3]);
	timeVec.clear();

	while (ss.good())
	{
		string subStr;
		getline(ss, subStr, ':');
		timeVec.push_back(subStr);
	}

	timeStr += timeVec[0] + timeVec[1];
	
	return timeStr;
}

/* Show software description and welcome messages to user */
void showWelcomeMsg()
{
	/* 
	Welcome to BLITZ. 
	BLITZ (Behavioral Learning In The Zebrafish) enables programmatic
	control in video capture, online image processing, visual stimuli 
	presentation and electric shock delivery, which allow researchers
	design their custom learning paradigm in zebrafish.
	Most updated code and other resources can be found at
	https://github.com/Wenlab/BLITZ
	Please send any feedback and suggestions to my email,
	bysin7@gmail.com Wenbin Yang.
	Copyright 2018 Wenbin Yang <bysin7@gmail.com>
	*/
	cout << "Welcome to BLITZ (Behavioral Learning In The Zebrafish) ." << endl
		<< "Most updated code and other resources can be found at " << endl
		<< "https://github.com/Wenlab/BLITZ" << endl
		<< "Please send any feedback and suggestions to my email: " << endl
		<< "bysin7@gmail.com Wenbin Yang." << endl
		<< "Copyright 2018 Wenbin Yang <bysin7@gmail.com>" << endl;
	cout << endl; // separated with an empty line

	cout << "Please make sure all devices are connected." << endl;
	cout << "Cameras, the relay, the projector." << endl;

}

/*
Diagram of fish positions in arena
|		|		|
|	0   |   1	|
|		|		|
|---------------|
|		|		|
|	2   |   3	|
|		|		|
*/
void showFishPosDiagram()
{
	const int numSpaces = 3;
	const string spaces(numSpaces, ' ');
	const string cutLineL(4 * numSpaces + 3, '-');
	// show the fish indices diagram
	cout << "Diagram of fish indices in the arena: " << endl;
	cout << cutLineL << endl;
	cout << spaces << "0" << spaces << "|" << spaces << "1" << spaces << endl;
	cout << cutLineL << endl;
	cout << spaces << "2" << spaces << "|" << spaces << "3" << spaces << endl;
	cout << cutLineL << endl;
	cout << endl; // separated with an empty line
}

/* Ask for how many cameras to use in the experiment */
int enquireNumCams()
{
	int nCams;

	cout << "Number of cameras to use? (3, 2, 1)" << endl;
	cin >> nCams;
	cout << endl; // separated with an empty line

	return nCams;
}

/* Ask for fish IDs in the arena */
vector<string> enquireFishIDs(int arenaIdx)
{
	showFishPosDiagram();
	cout << "Enter fishIDs used in the Arena " << arenaIdx + 1 << endl;
	cout << "Enter all fishIDs with ',' as separator. (e.g. G11,G14)" << endl;

	string inputStr;
	getline(cin, inputStr);
	cout << endl; // separated with an empty line
	vector<string> fishIDs;

	istringstream ss;
	ss.clear();
	ss.str(inputStr);
	while (ss.good())
	{
		string subStr;
		getline(ss, subStr, ',');
		fishIDs.push_back(subStr);
	}

	return fishIDs;
}

int enquireFishAge()
{
	cout << "Enter age for all fish. (A number, e.g. 9)" << endl;
	cout << "(Assume they are at the same age)" << endl;

	int fishAge;
	while (!(cin >> fishAge)) // wrong input
	{
		cout << "Wrong input for fish age" << endl
			<< "Please enter a number. (e.g. 9)" << endl;
		cin.clear();
		cin.ignore(256, '\n');
	}
	cout << endl; // separated with an empty line
	return fishAge;
}

/* Ask for what experiment task for poor fish */
string enquireExpTask()
{
	string expTask;
	cout << "Enter task for all fish (e.g. OLcontrol, OLexp)" << endl;
	cin >> expTask;

	if (iequals(expTask,"OLcontrol"))
	{
		cout << "Make sure the OUTPUT button is" << endl
			<< "OFF (on the power source)" << endl;
	}
	else if (iequals(expTask, "OLexp"))
	{
		cout << "Make sure the OUTPUT button is" << endl
			<< "ON (on the power source)" << endl;
	}
	else {
		cout << "Unrecognized expType" << endl;
	}

	cout << endl;

	// process the extra '\n'
	// Get preparation done for getline
	// should only be done for once
	char c;
	while (cin.get(c) && c != '\n')
		if (!isspace(c))
		{
			std::cerr << "ERROR unexpected character '" << c << "' found\n";
			exit(EXIT_FAILURE);
		}

	return expTask;
}

/* convert string vector to int-vector like formatted output */
string strVec2str(vector<string> strVec)
{
	if (strVec.empty()) // vector is empty, return empty string
		return "";

	string str = strVec[0];
	for (int j = 1; j < strVec.size(); j++)
	{
		str += "," + strVec[j];
	}
	//str += "]";
	return str;
}


bool iequals(const string& a, const string& b)
{
	unsigned int sz = a.size();
	if (b.size() != sz)
		return false;
	for (unsigned int i = 0; i < sz; ++i)
		if (tolower(a[i]) != tolower(b[i]))
			return false;
	return true;
}
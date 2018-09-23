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
#include <ctime> // to get the current date and time
#include <algorithm>  // include the algorithm reverse


using namespace std;
using namespace cv;

bool WriteOutData::initialize(string pathName, int width, int height, int frameRate, int x_cut, int y_cut, vector<vector<int>> yDivs)
{
	for (int i = 0; i < numFiles; i++)
	{
		string videoName = pathName + baseNames[i] + ".avi";
		string yamlName = pathName + baseNames[i] + ".yaml";
		FileStorage fObj(yamlName, FileStorage::WRITE);
		if (!fObj.isOpened())
			return false;
		yamlVec.push_back(fObj);
		VideoWriter vObj(videoName, CV_FOURCC('D', 'I', 'V', 'X'), frameRate, Size(width, height), false);
		if (!vObj.isOpened())
			return false;
		videoVec.push_back(vObj);

	}
	writeOutExpSettings(frameRate, width, height,
		x_cut, y_cut, yDivs);

	
	return true;
	
}

/* Ask the user about the experiment infos */
int WriteOutData::enquireInfoFromUser()
{
	showWelcomeMsg();
	get_current_date_time();
	enquireNumCams();
	enquireAllBasenames();
	enquireStrainNames();
	enquireFishAge();
	enquireExpTask();
	// Enquire fish IDs for all arenas
	enquireFishIDs();
	//get_strainNames();
	getBasenames();

	return numFiles;
}

/* Get current date and time string from chrono system clock */
void WriteOutData::get_current_date_time()
{
	// Get system time
	time_t rawtime;
	struct tm timeinfo;
	char buffer[80];

	time(&rawtime);

	int errCode = localtime_s(&timeinfo, &rawtime);
	strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M", &timeinfo);
	timeStr = buffer;
}

/* Ask for the number of cameras to use in the experiment */
void WriteOutData::enquireNumCams()
{
	cout << "Number of cameras to use? (3, 2, 1)" << endl;
	cin >> numFiles;
	cout << endl; // separated with an empty line
}

/* Ask for CS strs in the screen */
void WriteOutData::enquireAllBasenames()
{
	showFishPosDiagram();
	for (int i = 0; i < numFiles; i++)
	{
		CSstrs.push_back(enquireBasename(i));
	}
}

/* get CS str in the area */
string WriteOutData::enquireBasename(int areaIdx) 
{
	cout << "Enter the pattern used in the Arena " << areaIdx + 1 << endl;
	string basename;
	cin >> basename;
    // separated with an empty line
	cout << endl;
	return basename;
	
}

/* Ask for what strain of Fish is using */
void WriteOutData::enquireStrainNames()
{
	cout << "Enter the strain name for all fish. (A number, e.g. GCaMP6f)" << endl;
	cout << "(Assume they are the same strain)" << endl;
	
	string strainName;
	cin >> strainName;

	for (int i = 0; i < numFiles; i++)
	{
		strainNames.push_back(strainName);
	}
}

/* Ask for the age for all fish */
void WriteOutData::enquireFishAge()
{
	cout << "Enter age for all fish. (A number, e.g. 9)" << endl;
	cout << "(Assume they are at the same age)" << endl;

	while (!(cin >> fishAge)) // wrong input
	{
		cout << "Wrong input for fish age" << endl
			<< "Please enter a number. (e.g. 9)" << endl;
		cin.clear();
		cin.ignore(256, '\n');
	}
	cout << endl; // separated with an empty line
}



/* Ask for what experiment task for poor fish */
void WriteOutData::enquireExpTask()
{
	cout << "Enter task for all fish (e.g. OLcontrol, OLexp)" << endl;
	cin >> expTask;

	if (iequals(expTask, "OLcontrol"))
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

}

/* Ask for fish IDs for all arenas */
void WriteOutData::enquireFishIDs()
{
	for (int i = 0; i < numFiles; i++)
		fishIDs.push_back(enquireFishIDs(i));
}



/* Ask for fish IDs in the arena */
vector<string> WriteOutData::enquireFishIDs(int arenaIdx)
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

/* Get strain names of fish in all arenas */
void WriteOutData::get_strainNames()
{
	for (int i = 0; i < numFiles; i++)
	{
		strainNames.push_back(get_strainName(fishIDs[i]));
	}
}
/* Get strain name of fish in the arena */
string WriteOutData::get_strainName(vector<string> fishIDs)
{
	char firstChar = fishIDs[0][0];
	string strainName;
	if (firstChar == 'G' || firstChar == 'g') // GCaMP fish
		strainName += "GCaMP";
	else if (firstChar == 'S' || firstChar == 's')
		strainName += "WT";
	return strainName;
}



/* Get basename for the output files */
void WriteOutData::getBasenames()
{
	for (int i = 0; i < numFiles; i++)
		baseNames.push_back(getBasename(i));
}

/* Get basename for the output files */
string WriteOutData::getBasename(int arenaIdx)
{
	
	string baseName =
		timeStr + "_" + "Arena" + to_string(arenaIdx + 1)
		+ "_" + strainNames[arenaIdx] + "_" 
		+ to_string(fishAge)+ "dpf_" 
		+ expTask + "_" + CSstrs[arenaIdx];
	return baseName;
}

/* Get CS strings for all arena */
void WriteOutData::get_CS_strings(vector<const char*> CSpatterns)
{
	for (int i = 0; i < CSpatterns.size(); i++)
	{
		CSstrs.push_back(get_CS_string(CSpatterns[i]));
	}
}

/* Get CS string to append to the filenames of yaml and video files */
string WriteOutData::get_CS_string(const char* CSpattern)
{
	
	string patternStr = extractPatternName(CSpattern);
	string CSstr;
	if (patternStr.compare("redBlackCheckerboard") == 0)
		CSstr = "RBC";
	else if (patternStr.compare("whiteBlackCheckerboard") == 0)
		CSstr = "WBC";
	else if (patternStr.compare("pureBlack") == 0)
		CSstr = "PB";
	else
	{
		CSstr = patternStr;
		cout << "CS pattern is: " << CSstr << endl;

	}
	return CSstr;

}

/* Write out experiment settings as the header for files 
	only write once */
void WriteOutData::writeOutExpSettings(
	int frameRate,
	int width,
	int height,
	int x_cut,
	int y_cut,
	vector<vector<int>> yDivs
	)
{
	for (int i = 0; i < numFiles; i++)
	{
		writeKeyValuePair("FishIDs", strVec2str(fishIDs[i]), i);
		writeKeyValuePair("FishAge", fishAge, i);
		writeKeyValuePair("FishStrain", strainNames[i], i);
		writeKeyValuePair("Arena", i + 1, i); // record which arena is in use
		writeKeyValuePair("Task", expTask, i);
		writeKeyValuePair("CSpattern", CSstrs[i], i);
		writeKeyValuePair("ExpStartTime", timeStr, i);
		writeKeyValuePair("FrameRate", frameRate, i);
		writeKeyValuePair("FrameSize", Size(width, height), i);
		writeKeyValuePair("xCut", x_cut, i);
		writeKeyValuePair("yCut", y_cut, i);
		writeKeyValuePair("yDivide", yDivs[i], i);
	}
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

	foundIdx = s.find('.');
	if (foundIdx != string::npos)
		endIdx = s.size() - foundIdx;
	else {
		cout << "Wrong input pattern directory!" << endl;
		cout << "Please check!" << endl;
		exit(0);
	}
	
	std::reverse(s.begin(), s.end()); // reverse back
	string patternName = s.substr(startIdx , endIdx - startIdx - 1);
	return patternName;
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
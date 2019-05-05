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
* Filename: userInterface.cpp
* Abstract: this file contains all function definitions
*			used to communicate with users through an command line interface
*
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>

* Created on: Apr. 15, 2018
* Modified on: Apr. 20, 2019
*/
#include "userInterface.h"
#include "errorHandling.h"
#include <algorithm> // remove all spaces from a string

using namespace std;


/* 
void UserInterface::enquireInfoFromUser()
{
	showWelcomeMsg();
	enquireDevice2use();
	if (devices2use[2])
		enquireCameras2use(is);
	for (int i = 0; i < cameras2open.size(); i++)
	{
		if (cameras2open[i])
		{
			numOpenCameras++;
			enquirePattern2use();
			int numFish = enquireNumFishForACam(i);
			fishIDs.push_back(generateFishIDs(numFish));
			arenaIDs.push_back(i);
		}

	}
	enquireFishStrain();
	enquireFishAge();
	enquireExpTask();

	startTimeStr = getCurDateTime();
	generateBasenames();
}
*/

void UserInterface::enquireDevice2use(std::istream& is)
{
	const int numDevices = 3;
	devices2use.resize(numDevices);
	cout << "Which devices are you going to use?" << endl
	<< " Please enter all the indices. (1. relay, 2. projector, 3. cameras)" << endl
	<< "e.g., '1,2,3' to open all the devices" << endl;

	vector<string> strVec = getStrVecFromCMD(is);
	for (string s : strVec)
	{
		if (!s.compare("1"))
			devices2use[0] = 1;
		else if (!s.compare("2"))
			devices2use[1] = 1;
		else if (!s.compare("3"))
			devices2use[2] = 1;
		
		else
		{
			cout << "Invalid input! Please enter again." << endl;
			waitUserInput2exit();
		}
	}

}



void UserInterface::enquireCameras2use(std::istream& is)
{
	cout << "How would you use the middle, left, and right cameras?" << endl
		<< "Enter an array to tell the system" << endl
		<< "(e.g., '0,1,1' to open the left and right cameras, but not the middle one.)" << endl;

	vector<string> tempStrVec = getStrVecFromCMD(is);
	int numCameras = tempStrVec.size();
	cameras2open.resize(numCameras);

	for (int i = 0; i < numCameras; i++)
		cameras2open[i] = stoi(tempStrVec[i]);
	


	cout << endl; // separated with an empty line
}

/*
void UserInterface::enquirePattern2use()
{

}

int UserInterface::enquireNumFishForACam(int idxCamera)
{
	string camPos;
	if (idxCamera == 0)
	{
		camPos = "Middle";
	}
	else if (idxCamera == 1)
	{
		camPos = "Left";
	}
	else if (idxCamera == 2)
	{
		camPos = "Right";
	}
	else{
		cout << "Invalid input for idxCamera!" << endl;
		waitUserInput2exit();
	}

	cout << "How many fish are under the " << camPos
	<< " camera? (e.g., 4)" << endl;
	int numFish; // TODO: add errorHandling over the wrong datatype
	cin >> numFish;
	cout << endl;

	return numFish;
}

void UserInterface::enquirePattern2use()
{
	// TODO: reduce the code duplication
	// duplicated with the 'enquireNumFishForACam'
	string camPos;
	if (idxCamera == 0)
	{
		camPos = "Middle";
	}
	else if (idxCamera == 1)
	{
		camPos = "Left";
	}
	else if (idxCamera == 2)
	{
		camPos = "Right";
	}
	else{
		cout << "Invalid input for idxCamera!" << endl;
		waitUserInput2exit();
	}

	cout << "Enter the pattern to use under the "
	<< camPos << " camera? (e.g., fullBlue)" << endl;

	string CSpattern;
	cin >> CSpattern;

	CSstrs.push_back(CSpattern);

}

vector<int> UserInterface::generateFishIDs(int numFish)
{
	vector<int> fishIDs;
	for (int i = 0; i < numFish; i++)
		fishIDs.push_back(i+1);
}

void UserInterface::enquireFishStrain()
{
	cout << "Enter the strain name for all fish. (A number, e.g. GCaMP6f)" << endl;
	cout << "(Assume they are the same strain)" << endl;

	cin >> strainName;

	cout << endl; // leave an empty line

}

void UserInterface::enquireFishAge()
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

void UserInterface::enquireExpTask()
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
	else if (iequals(expTask, "BlueTest"))
	{
		cout << "Make sure the `fullBlue` pattern" << endl
			<< "is using." << endl;
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



string UserInterface::generateBasenames(int idxFile)
{
	string baseName =
		timeStr + "_" + "Arena" + to_string(arenaIDs[idxFile])
		+ "_" + strainNames[arenaIdx] + "_"
		+ to_string(fishAge)+ "dpf_"
		+ expTask;
	return baseName;
}

*/



void showWelcomeMsg()
{
	/*
	Welcome to BLITZ.
	BLITZ (Behavioral Learning In The Zebrafish) enables programmatic
	control in video capture, online image processing, visual stimuli
	presentation and electric shock delivery, which allow researchers
	design their custom learning paradigms in zebrafish.
	Most updated code and other resources can be found at
	https://github.com/Wenlab/BLITZ
	Please send any feedback and suggestions to my email,
	bysin7@gmail.com Wenbin Yang.
	Copyright 2018 Wenbin Yang <bysin7@gmail.com>
	*/
	cout << "Welcome to BLITZ (Behavioral Learning In The Zebrafish)." << endl
		<< "This program is under GNU 3.0 License." << endl 
		<< "Most updated code and other resources can be found at " << endl
		<< "https://github.com/Wenlab/BLITZ" << endl
		<< "Please cite (Wenbin Yang et al., 2019) if you use any portion of this program." << endl
		<< "Please send any feedback and suggestions to the email: " << endl
		<< "bysin7@gmail.com Wenbin Yang." << endl
		<< "Copyright 2018 Wenbin Yang <bysin7@gmail.com>" << endl;
	cout << endl; // separated with an empty line

}

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

vector<string> getStrVecFromCMD(std::istream& is)
{
	vector<string> strVec;
	string inputStr;
	getline(is, inputStr);
	cout << endl; // separated with an empty line
	

	istringstream ss;
	ss.clear();
	ss.str(inputStr);
	while (ss.good())
	{
		string subStr;
		getline(ss, subStr, ',');
		// remove all spaces from a string
		subStr.erase(remove_if(subStr.begin(), subStr.end(), isspace), subStr.end());
		strVec.push_back(subStr);
	}

	return strVec;
}

string getCurDateTime()
{
	// Get system time
	time_t rawtime;
	struct tm timeinfo;
	char buffer[80];

	time(&rawtime);

	string timeStr;
	int errCode = localtime_s(&timeinfo, &rawtime);
	strftime(buffer, sizeof(buffer), "%Y%m%d_%H%M", &timeinfo);
	timeStr = buffer;
	return timeStr;
}

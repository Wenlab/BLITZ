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
#include <algorithm> // remove all spaces from a string

using namespace std;


/* */
void UserInterface::enquireInfoFromUser()
{
	showWelcomeMsg();

	enquireFishStrain(cin);
	enquireFishAge(cin);
	enquireExpTask(cin);

	startTimeStr = getCurDateTime();
	generateBasenames();
}



void UserInterface::enquireFishStrain(std::istream& is)
{
	cout << "Enter the strain name for all fish. (A number, e.g. GCaMP6f)" << endl;
	cout << "(Assume they are the same strain)" << endl;

	is >> strainName;

	cout << endl; // leave an empty line

}

void UserInterface::enquireFishAge(std::istream& is)
{
	cout << "Enter age for all fish. (A number, e.g. 9)" << endl;
	cout << "(Assume they are at the same age)" << endl;

	while (!(is >> fishAge)) // wrong input
	{
		cout << "Wrong input for fish age" << endl
			<< "Please enter a number. (e.g. 9)" << endl;
		is.clear();
		is.ignore(256, '\n');
	}
	cout << endl; // separated with an empty line
}

void UserInterface::enquireExpTask(std::istream& is)
{
	cout << "Enter task for all fish (e.g. OLcontrol, OLexp)" << endl;
	is >> expTask;

	if (!expTask.compare("OLcontrol"))
	{
		cout << "Make sure the OUTPUT button is" << endl
			<< "OFF (on the power source)" << endl;
	}
	else if (!expTask.compare("OLexp"))
	{
		cout << "Make sure the OUTPUT button is" << endl
			<< "ON (on the power source)" << endl;
	}
	else if (!expTask.compare("BlueTest"))
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


void UserInterface::generateBasenames()
{
	
	baseNames=generateBasename(0);

}

string UserInterface::generateBasename(int idxFile)
{
	string baseName =
		startTimeStr
		+ "_" + strainName + "_"
		+ to_string(fishAge) + "dpf_"
		+ expTask;
	return baseName;
}


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
//void waitUserInput2exit()
//{
//	do
//	{
//		cout << '\n'
//			<< "Please check the error message and press a key to exit...";
//	} while (cin.get() != '\n');
//	exit(0);
//}
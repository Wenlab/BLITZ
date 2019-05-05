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
* Filename: main.cpp
* Abstract: this file contains all functions used in constructing final
*			behavioral learning experiment in zebrafish
*
* Current Version: 3.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: Apr. 20, 2019
*/

// Include user-defined libraries
//#include "3rdPartyLibs/OpenGL/shader_s.h"
#include <iostream>
#include <sstream>
#include <vector>


#include "MyLibs/userInterface.h"

using namespace std;



int main()
{
	UserInterface UIobj;

	UIobj.enquireInfoFromUser();

	// display all the input in the CMD
	cout << "devices2use: " << endl;
	vector<bool>& devices2use = UIobj.devices2use;
	for (int i = 0; i < devices2use.size(); i++)
		cout << devices2use[i] << endl;

	cout << "cameras2open: " << endl;
	vector<bool>& cameras2open = UIobj.cameras2open;
	for (int i = 0; i < cameras2open.size(); i++)
		cout << cameras2open[i] << endl;

	cout << "allFishIDs: " << endl;
	std::vector<std::vector<int>> allFishIDs = UIobj.allFishIDs;
	for (int i = 0; i < allFishIDs.size(); i++)
	{
		cout << "Arena " << UIobj.arenaIDs[i] << " :" << endl;
		vector<int> fishIDs = allFishIDs[i];
		for (int j = 0; j < fishIDs.size(); j++)
			cout << fishIDs[j] << endl;
	}

	cout << "fishAge: " << endl;
	int fishAge = UIobj.fishAge;
	cout << fishAge << endl;

	cout << "fishStrain: " << endl;
	string strainName = UIobj.strainName;
	cout << strainName << endl;

	cout << "expTask: " << endl;
	string expTask = UIobj.expTask;
	cout << expTask << endl;

	cout << "visPattern: " << endl;
	string visPattern = UIobj.visPattern;
	cout << visPattern << endl;

	cout << "startTimeStr: " << endl;
	string startTimeStr = UIobj.startTimeStr;
	cout << startTimeStr << endl;

	cout << "baseNames: " << endl;
	vector<string> baseNames = UIobj.baseNames;
	for (int i = 0; i < baseNames.size(); i++)
		cout << baseNames[i] << endl;


	cout << endl << "All tests finished. Press key to exit." << endl;
	getchar();
}




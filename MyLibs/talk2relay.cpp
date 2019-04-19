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
* Filename: talk2relay.cpp
* Abstract: this file contains all function definitions
*			used in serial communication
*
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: Apr. 28, 2018

* Replaced Version: 1.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
*/

// Include user-defined libraries
#include "talk2relay.h"

using namespace std;

void Relay::initialize(int com_num)
{
	try {
		tryCatchFalse(sPort.InitPort(com_num),"The relay initiation failed!");
	} catch (string errorMsg) {
		cout << errorMsg << endl;
		waitUserInput2exit();
	}

	cout << "The relay initiation succeeded! " << endl;

}
/* Open channel by index */
void Relay::givePulse(int idxChannel)
{
	float openDuration = 0.1; // seconds
	vector<bool> openStatuses(NUM_CHANNEL, 0);
	openStatuses[idxChannel] = 1;
	givePulse(openStatuses, openDuration);
}

void Relay::givePulse(vector<bool> channelStatuses, float openDuration) // TODO: -> 16-bit boolean array, or a 16-bit binary number
{
	const unsigned char BIT_1 = 0x00; // default
	const unsigned char BIT_2 = 0x5A; // device number
	const unsigned char BIT_3 = 0x60; // Board number
	const unsigned char BIT_4 = 0x01; // address number
	const unsigned char BIT_5 = 0x12; // operation number


	unsigned char sumCh0to7 = 0;
	unsigned char sumCh8to15 = 0;
	unsigned char numUnit = ceil(openDuration * 10); // openDuration in unit count; an unit is 0.1s

	try {
		tryCatchFalse((numUnit > 0), "openDuration should be a positive number!");
	} catch (string errorMsg) {
		cout << errorMsg << endl;
	}

	for (int i = 7; i >= 0; i--)
	{
		sumCh0to7 <<= 1;
		sumCh0to7 += channelStatuses[i];
	}
	for (int i = 15; i >= 8; i--)
	{
		sumCh8to15 <<= 1;
		sumCh8to15 += channelStatuses[i];
	}

	unsigned char sumCheck = BIT_1 + BIT_2 + BIT_3 + BIT_4 + BIT_5 + sumCh0to7 + sumCh8to15 + numUnit;
	unsigned char openCommand[LEN_COMMAND] = {BIT_1, BIT_2, BIT_3, BIT_4, BIT_5,
		 sumCh0to7, sumCh8to15, numUnit, sumCheck}

	try {
		tryCatchFalse(sPort.WriteData(openCommand, LEN_COMMAND),"Cannot write to the relay!")
	} catch (string errorMsg) {
		cout << errorMsg << endl;
		waitUserInput2exit();
	}



}

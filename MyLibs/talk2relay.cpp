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
// Include user-defined libraries
#include "errorHandling.h"

using namespace std;

void Relay::initialize(int com_num)
{

	tryCatchFalse(sPort.InitPort(com_num),"The relay initiation failed!");
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

void Relay::givePulse(vector<bool> channelStatuses, float openDuration=0.1)
{
	tryCatchFalse((openDuration > 0.1), "openDuration should be at least 0.1 second!");
	unsigned char* openCommand = new unsigned char[LEN_COMMAND];
	generateOpenCommand(openCommand, channelStatuses, openDuration);

	tryCatchFalse(sPort.WriteData(openCommand, LEN_COMMAND),"Cannot write to the relay!");
}

void Relay::generateOpenCommand(unsigned char* openCommand, vector<bool> channelStatuses, float openDuration)
{
	const unsigned char BIT_5 = 0x12; // operation number
	unsigned char BIT_6 = binaryAdding(channelStatuses.begin(), channelStatuses.begin() + 8);
	unsigned char BIT_7 = binaryAdding(channelStatuses.begin() + 8, channelStatuses.end());
	unsigned char BIT_8 = ceil(openDuration * 10); // openDuration in unit count; an unit is 0.1s

	unsigned char sumCheck = BIT_1 + BIT_2 + BIT_3 + BIT_4 + BIT_5 + BIT_6 + BIT_7 + BIT_8;
	unsigned char command[] = { BIT_1, BIT_2, BIT_3, BIT_4, BIT_5,
		BIT_6, BIT_7, BIT_8, sumCheck };

	std::copy(std::begin(command), std::end(command), openCommand);

}

void Relay::generateOpenCommand(unsigned char* openCommand, vector<bool> channelStatuses)
{
	const unsigned char BIT_5 = 0x01; // operation number
	unsigned char BIT_6 = binaryAdding(channelStatuses.begin(), channelStatuses.begin() + 8);
	unsigned char BIT_7 = binaryAdding(channelStatuses.begin() + 8, channelStatuses.end());
	const unsigned char BIT_8 = 0; // not used in this context

	unsigned char sumCheck = BIT_1 + BIT_2 + BIT_3 + BIT_4 + BIT_5 + BIT_6 + BIT_7 + BIT_8;
	unsigned char command[] = { BIT_1, BIT_2, BIT_3, BIT_4, BIT_5,
		BIT_6, BIT_7, BIT_8, sumCheck };

	std::copy(std::begin(command), std::end(command), openCommand);

}

int binaryAdding(vector<bool>::iterator vecBegin, vector<bool>::iterator vecEnd)
{
	int sum = 0;
	for (vector<bool>::iterator it = vecBegin; it != vecEnd; ++it)
		sum += (*it << (it - vecBegin));
	
	return sum;
}
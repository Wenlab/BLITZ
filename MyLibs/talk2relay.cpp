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



bool Relay::initialize(int com_num)
{
	if (!sPort.InitPort(com_num))
	{
		std::cout << "SerialPort initiation failed! " << std::endl;
		return false;
	}
	else
	{
		std::cout << "SerialPort initiation succeeded! " << std::endl << std::endl;
	}
	return true;
}
/* Open channel by index */
bool Relay::givePulse(int idxChannel)
{
	bool res = sPort.WriteData(openCommands[idxChannel], LEN_COMMAND);
	return res;
}

bool Relay::givePulse(vector<int> channelIndices,int openDuration)
{
	// TODO: write the code to open multiple channesl simultaneously,
	// might need to generate new 16-base code
	int channelIndices_0_7 = 0, channelIndices_8_15 = 0;
	for (int i = 7; i >= 0; i--) {
		channelIndices_0_7 <<= 1;
		channelIndices_0_7 += channelIndices[i];
	}
	for (int i = 15; i >= 8; i--) {
		channelIndices_8_15 <<= 1;
		channelIndices_8_15 += channelIndices[i];
	}
	int check = 0x00 + 0x5A + 0x60 + 0x01 + 0x12 + channelIndices_0_7 + channelIndices_8_15 + openDuration;
	unsigned char openCommand[LEN_COMMAND]= { 0x00,0x5A,0x60,0x01,0x12,channelIndices_0_7,channelIndices_8_15,openDuration,check };
	bool res = sPort.WriteData(openCommand, LEN_COMMAND);
	return res;
}

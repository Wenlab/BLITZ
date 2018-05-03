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


/* Commands to open specific channel on relay for several seconds*/
unsigned char openCommands[NUM_CHANNEL][LEN_COMMAND] =
{
	{ 0x00,0x5A,0x54,0x00,0x12,0x01,0x00,0x01,0xC2 }, //open the channel 1 for 0.1 s
	{ 0x00,0x5A,0x54,0x00,0x12,0x02,0x00,0x01,0xC3 }, //open the channel 2 for 0.1 s
	{ 0x00,0x5A,0x54,0x00,0x12,0x04,0x00,0x01,0xC5 }, //open the channel 3 for 0.1 s
	{ 0x00,0x5A,0x54,0x00,0x12,0x08,0x00,0x01,0xC9 }, //open the channel 4 for 0.1 s
	{ 0x00,0x5A,0x54,0x00,0x12,0x10,0x00,0x01,0xD1 }, //open the channel 5 for 0.1 s
	{ 0x00,0x5A,0x54,0x00,0x12,0x20,0x00,0x01,0xE1 }, //open the channel 6 for 0.1 s
	{ 0x00,0x5A,0x54,0x00,0x12,0x40,0x00,0x01,0x01 }, //open the channel 7 for 0.1 s
	{ 0x00,0x5A,0x54,0x00,0x12,0x80,0x00,0x01,0x41 }  //open the channel 8 for 0.1 s
};

bool PortData::initialize(int com_num)
{
	if (!port.InitPort(com_num))
	{
		std::cout << "SerialPort initiation failed! " << std::endl;
		return false;
	}
	else
	{
		std::cout << "SerialPort initiation succeeded! " << std::endl;
	}
	return true;	
}
/* Open channel by index */
bool PortData::givePulse(int idxChannel)
{
	return port.WriteData(openCommands[idxChannel], LEN_COMMAND);
}


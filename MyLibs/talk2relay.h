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
* Filename: talk2relay.h
* Abstract: this file contains all classes and functions' declarations
*			used in serial communication 
*
* Current Version: 2.0
* Author: Wenbin Yang <bysin7@gmail.com>
* Modified on: Apr. 28, 2018

* Replaced Version: 1.1
* Author: Wenbin Yang <bysin7@gmail.com>
* Created on: Jan. 1, 2018
*/

#ifndef _GUARD_TALK2RELAY_H
#define _GUARD_TALK2RELAY_H

// Include 3rd party libraries
#include "../3rdPartyLibs/SerialCom/SerialPort.h"

// Include standard libraries
#include <iostream>

#define NUM_CHANNEL 8
#define LEN_COMMAND 9

class PortData
{
private:
	;
public:
	// methods
	PortData()
	{
		
	}

	/* Initialize the serial port by COM number*/
	bool initialize(int com_num);
	/* Open channel for several seconds by index */
	bool givePulse(int idxChannel);

	// properties
	CSerialPort port;
	unsigned char openCommands[NUM_CHANNEL][LEN_COMMAND];

};



#endif // !_GUARD_TALK2RELAY_H
